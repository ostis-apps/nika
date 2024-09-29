#include "NumberHandler.hpp"

#include "sc-agents-common/utils//IteratorUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include <sstream>

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
  SC_LOG_DEBUG("Found " << std::to_string(candidateList.size()) << " sc-link with content " << numberAsString);

  for (ScAddr candidateLink : candidateList)
  {
    ScAddr candidateNode =
        utils::IteratorUtils::getAnyByInRelation(this->context, candidateLink, ScKeynodes::nrel_idtf);
    if (this->context->CheckConnector(Keynodes::number, candidateNode, ScType::EdgeAccessConstPosPerm))
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
      ScType::NodeVar >> "_number_node",
      ScType::EdgeDCommonVar,
      numberLink,
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::nrel_idtf);
  scTemplate.Triple(Keynodes::file, ScType::EdgeAccessVarPosPerm, numberLink);
  scTemplate.Triple(Keynodes::number, ScType::EdgeAccessVarPosPerm, "_number_node");
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

commonModule::NumberHandler::NumberHandler(ScMemoryContext * ms_context)
{
  this->context = ms_context;
  this->linkHandler = new LinkHandler(ms_context);
}

commonModule::NumberHandler::~NumberHandler()
{
  delete this->linkHandler;
}
