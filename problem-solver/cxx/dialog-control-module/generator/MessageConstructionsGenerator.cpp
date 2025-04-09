#include "MessageConstructionsGenerator.hpp"

#include "keynodes/MessageKeynodes.hpp"
#include <common/handler/LinkHandler.hpp>
#include <common/keynodes/Keynodes.hpp>

namespace dialogControlModule
{
MessageConstructionsGenerator::MessageConstructionsGenerator(ScMemoryContext * context)
  : context(context)
{
}

void MessageConstructionsGenerator::generateTextTranslationConstruction(
    const ScAddr & messageAddr,
    const ScAddr & langAddr,
    const std::string & messageText)
{
  commonModule::LinkHandler handler(context);

  ScAddr linkAddr = handler.createLink(messageText);
  context->GenerateConnector(ScType::ConstPermPosArc, langAddr, linkAddr);
  generateTextTranslationConstruction(messageAddr, linkAddr);
}

void MessageConstructionsGenerator::generateTextTranslationConstruction(
    const ScAddr & messageAddr,
    const ScAddr & linkAddr)
{
  ScAddr translationAddr = context->GenerateNode(ScType::ConstNode);
  ScAddr translationEdgeAddr = context->GenerateConnector(ScType::ConstCommonArc, translationAddr, messageAddr);
  context->GenerateConnector(
      ScType::ConstPermPosArc, commonModule::Keynodes::nrel_sc_text_translation, translationEdgeAddr);
  context->GenerateConnector(ScType::ConstPermPosArc, translationAddr, linkAddr);
  context->GenerateConnector(ScType::ConstPermPosArc, MessageKeynodes::concept_text_file, linkAddr);
}

}  // namespace dialogControlModule
