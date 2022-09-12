#pragma once

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_module.hpp"
#include "keynodes/CoreKeynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include "agent/PhraseGenerationAgent.hpp"
#include "agent/StandardMessageReplyAgent.hpp"
#include "utils/ActionUtils.hpp"

#include "DialogControlModule.generated.hpp"

class DialogControlModule : public ScModule
{
  SC_CLASS(LoadOrder(100))
  SC_GENERATED_BODY()

  virtual sc_result InitializeImpl() override;

  virtual sc_result ShutdownImpl() override;
};
