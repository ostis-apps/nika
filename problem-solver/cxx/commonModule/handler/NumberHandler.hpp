#pragma once

#include "sc-memory/sc_memory.hpp"

#include "handler/LinkHandler.hpp"

namespace commonModule
{
class NumberHandler
{
public:
  explicit NumberHandler(ScMemoryContext * ms_context);

  ~NumberHandler();

  ScAddr getNumberNode(const double & number);

  ScAddr findNumberNode(const double & number);

  ScAddr generateNumberNode(const double & number);

  std::string numberToLikView(const double & number);

private:
  ScMemoryContext * context;
  LinkHandler * linkHandler;
};

}  // namespace commonModule
