#pragma once

#include <sc-memory/sc_memory.hpp>

#include "common/handler/NumberHandler.hpp"

namespace commonModule
{
class ParameterHandler
{
public:
  explicit ParameterHandler(ScMemoryContext * context);

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
  std::unique_ptr<NumberHandler> numberHandler;

  static std::string getClassNameForLog();
};

}  // namespace commonModule
