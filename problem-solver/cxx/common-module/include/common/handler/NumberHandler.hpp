#pragma once

#include <sc-memory/sc_memory.hpp>

#include "common/handler/LinkHandler.hpp"

namespace commonModule
{
class NumberHandler
{
public:
  explicit NumberHandler(ScMemoryContext * context);

  ScAddr getNumberNode(const double & number);

  ScAddr findNumberNode(const double & number);

  ScAddr generateNumberNode(const double & number);

  std::string numberToLikView(const double & number);

private:
  ScMemoryContext * context;
  std::unique_ptr<LinkHandler> linkHandler;
};

}  // namespace commonModule
