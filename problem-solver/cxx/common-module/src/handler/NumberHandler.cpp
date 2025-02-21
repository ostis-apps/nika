#include "common/handler/NumberHandler.hpp"

#include "common/keynodes/Keynodes.hpp"
#include <sc-agents-common/utils/IteratorUtils.hpp>

ScAddr commonModule::NumberHandler::getNumberNode(const double & number)
{
  ScAddr numberNode = findNumberNode(number);
  if (!numberNode.IsValid())
  {
    numberNode = this->generateNumberNode(number);
  }
  return numberNode;
}

ScAddr commonModule::NumberHandler::findNumberNode(const double & number)
{
  ScAddr numberNode = ScAddr();
  std::string numberAsString = this->numberToLikView(number);
  ScStreamPtr numberAsStream = ScStreamConverter::StreamFromString(numberAsString);

  ScAddrSet candidateList = this->context->SearchLinksByContent(numberAsStream);
  SC_LOG_DEBUG(
      "NumberHandler: Found " << std::to_string(candidateList.size()) << " sc-link with content " << numberAsString);

  for (ScAddr candidateLink : candidateList)
  {
    ScAddr candidateNode =
        utils::IteratorUtils::getAnyByInRelation(this->context, candidateLink, ScKeynodes::nrel_idtf);
    if (this->context->CheckConnector(Keynodes::number, candidateNode, ScType::ConstPermPosArc))
    {
      numberNode = candidateNode;
      break;
    }
  }
  return numberNode;
}

ScAddr commonModule::NumberHandler::generateNumberNode(const double & number)
{
  std::string numberAsString = this->numberToLikView(number);
  ScAddr numberLink = this->linkHandler->createLink(numberAsString);
  ScTemplate scTemplate;
  scTemplate.Quintuple(
      ScType::VarNode >> "_number_node",
      ScType::VarCommonArc,
      numberLink,
      ScType::VarPermPosArc,
      ScKeynodes::nrel_idtf);
  scTemplate.Triple(Keynodes::file, ScType::VarPermPosArc, numberLink);
  scTemplate.Triple(Keynodes::number, ScType::VarPermPosArc, "_number_node");
  ScTemplateGenResult genResult;

  this->context->GenerateByTemplate(scTemplate, genResult);
  return genResult["_number_node"];
}

std::string commonModule::NumberHandler::numberToLikView(const double & number)
{
  std::ostringstream stringStream;
  stringStream << number;
  return stringStream.str();
}

commonModule::NumberHandler::NumberHandler(ScMemoryContext * context)
  : context(context)
  , linkHandler(std::make_unique<LinkHandler>(context))
{
}
