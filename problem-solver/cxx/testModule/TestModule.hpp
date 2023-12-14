#pragma once

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_module.hpp"

#include "keynodes/TestKeynodes.hpp"
#include "agent/TestAgent.hpp"
#include "agent/AuthorizationAgent.hpp"
#include "agent/RegistrationAgent.hpp"
#include "agent/TestingAgent.hpp"
#include "utils/ActionUtils.hpp"

#include "TestModule.generated.hpp"

namespace testModule
{
class TestModule : public ScModule
{
  SC_CLASS(LoadOrder(100))
  SC_GENERATED_BODY()

  virtual sc_result

  InitializeImpl() override;

  virtual sc_result ShutdownImpl() override;
};
 
}  // namespace interfaceModule
