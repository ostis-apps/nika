#pragma once

#include <sc-memory/sc_agent.hpp>

namespace nonAtomicActionInterpreterModule
{

class NonAtomicActionInterpreter
{
public:
  explicit NonAtomicActionInterpreter(ScAgentContext * context);

  void interpret(const ScAddr & nonAtomicActionAddr);

private:
  const int WAIT_TIME = 48000;

  ScAgentContext * context;

  ScAddr getFirstSubAction(ScAddr const & decompositionTuple);

  void applyAction(ScAddr const & actionAddr);

  ScAddr getNextAction(ScAddr const & previousActionAddr);

  ScAddr getThenAction(ScAddr const & actionAddr);

  ScAddr getElseAction(ScAddr const & actionAddr);

  ScAddr getGoToAction(ScAddr const & actionAddr);

  static std::string getClassNameForLog();
};

}  // namespace nonAtomicActionInterpreterModule
