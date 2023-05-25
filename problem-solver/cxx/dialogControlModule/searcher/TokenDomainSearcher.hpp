#pragma once

#include "sc-memory/sc_addr.hpp"

#include "handler/LinkHandler.hpp"
#include "keynodes/DialogKeynodes.hpp"

namespace dialogControlModule
{
class TokenDomainSearcher
{
public:
  explicit TokenDomainSearcher(ScMemoryContext * ms_context);

  ~TokenDomainSearcher();

  ScAddr getMessageText(const ScAddr & message);

private:
  ScMemoryContext * context;
  commonModule::LinkHandler * linkHandler;
};
}  // namespace dialogControlModule
