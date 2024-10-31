#include "DialogControlModule.hpp"

#include "agent/PhraseGenerationAgent.hpp"
#include "agent/StandardMessageReplyAgent.hpp"

using namespace dialogControlModule;

SC_MODULE_REGISTER(DialogControlModule)->Agent<PhraseGenerationAgent>()->Agent<StandardMessageReplyAgent>();
