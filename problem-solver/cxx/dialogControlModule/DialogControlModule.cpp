#include "DialogControlModule.hpp"

using namespace dialogControlModule;

SC_MODULE_REGISTER(DialogControlModule)->Agent<PhraseGenerationAgent>()->Agent<StandardMessageReplyAgent>();
