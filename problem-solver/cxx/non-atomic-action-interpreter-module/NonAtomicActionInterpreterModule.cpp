#include "NonAtomicActionInterpreterModule.hpp"

#include "agent/NonAtomicActionInterpreterAgent.hpp"

using namespace nonAtomicActionInterpreterModule;

SC_MODULE_REGISTER(NonAtomicActionInterpreterModule)->Agent<NonAtomicActionInterpreterAgent>();
