#include "MessageHandler.hpp"

#include "keynodes/MessageKeynodes.hpp"

using namespace dialogControlModule;
using namespace utils;

MessageHandler::MessageHandler(ScAgentContext * context, utils::ScLogger * logger)
  : context(context)
  , logger(logger)
  , languageSearcher(std::make_unique<LanguageSearcher>(context, logger))
  , messageConstructionsGenerator(std::make_unique<MessageConstructionsGenerator>(context))
  , messageSearcher(std::make_unique<commonModule::MessageSearcher>(context, logger))
  , phraseSearcher(std::make_unique<PhraseSearcher>(context, logger))
{
}

bool MessageHandler::processReplyMessage(
    ScAddr const & replyMessageNode,
    ScAddr const & logicRuleNode,
    ScAddr const & langNode,
    ScAddr const & parametersNode)
{
  bool result = false;

  if (!replyMessageNode.IsValid())
  {
    logger->Error(getClassNameForLog() + ": The message isn't valid");
    return false;
  }
  if (!logicRuleNode.IsValid())
  {
    logger->Error(getClassNameForLog() + ": The message logic rule isn't valid");
    return false;
  }
  logger->Debug(
      getClassNameForLog() + ": The logic rule " + context->GetElementSystemIdentifier(logicRuleNode) + " is found");

  if (!langNode.IsValid())
  {
    logger->Error(getClassNameForLog() + ": The message language isn't valid");
    return false;
  }
  logger->Debug(getClassNameForLog() + ": The message language is " + context->GetElementSystemIdentifier(langNode));
  if (!parametersNode.IsValid())
  {
    logger->Error(getClassNameForLog() + ": The parameters aren't valid");
    return false;
  }

  clearSemanticAnswer();
  context->GenerateConnector(ScType::ConstPermPosArc, parametersNode, langNode);
  if (context->CheckConnector(MessageKeynodes::concept_atomic_message, replyMessageNode, ScType::ConstPermPosArc))
  {
    logger->Debug(getClassNameForLog() + ": The message is atomic");
    ScAddr phraseClassNode = phraseSearcher->getFirstPhraseClass(logicRuleNode);
    if (phraseClassNode.IsValid())
      result = processAtomicMessage(replyMessageNode, phraseClassNode, parametersNode, langNode);
    else
      logger->Debug(getClassNameForLog() + ": The first phrase class isn't found");
  }
  else
  {
    logger->Debug(getClassNameForLog() + ": The message is non-atomic");
    result = processNonAtomicMessage(replyMessageNode, logicRuleNode, parametersNode, langNode);
  }
  return result;
}

bool MessageHandler::processAtomicMessage(
    ScAddr const & messageNode,
    ScAddr const & phraseClassNode,
    ScAddr const & parametersNode,
    ScAddr const & langNode)
{
  ScAddr resultLink = generateLinkByPhrase(messageNode, phraseClassNode, parametersNode, langNode);
  if (resultLink.IsValid())
  {
    messageConstructionsGenerator->generateTextTranslationConstruction(messageNode, resultLink);
    return true;
  }

  return false;
}

void MessageHandler::clearSemanticAnswer()
{
  ScTemplate clearAnswer;
  clearAnswer.Triple(MessageKeynodes::answer_structure, ScType::VarPermPosArc >> "_remove_arc", ScType::Unknown);

  ScTemplateSearchResult clearAnswerResult;
  context->SearchByTemplate(clearAnswer, clearAnswerResult);

  for (size_t i = 0; i < clearAnswerResult.Size(); i++)
  {
    context->EraseElement(clearAnswerResult[i]["_remove_arc"]);
  }
}

bool MessageHandler::processNonAtomicMessage(
    ScAddr const & replyMessageNode,
    ScAddr const & logicRuleNode,
    ScAddr const & parametersNode,
    ScAddr const & langNode)
{
  std::string linkContent;

  ScAddr messageNode = messageSearcher->getFirstMessage(replyMessageNode);
  if (!messageNode.IsValid())
  {
    logger->Debug(getClassNameForLog() + ": The first message isn't found");
    return false;
  }
  ScAddr phraseClassNode = phraseSearcher->getFirstPhraseClass(logicRuleNode);
  if (!phraseClassNode.IsValid())
  {
    logger->Debug(getClassNameForLog() + ": The first phrase class isn't found");
    return false;
  }

  std::string resultText;
  while (messageNode.IsValid() && phraseClassNode.IsValid())
  {
    ScAddr nextMessageNode;
    logger->Debug(
        getClassNameForLog() + ": The phrase class is " + context->GetElementSystemIdentifier(phraseClassNode));
    ScAddr resultLink = generateLinkByPhrase(replyMessageNode, phraseClassNode, parametersNode, langNode);
    if (!resultLink.IsValid())
      break;
    context->GetLinkContent(resultLink, linkContent);
    resultText = resultText.append(linkContent).append(" ");
    messageConstructionsGenerator->generateTextTranslationConstruction(messageNode, resultLink);
    messageNode = messageSearcher->getNextMessage(messageNode);
    phraseClassNode = phraseSearcher->getNextPhraseClass(phraseClassNode);
  }

  if (messageNode.IsValid() || phraseClassNode.IsValid())
  {
    logger->Error(getClassNameForLog() + ": No match between the message and the phrase class was found.");
    return false;
  }
  resultText = resultText.substr(0, resultText.size() - 1);
  logger->Debug(getClassNameForLog() + ": Result text: \"" + resultText + "\"");
  messageConstructionsGenerator->generateTextTranslationConstruction(replyMessageNode, langNode, resultText);

  return true;
}

ScAddr MessageHandler::generateLinkByPhrase(
    ScAddr const & replyMessageNode,
    ScAddr const & phraseClassNode,
    ScAddr const & parametersNode,
    ScAddr const & langNode)
{
  ScAddrVector phrases = phraseSearcher->getPhrases(phraseClassNode, langNode);
  if (phrases.empty())
  {
    logger->Debug(getClassNameForLog() + ": Phrases aren't found");
    return {};
  }

  ScAddr resultLink;
  for (auto phraseLink : phrases)
  {
    if (!phraseLink.IsValid())
      continue;

    std::string linkContent;
    context->GetLinkContent(phraseLink, linkContent);
    logger->Debug(getClassNameForLog() + ": The phrase with the content " + linkContent + " is found");

    ScAction phraseGenerationAction = context->GenerateAction(MessageKeynodes::action_phrase_generation);
    phraseGenerationAction.SetArguments(replyMessageNode, phraseLink, parametersNode);

    phraseGenerationAction.InitiateAndWait(PHRASE_GENERATION_AGENT_WAIT_TIME);

    if (phraseGenerationAction.IsFinishedSuccessfully())
    {
      resultLink = phraseGenerationAction.GetResult();
      context->GetLinkContent(resultLink, linkContent);
      logger->Debug(getClassNameForLog() + ": The result link with the content \"" + linkContent + "\" is generated");
      break;
    }
    else
    {
      context->GetLinkContent(phraseLink, linkContent);
      logger->Debug(getClassNameForLog() + ": The result link from the phrase \"" + linkContent + "\" isn't generated");
    }
  }

  return resultLink;
}

std::string MessageHandler::getClassNameForLog()
{
  static std::string const className = "MessageHandler";
  return className;
}
