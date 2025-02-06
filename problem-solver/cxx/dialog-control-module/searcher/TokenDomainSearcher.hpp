#pragma once

#include <sc-memory/sc_addr.hpp>

#include <common/handler/LinkHandler.hpp>

namespace dialogControlModule
{
class TokenDomainSearcher
{
public:
  explicit TokenDomainSearcher(ScMemoryContext * context);

  ScAddr getMessageText(const ScAddr & message);

private:
  ScMemoryContext * context;
  std::unique_ptr<commonModule::LinkHandler> linkHandler;
};
}  // namespace dialogControlModule
