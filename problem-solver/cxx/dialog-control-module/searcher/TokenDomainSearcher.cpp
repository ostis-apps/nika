#include "TokenDomainSearcher.hpp"

#include <stdexcept>

#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "keynodes/DialogKeynodes.hpp"

using namespace dialogControlModule;
using namespace utils;

TokenDomainSearcher::TokenDomainSearcher(ScMemoryContext * context)
  : context(context)
  , linkHandler(std::make_unique<commonModule::LinkHandler>(context))
{
}

ScAddr TokenDomainSearcher::getMessageText(const ScAddr & message)
{
  ScAddr translationNode =
      utils::IteratorUtils::getAnyByInRelation(context, message, DialogKeynodes::nrel_sc_text_translation);
  if (!translationNode.IsValid())
  {
    throw std::runtime_error("Invalid params: the text of the message is not specified.");
  }

  ScIterator3Ptr iterator3 = context->CreateIterator3(translationNode, ScType::EdgeAccessConstPosPerm, ScType::Link);
  if (!iterator3->Next())
  {
    throw std::runtime_error("Invalid params: the text of the message is not specified.");
  }

  return iterator3->Get(2);
}
