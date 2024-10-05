#include "MessageHandler.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/MessageKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace dialogControlModule;
using namespace utils;

MessageHandler::MessageHandler(ScAgentContext * context)
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
      SC_LOG_DEBUG("The logic rule " << context->GetElementSystemIdentifier(logicRuleNode) << " is found");
      if (langNode.IsValid())
      {
        SC_LOG_DEBUG("The message language is " << context->GetElementSystemIdentifier(langNode));
        if (parametersNode.IsValid())
        {
          clearSemanticAnswer();
          context->GenerateConnector(ScType::EdgeAccessConstPosPerm, parametersNode, langNode);
          if (context->CheckConnector(
                  MessageKeynodes::concept_atomic_message, replyMessageNode, ScType::EdgeAccessConstPosPerm))
          {
            SC_LOG_DEBUG("The message is atomic");
            ScAddr phraseClassNode = phraseSearcher->getFirstPhraseClass(logicRuleNode);
            if (phraseClassNode.IsValid())
              result = processAtomicMessage(replyMessageNode, phraseClassNode, parametersNode, langNode);
            else
              SC_LOG_DEBUG("The first phrase class isn't found");
          }
          else
          {
            SC_LOG_DEBUG("The message is non-atomic");
            result = processNonAtomicMessage(replyMessageNode, logicRuleNode, parametersNode, langNode);
          }
        }
        else
          SC_LOG_ERROR("The parameters aren't valid");
      }
      else
        SC_LOG_ERROR("The message language isn't valid");
    }
    else
      SC_LOG_ERROR("The message logic rule isn't valid");
  }
  else
    SC_LOG_ERROR("The message isn't valid");

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
  context->SearchByTemplate(clearAnswer, clearAnswerResult);

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
  std::string linkContent;
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
        SC_LOG_DEBUG("The phrase class is " << context->GetElementSystemIdentifier(phraseClassNode));
        ScAddr resultLink = generateLinkByPhrase(replyMessageNode, phraseClassNode, parametersNode, langNode);
        if (resultLink.IsValid())
        {
          context->GetLinkContent(resultLink, linkContent);
          resultText = resultText.append(linkContent).append(" ");
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
        SC_LOG_ERROR("No match between the message and the phrase class was found.");
        result = false;
      }
      if (result)
      {
        resultText = resultText.substr(0, resultText.size() - 1);
        SC_LOG_DEBUG("Result text: \"" + resultText + "\"");
        messageConstructionsGenerator->generateTextTranslationConstruction(replyMessageNode, langNode, resultText);
      }
    }
    else
    {
      SC_LOG_DEBUG("The first phrase class isn't found");
      result = false;
    }
  }
  else
  {
    SC_LOG_DEBUG("The first message isn't found");
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
  std::string linkContent;
  vector<ScAddr> phrases = phraseSearcher->getPhrases(phraseClassNode, langNode);
  if (!phrases.empty())
  {
    for (auto phraseLink : phrases)
    {
      if (phraseLink.IsValid())
      {
        context->GetLinkContent(phraseLink, linkContent);
        SC_LOG_DEBUG(R"(The phrase with the content ")" << linkContent << R"(" is found)");

        ScAction phraseGenerationAction = ActionUtils::CreateAction(
            context, MessageKeynodes::action_phrase_generation, {replyMessageNode, phraseLink, parametersNode});

        ActionUtils::waitAction(context, phraseGenerationAction, PHRASE_GENERATION_AGENT_WAIT_TIME);

        if (context->CheckConnector(
                ScKeynodes::action_finished_successfully, phraseGenerationAction, ScType::EdgeAccessConstPosPerm))
        {
          resultLink = IteratorUtils::getAnyByOutRelation(context, phraseGenerationAction, ScKeynodes::nrel_result);
          context->GetLinkContent(resultLink, linkContent);
          SC_LOG_DEBUG("The result link with the content \"" << linkContent << "\" is generated");
          break;
        }
        else
        {
          context->GetLinkContent(phraseLink, linkContent);
          SC_LOG_DEBUG("The result link from the phrase \"" << linkContent << "\" isn't generated");
        }
      }
    }
  }
  else
  {
    SC_LOG_DEBUG("Phrases aren't found");
  }

  return resultLink;
}
