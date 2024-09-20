#include "LinkHandler.hpp"

using namespace commonModule;

LinkHandler::LinkHandler(ScMemoryContext * context)
{
  this->context = context;
}

ScAddr LinkHandler::createLink(const std::string & text)
{
  ScAddr addr = context->GenerateLink();
  context->SetLinkContent(addr, ScStreamConverter::StreamFromString(text));
  return addr;
}
