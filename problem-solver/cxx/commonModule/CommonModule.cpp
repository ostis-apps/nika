#include "CommonModule.hpp"

#include "agent/NonAtomicActionInterpreterAgent.hpp"

using namespace commonModule;

SC_MODULE_REGISTER(CommonModule)->Agent<NonAtomicActionInterpreterAgent>();
