#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "AddSatelliteCoordinatesAgent.generated.hpp"

namespace satelliteCoordinatesAgentsModule
{

class AddSatelliteCoordinatesAgent : public ScAgent
{
  // условие инициирования агента (появление в sc-памяти
  // выходящей дуги из `question_initiated` к экземпляру
  // агента
  SC_CLASS(Agent, Event(
                      scAgentsCommon::CoreKeynodes::question_initiated,
                      ScEvent::Type::AddOutputEdge)
  )

  // описывает логику работы агента
  SC_GENERATED_BODY();


private:
  // метод, описывающий логику нахождения координат спутника
  ScAddrVector setCoordinates(ScAddr const & timezone);

  // вспомогательный метод проверки принадлежности экземпляра
  // агента к его классу
  bool checkActionClass(ScAddr const & actionAddr);

};

}  // namespace satelliteCoordinatesAgentsModule

