#include "common/handler/ParameterHandler.hpp"

#include "common/utils/RelationUtils.hpp"
#include "common/utils/ScTemplateUtils.hpp"

using namespace commonModule;

ScAddr ParameterHandler::updateMeasurableParameter(
    const ScAddr & entity,
    const ScAddr & parameterClass,
    const ScAddr & measurementRel,
    const double & number)
{
  ScAddr numberNode = this->numberHandler->getNumberNode(number);
  ScAddr parameterNode = getParameterNode(parameterClass, measurementRel, numberNode);
  if (!this->context->CheckConnector(parameterNode, entity, ScType::ConstPermPosArc))
  {
    ScAddr currentParameterNode = this->findParameterNodeByEntity(parameterClass, entity);
    if (currentParameterNode.IsValid())
    {
      RelationUtils::eraseAllEdges(this->context, currentParameterNode, entity, ScType::ConstPermPosArc);
    }
    this->context->GenerateConnector(ScType::ConstPermPosArc, parameterNode, entity);
  }
  return parameterNode;
}

ScAddr ParameterHandler::getParameterNode(
    const ScAddr & parameterClass,
    const ScAddr & measurementRel,
    const ScAddr & numberNode)
{
  ScAddr parameterNode = findParameterNodeByNumber(parameterClass, measurementRel, numberNode);
  if (!parameterNode.IsValid())
  {
    parameterNode = this->generateParameterNode(parameterClass, measurementRel, numberNode);
  }

  return parameterNode;
};

ScAddr ParameterHandler::findParameterNodeByNumber(
    const ScAddr & parameterClass,
    const ScAddr & measurementRel,
    const ScAddr & numberNode)
{
  ScAddr parameterNode = ScAddr();
  ScTemplate scTemplate;
  scTemplate.Triple(parameterClass, ScType::VarPermPosArc, ScType::VarNode >> "_parameter_node");
  scTemplate.Quintuple("_parameter_node", ScType::VarCommonArc, numberNode, ScType::VarPermPosArc, measurementRel);
  ScAddrVector parameterNodes = ScTemplateUtils::getAllWithKey(this->context, scTemplate, "_parameter_node");
  if (parameterNodes.size() == 1)
    parameterNode = parameterNodes[0];
  else if (parameterNodes.size() > 1)
  {
    SC_LOG_DEBUG(
        getClassNameForLog() + "More then 1 parameter node of " +
        this->context->GetElementSystemIdentifier(parameterClass) + " by number " +
        this->context->GetElementSystemIdentifier(numberNode));
  }
  else
  {
    SC_LOG_DEBUG(
        getClassNameForLog() + "Found no parameter node of " +
        this->context->GetElementSystemIdentifier(parameterClass) + " by number " +
        this->context->GetElementSystemIdentifier(numberNode));
  }

  return parameterNode;
};

ScAddr ParameterHandler::findParameterNodeByEntity(const ScAddr & parameterClass, const ScAddr & entity)
{
  ScAddr parameterNode = ScAddr();
  ScTemplate scTemplate;
  scTemplate.Triple(parameterClass, ScType::VarPermPosArc, ScType::VarNode >> "_parameter_node");
  scTemplate.Triple("_parameter_node", ScType::VarPermPosArc, entity);
  ScAddrVector parameterNodes = ScTemplateUtils::getAllWithKey(this->context, scTemplate, "_parameter_node");
  if (parameterNodes.size() == 1)
    parameterNode = parameterNodes[0];
  else if (parameterNodes.size() > 1)
  {
    SC_LOG_DEBUG(
        getClassNameForLog() + "More then 1 parameter node of " +
        this->context->GetElementSystemIdentifier(parameterClass) + " for entity " +
        this->context->GetElementSystemIdentifier(entity));
  }
  else
  {
    SC_LOG_DEBUG(
        getClassNameForLog() + "Found no parameter node of " +
        this->context->GetElementSystemIdentifier(parameterClass) + " for entity " +
        this->context->GetElementSystemIdentifier(entity));
  }

  return parameterNode;
};

ScAddr ParameterHandler::generateParameterNode(
    const ScAddr & parameterClass,
    const ScAddr & measurementRel,
    const ScAddr & numberNode)
{
  ScTemplate scTemplate;
  scTemplate.Triple(parameterClass, ScType::VarPermPosArc, ScType::VarNode >> "_parameter_node");
  scTemplate.Quintuple("_parameter_node", ScType::VarCommonArc, numberNode, ScType::VarPermPosArc, measurementRel);

  ScTemplateGenResult genResult;
  this->context->GenerateByTemplate(scTemplate, genResult);
  return genResult["_parameter_node"];
};

ParameterHandler::ParameterHandler(ScMemoryContext * context)
  : context(context)
  , numberHandler(std::make_unique<NumberHandler>(context))
{
}

std::string ParameterHandler::getClassNameForLog()
{
  static std::string const className = "ParameterHandler";
  return className;
}
