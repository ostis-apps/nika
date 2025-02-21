#include "MessageReplyModule.hpp"

#include "agent/MessageReplyAgent.hpp"

using namespace messageReplyModule;

SC_MODULE_REGISTER(MessageReplyModule)->Agent<MessageReplyAgent>();
