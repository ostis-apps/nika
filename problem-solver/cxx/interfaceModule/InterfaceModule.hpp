#pragma once

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_module.hpp"

#include "keynodes/InterfaceKeynodes.hpp"
#include "agent/InterfaceAgent.hpp"
#include "utils/ActionUtils.hpp"

#include "InterfaceModule.generated.hpp"

namespace interfaceModule
{
class InterfaceModule : public ScModule
{
  SC_CLASS(LoadOrder(100))
  SC_GENERATED_BODY()

  virtual sc_result

  InitializeImpl() override;

  virtual sc_result ShutdownImpl() override;
};
 
}  // namespace interfaceModule
