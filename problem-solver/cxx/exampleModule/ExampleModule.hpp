#pragma once

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_module.hpp"

#include "keynodes/LetterKeynodes.hpp"
#include "agent/LetterAgent.hpp"
#include "utils/ActionUtils.hpp"

#include "ExampleModule.generated.hpp"


namespace exampleModule
{
class ExampleModule : public ScModule
{
  SC_CLASS(LoadOrder(100))
  SC_GENERATED_BODY()

  // Метод инициализации модуля и его агентов
  virtual sc_result InitializeImpl() override;

  // Метод деинициализации модуля и его агентов
  virtual sc_result ShutdownImpl() override;
};

} // namespace ExampleModule
