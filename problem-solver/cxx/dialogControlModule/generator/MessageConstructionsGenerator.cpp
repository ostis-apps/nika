#include "MessageConstructionsGenerator.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "handler/LinkHandler.hpp"
#include "keynodes/MessageKeynodes.hpp"

namespace dialogControlModule
{
MessageConstructionsGenerator::MessageConstructionsGenerator(ScMemoryContext * context)
{
  this->context = context;
}

void MessageConstructionsGenerator::generateTextTranslationConstruction(
    const ScAddr & messageAddr,
    const ScAddr & langAddr,
    const std::string & messageText)
{
  commonModule::LinkHandler handler(context);

  ScAddr linkAddr = handler.createLink(messageText);
  context->CreateEdge(ScType::EdgeAccessConstPosPerm, langAddr, linkAddr);
  generateTextTranslationConstruction(messageAddr, linkAddr);
}

void MessageConstructionsGenerator::generateTextTranslationConstruction(
    const ScAddr & messageAddr,
    const ScAddr & linkAddr)
{
  ScAddr translationAddr = context->CreateNode(ScType::NodeConst);
  ScAddr translationEdgeAddr = context->CreateEdge(ScType::EdgeDCommonConst, translationAddr, messageAddr);
  context->CreateEdge(
      ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::nrel_sc_text_translation, translationEdgeAddr);
  context->CreateEdge(ScType::EdgeAccessConstPosPerm, translationAddr, linkAddr);
  context->CreateEdge(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_text_file, linkAddr);
}

}  // namespace dialogControlModule
