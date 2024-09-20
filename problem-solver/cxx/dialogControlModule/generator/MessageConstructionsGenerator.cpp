#include "MessageConstructionsGenerator.hpp"

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
  context->GenerateConnector(ScType::EdgeAccessConstPosPerm, langAddr, linkAddr);
  generateTextTranslationConstruction(messageAddr, linkAddr);
}

void MessageConstructionsGenerator::generateTextTranslationConstruction(
    const ScAddr & messageAddr,
    const ScAddr & linkAddr)
{
  ScAddr translationAddr = context->GenerateNode(ScType::NodeConst);
  ScAddr translationEdgeAddr = context->GenerateConnector(ScType::EdgeDCommonConst, translationAddr, messageAddr);
  context->GenerateConnector(
      ScType::EdgeAccessConstPosPerm, commonModule::Keynodes::nrel_sc_text_translation, translationEdgeAddr);
  context->GenerateConnector(ScType::EdgeAccessConstPosPerm, translationAddr, linkAddr);
  context->GenerateConnector(ScType::EdgeAccessConstPosPerm, MessageKeynodes::concept_text_file, linkAddr);
}

}  // namespace dialogControlModule
