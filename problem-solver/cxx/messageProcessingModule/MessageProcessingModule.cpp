#include "MessageProcessingModule.hpp"

#include "agent/FindWordInSetByFirstLetterAgent.hpp"

namespace messageProcessingModule
{
SC_MODULE_REGISTER(MessageProcessingModule)->Agent<FindWordInSetByFirstLetterAgent>();
}  // namespace messageProcessingModule
