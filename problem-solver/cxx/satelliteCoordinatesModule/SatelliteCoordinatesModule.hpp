#pragma once

#include "sc-memory/sc_module.hpp"

#include "SatelliteCoordinatesModule.generated.hpp"

namespace satelliteCoordinatesAgentsModule
{

class SatelliteCoordinatesModule : public ScModule
{
  SC_CLASS(LoadOrder(100))
  SC_GENERATED_BODY()

  // метод инициализации модуля и его агентов
  virtual sc_result InitializeImpl() override;

  // метод деинициализации модуля и его агентов
  virtual sc_result ShutdownImpl() override;
};

} // namespace satelliteCoordinatesAgentsModule
