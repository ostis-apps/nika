#include "TokenDomainSearcher.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include <stdexcept>

using namespace dialogControlModule;
using namespace utils;

TokenDomainSearcher::TokenDomainSearcher(ScMemoryContext * context)
{
  this->context = context;
  this->linkHandler = new commonModule::LinkHandler(context);
}

TokenDomainSearcher::~TokenDomainSearcher()
{
  delete this->linkHandler;
}

ScAddr TokenDomainSearcher::getMessageText(const ScAddr & message)
{
  ScAddr translationNode =
      utils::IteratorUtils::getFirstByInRelation(context, message, DialogKeynodes::nrel_sc_text_translation);
  if (!translationNode.IsValid())
  {
    throw std::runtime_error("Invalid params: the text of the message is not specified.");
  }

  ScIterator3Ptr iterator3 = context->Iterator3(translationNode, ScType::EdgeAccessConstPosPerm, ScType::Link);
  if (!iterator3->Next())
  {
    throw std::runtime_error("Invalid params: the text of the message is not specified.");
  }

  return iterator3->Get(2);
}
