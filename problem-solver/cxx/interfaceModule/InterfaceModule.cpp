#include "InterfaceModule.hpp"

#include "agent/ChangeInterfaceColorAgent.hpp"

using namespace interfaceModule;

SC_MODULE_REGISTER(InterfaceModule)->Agent<ChangeInterfaceColorAgent>();
