#pragma once

#include "sc-memory/sc_memory.hpp"

namespace commonModule
{
class NonAtomicActionInterpreter
{
public:
  explicit NonAtomicActionInterpreter(ScMemoryContext * ms_context);

  void interpret(const ScAddr & nonAtomicActionAddr);

private:
  const int WAIT_TIME = 48000;

  ScMemoryContext * context;

  ScAddr getFirstSubAction(ScAddr const & decompositionTuple);

  void applyAction(ScAddr const & actionAddr);

  ScAddr getNextAction(ScAddr const & previousActionAddr);

  ScAddr getThenAction(ScAddr const & actionAddr);

  ScAddr getElseAction(ScAddr const & actionAddr);

  ScAddr getGoToAction(ScAddr const & actionAddr);
};

}  // namespace commonModule
