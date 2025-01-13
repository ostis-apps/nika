#include "MessageClassificationModule.hpp"

#include "agent/AlternativeMessageTopicClassificationAgent.hpp"
#include "agent/WitMessageTopicClassificationAgent.hpp"
#include "agent/RasaMessageTopicClassificationAgent.hpp"

using namespace messageClassificationModule;

SC_MODULE_REGISTER(MessageClassificationModule)
    ->Agent<WitMessageTopicClassificationAgent>()
    ->Agent<AlternativeMessageTopicClassificationAgent>()
    ->Agent<RasaMessageTopicClassificationAgent>();
