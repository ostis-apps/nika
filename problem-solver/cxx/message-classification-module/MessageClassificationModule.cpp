#include "MessageClassificationModule.hpp"

#include "agent/AlternativeMessageTopicClassificationAgent.hpp"
#include "agent/MessageTopicClassificationAgent.hpp"

using namespace messageClassificationModule;

SC_MODULE_REGISTER(MessageClassificationModule)
    ->Agent<MessageTopicClassificationAgent>()
    ->Agent<AlternativeMessageTopicClassificationAgent>();
