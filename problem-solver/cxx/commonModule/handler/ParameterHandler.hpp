#pragma once

#include "sc-memory/sc_memory.hpp"

#include "handler/NumberHandler.hpp"

namespace commonModule
{
class ParameterHandler
{
public:
  explicit ParameterHandler(ScMemoryContext * ms_context);

  ~ParameterHandler();

  ScAddr updateMeasurableParameter(
      const ScAddr & entity,
      const ScAddr & parameterClass,
      const ScAddr & measurementRel,
      const double & number);

  ScAddr getParameterNode(const ScAddr & parameterClass, const ScAddr & measurementRel, const ScAddr & numberNode);

  ScAddr findParameterNodeByNumber(
      const ScAddr & parameterClass,
      const ScAddr & measurementRel,
      const ScAddr & numberNode);

  ScAddr findParameterNodeByEntity(const ScAddr & parameterClass, const ScAddr & entity);

  ScAddr generateParameterNode(const ScAddr & parameterClass, const ScAddr & measurementRel, const ScAddr & numberNode);

private:
  ScMemoryContext * context;
  NumberHandler * numberHandler;
};

}  // namespace commonModule
