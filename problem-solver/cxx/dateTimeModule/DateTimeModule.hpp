#pragma once

#include "sc-memory/sc_module.hpp"

#include "DateTimeModule.generated.hpp"

namespace dateTimeAgentsModule
{

class DateTimeModule : public ScModule
{
  SC_CLASS(LoadOrder(100))
  SC_GENERATED_BODY()

  virtual sc_result InitializeImpl() override;

  virtual sc_result ShutdownImpl() override;
};

} // namespace dateTimeAgentsModule
