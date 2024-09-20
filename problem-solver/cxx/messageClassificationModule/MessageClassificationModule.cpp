#include "MessageClassificationModule.hpp"

#include "sc-memory/sc_memory.hpp"

#include "agent/AlternativeMessageTopicClassificationAgent.hpp"
#include "agent/MessageTopicClassificationAgent.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace messageClassificationModule;

SC_MODULE_REGISTER(MessageClassificationModule)
    ->Agent<MessageTopicClassificationAgent>()
    ->Agent<AlternativeMessageTopicClassificationAgent>();
