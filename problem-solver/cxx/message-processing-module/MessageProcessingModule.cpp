#include "MessageProcessingModule.hpp"

#include "agent/FindWordInSetByFirstLetterAgent.hpp"

using namespace messageProcessingModule;

SC_MODULE_REGISTER(MessageProcessingModule)->Agent<FindWordInSetByFirstLetterAgent>();
