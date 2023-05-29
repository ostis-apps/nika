#include "MessageHandler.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/MessageKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace scAgentsCommon;
using namespace dialogControlModule;
using namespace utils;

MessageHandler::MessageHandler(ScMemoryContext * context)
{
  this->context = context;
  languageSearcher = new LanguageSearcher(context);
  messageConstructionsGenerator = new MessageConstructionsGenerator(context);
  messageSearcher = new MessageSearcher(context);
  phraseSearcher = new PhraseSearcher(context);
}

MessageHandler::~MessageHandler()
{
  delete languageSearcher;
  delete messageConstructionsGenerator;
  delete messageSearcher;
  delete phraseSearcher;
}

bool MessageHandler::processReplyMessage(
    const ScAddr & replyMessageNode,
    const ScAddr & logicRuleNode,
    const ScAddr & langNode,
    const ScAddr & parametersNode)
{
  bool result = false;

  if (replyMessageNode.IsValid())
  {
    if (logicRuleNode.IsValid())
    {
      SC_LOG_DEBUG("MessageHandler: the logic rule " + context->HelperGetSystemIdtf(logicRuleNode) + " is found");
      if (langNode.IsValid())
      {
        SC_LOG_DEBUG("MessageHandler: the message language is " + context->HelperGetSystemIdtf(langNode));
        if (parametersNode.IsValid())
        {
          clearSemanticAnswer();
          context->CreateEdge(ScType::EdgeAccessConstPosPerm, parametersNode, langNode);
          if (context->HelperCheckEdge(
                  MessageKeynodes::concept_atomic_message, replyMessageNode, ScType::EdgeAccessConstPosPerm))
          {
            SC_LOG_DEBUG("MessageHandler: the message is atomic");
            ScAddr phraseClassNode = phraseSearcher->getFirstPhraseClass(logicRuleNode);
            if (phraseClassNode.IsValid())
              result = processAtomicMessage(replyMessageNode, phraseClassNode, parametersNode, langNode);
            else
              SC_LOG_DEBUG("MessageHandler: the first phrase class isn't found");
          }
          else
          {
            SC_LOG_DEBUG("MessageHandler: the message is non-atomic");
            result = processNonAtomicMessage(replyMessageNode, logicRuleNode, parametersNode, langNode);
          }
        }
        else
          SC_LOG_ERROR("MessageHandler: the parameters aren't valid");
      }
      else
        SC_LOG_ERROR("MessageHandler: the message language isn't valid");
    }
    else
      SC_LOG_ERROR("MessageHandler: the message logic rule isn't valid");
  }
  else
    SC_LOG_ERROR("MessageHandler: the message isn't valid");

  return result;
}

bool MessageHandler::processAtomicMessage(
    const ScAddr & messageNode,
    const ScAddr & phraseClassNode,
    const ScAddr & parametersNode,
    const ScAddr & langNode)
{
  bool result = false;

  ScAddr resultLink = generateLinkByPhrase(messageNode, phraseClassNode, parametersNode, langNode);
  if (resultLink.IsValid())
  {
    messageConstructionsGenerator->generateTextTranslationConstruction(messageNode, resultLink);
    result = true;
  }

  return result;
}

void MessageHandler::clearSemanticAnswer()
{
  ScTemplate clearAnswer;
  clearAnswer.Triple(MessageKeynodes::answer_structure, ScType::EdgeAccessVarPosPerm >> "_remove_arc", ScType::Unknown);

  ScTemplateSearchResult clearAnswerResult;
  context->HelperSearchTemplate(clearAnswer, clearAnswerResult);

  for (size_t i = 0; i < clearAnswerResult.Size(); i++)
  {
    context->EraseElement(clearAnswerResult[i]["_remove_arc"]);
  }
}

bool MessageHandler::processNonAtomicMessage(
    const ScAddr & replyMessageNode,
    const ScAddr & logicRuleNode,
    const ScAddr & parametersNode,
    const ScAddr & langNode)
{
  bool result = true;

  ScAddr messageNode = messageSearcher->getFirstMessage(replyMessageNode);
  if (messageNode.IsValid())
  {
    ScAddr phraseClassNode = phraseSearcher->getFirstPhraseClass(logicRuleNode);
    if (phraseClassNode.IsValid())
    {
      string resultText;
      do
      {
        ScAddr nextMessageNode;
        SC_LOG_DEBUG("MessageHandler: the phrase class is " + context->HelperGetSystemIdtf(phraseClassNode));
        ScAddr resultLink = generateLinkByPhrase(replyMessageNode, phraseClassNode, parametersNode, langNode);
        if (resultLink.IsValid())
        {
          resultText = resultText.append(CommonUtils::getLinkContent(context, resultLink)).append(" ");
          messageConstructionsGenerator->generateTextTranslationConstruction(messageNode, resultLink);
          messageNode = messageSearcher->getNextMessage(messageNode);
          phraseClassNode = phraseSearcher->getNextPhraseClass(phraseClassNode);
        }
        else
        {
          result = false;
        }
      } while (messageNode.IsValid() && phraseClassNode.IsValid() && result);
      if (messageNode.IsValid() || phraseClassNode.IsValid())
      {
        SC_LOG_ERROR("MessageHandler: no match between the message and the phrase class was found");
        result = false;
      }
      if (result)
      {
        resultText = resultText.substr(0, resultText.size() - 1);
        SC_LOG_DEBUG("MessageHandler: result text: \"" + resultText + "\"");
        messageConstructionsGenerator->generateTextTranslationConstruction(replyMessageNode, langNode, resultText);
      }
    }
    else
    {
      SC_LOG_DEBUG("MessageHandler: the first phrase class isn't found");
      result = false;
    }
  }
  else
  {
    SC_LOG_DEBUG("MessageHandler: the first message isn't found");
    result = false;
  }

  return result;
}

ScAddr MessageHandler::generateLinkByPhrase(
    const ScAddr & replyMessageNode,
    const ScAddr & phraseClassNode,
    const ScAddr & parametersNode,
    const ScAddr & langNode)
{
  ScAddr resultLink;

  vector<ScAddr> phrases = phraseSearcher->getPhrases(phraseClassNode, langNode);
  if (!phrases.empty())
  {
    for (auto phraseLink : phrases)
    {
      if (phraseLink.IsValid())
      {
        SC_LOG_DEBUG(
            "MessageHandler: the phrase with the content \"" + CommonUtils::getLinkContent(context, phraseLink) +
            "\" is found");
        ScAddr phraseGenerationActionNode = AgentUtils::initAgent(
            context, MessageKeynodes::action_phrase_generation, {replyMessageNode, phraseLink, parametersNode});

        ActionUtils::waitAction(context, phraseGenerationActionNode, PHRASE_GENERATION_AGENT_WAIT_TIME);

        if (context->HelperCheckEdge(
                CoreKeynodes::question_finished_successfully,
                phraseGenerationActionNode,
                ScType::EdgeAccessConstPosPerm))
        {
          resultLink = IteratorUtils::getAnyByOutRelation(
              context, phraseGenerationActionNode, scAgentsCommon::CoreKeynodes::nrel_answer);
          SC_LOG_DEBUG(
              "MessageHandler: the result link with the content \"" + CommonUtils::getLinkContent(context, resultLink) +
              "\" is generated");
          break;
        }
        else
        {
          SC_LOG_DEBUG(
              "MessageHandler: the result link from the phrase \"" + CommonUtils::getLinkContent(context, phraseLink) +
              "\" isn't generated");
        }
      }
    }
  }
  else
  {
    SC_LOG_DEBUG("MessageHandler: phrases aren't found");
  }

  return resultLink;
}
