
#include "CoordinatesManager.hpp"

#include "keynodes/Keynodes.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "http/sc_http_request.hpp"
#include <nlohmann/json.hpp>

namespace satelliteCoordinatesAgentsModule
{
CoordinatesManager::CoordinatesManager(ScMemoryContext * context) : context(context)
{
}

ScAddrVector CoordinatesManager::manageRemoval(ScAddrVector const & processParameters) const
{
  ScAddr satellite = processParameters.at(0);
  removeTempLinks(satellite);
  return {satellite};
}

ScAddrVector CoordinatesManager::manageAddition(ScAddrVector const & processParameters) const
{
  manageRemoval(processParameters);

  ScAddr satellite = processParameters.at(0);


  std::string apiUrl = "http://api.spectator.earth/satellite/";
  std::string satelliteId = getSatelliteId(satellite);
  ScHttpRequest request{apiUrl + satelliteId,{}};
  request.SetType(ScHttpRequest::Type::GET);
  ScHttpResponsePtr const response = request.Perform();
  const auto & jsonResponse = nlohmann::json::parse(response->GetData());
  SC_LOG_INFO("satellite response " + (jsonResponse.dump()));

  std::string coordinates = jsonResponse.at("geometry").at("coordinates").dump();
  //datetime = datetime.substr(0, datetime.find('.'));
  //auto delimiterIndex = datetime.find('T');
  //std::string date = datetime.substr(0, delimiterIndex);
  //std::string time = datetime.substr(delimiterIndex + 1);

  ScAddrVector answerElements;

  const auto & coordinatesElements = addTempLinkWithRelation(satellite, coordinates, Keynodes::nrel_coordinates);
  answerElements.insert(
      answerElements.cend(), coordinatesElements.cbegin(), coordinatesElements.cend());
  //const auto & timeElements = addTempLinkWithRelation(satellite, time, Keynodes::nrel_time);
  //answerElements.insert(
  //    answerElements.cend(), timeElements.cbegin(), timeElements.cend());

  return answerElements;
}

std::string CoordinatesManager::getSatelliteId(ScAddr satellite) const
{
  const auto & satelliteNameIterator =
      context->Iterator5(
          satellite,
          ScType::EdgeDCommonConst,
          ScType::LinkConst,
          ScType::EdgeAccessConstPosPerm,
          Keynodes::nrel_id);
  if (satelliteNameIterator->Next())
  {
    return utils::CommonUtils::getLinkContent(context, satelliteNameIterator->Get(2));
  }
  else
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "CoordinatesManager: satellite id not found for " + context->HelperGetSystemIdtf(satellite));
  }
}


ScAddrVector CoordinatesManager::addTempLinkWithRelation(ScAddr satellite, const std::string& linkContent, ScAddr relation) const
{
  const auto & link = context->CreateLink(ScType::LinkConst);
  context->SetLinkContent(link, ScStreamConverter::StreamFromString(linkContent));
  const auto & edgeDCommon = context->CreateEdge(ScType::EdgeDCommonConst, satellite, link);
  const auto & tempArc = context->CreateArc(ScType::EdgeAccessConstPosTemp, relation, edgeDCommon);
  return {link, relation, edgeDCommon, tempArc};

}

void CoordinatesManager::removeTempLinks(ScAddr satellite) const
{
  const auto & tempLinksIterator =
      context->Iterator5(
          satellite,
          ScType::EdgeDCommonConst,
          ScType::LinkConst,
          ScType::EdgeAccessConstPosTemp,
          ScType::NodeConst);
  while (tempLinksIterator->Next())
  {
    const ScAddr & linkAddr = tempLinksIterator->Get(2);
    context->EraseElement(linkAddr);
  }
}

}  // namespace satelliteCoordinatesAgentsModule