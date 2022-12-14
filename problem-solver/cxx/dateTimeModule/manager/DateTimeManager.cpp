
#include "DateTimeManager.hpp"

#include "keynodes/Keynodes.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "http/sc_http_request.hpp"
#include <nlohmann/json.hpp>

namespace dateTimeAgentsModule
{
DateTimeManager::DateTimeManager(ScMemoryContext * context) : context(context)
{
}

ScAddrVector DateTimeManager::manageRemoval(ScAddrVector const & processParameters) const
{
  ScAddr timezone = processParameters.at(0);
  removeTempLinks(timezone);
  return {timezone};
}

ScAddrVector DateTimeManager::manageAddition(ScAddrVector const & processParameters) const
{
  manageRemoval(processParameters);

  ScAddr timezone = processParameters.at(0);

  const auto & jsonResponse = getDatetime(timezone);
  SC_LOG_INFO("timezone response " + (jsonResponse.dump(2)));

  std::string datetime = jsonResponse.at("datetime");
  datetime = datetime.substr(0, datetime.find('.'));
  auto delimiterIndex = datetime.find('T');
  std::string date = datetime.substr(0, delimiterIndex);
  std::string time = datetime.substr(delimiterIndex + 1);

  ScAddrVector answerElements;

  const auto & dateElements = addTempLinkWithRelation(timezone, date, Keynodes::nrel_date);
  answerElements.insert(
      answerElements.cend(), dateElements.cbegin(), dateElements.cend());
  const auto & timeElements = addTempLinkWithRelation(timezone, time, Keynodes::nrel_time);
  answerElements.insert(
      answerElements.cend(), timeElements.cbegin(), timeElements.cend());

  return answerElements;
}

nlohmann::json DateTimeManager::getDatetime(ScAddr & timezone) const
{
  try
  {
    string apiUrl = "http://worldtimeapi.org/api/timezone/";
    string timezoneName = getTimezoneName(timezone);
    ScHttpRequest request{apiUrl + timezoneName,{}};
    request.SetType(ScHttpRequest::Type::GET);
    ScHttpResponsePtr const response = request.Perform();
    return nlohmann::json::parse(response->GetData());
  }
  catch (nlohmann::detail::parse_error & parseError)
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidState, "DateTimeManager: Cannot connect to 'worldtimeapi.org'");
  }
}

std::string DateTimeManager::getTimezoneName(ScAddr timezone) const
{
  const auto & timezoneNameIterator =
      context->Iterator5(
          timezone,
          ScType::EdgeDCommonConst,
          ScType::LinkConst,
          ScType::EdgeAccessConstPosPerm,
          Keynodes::nrel_timezone_name);
  if (timezoneNameIterator->Next())
    return utils::CommonUtils::getLinkContent(context, timezoneNameIterator->Get(2));
  else
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound,
                       "DateTimeManager: Timezone name not found for " + context->HelperGetSystemIdtf(timezone));
}


ScAddrVector DateTimeManager::addTempLinkWithRelation(ScAddr timezone, const std::string& linkContent, ScAddr relation) const
{
  const auto & link = context->CreateLink(ScType::LinkConst);
  context->SetLinkContent(link, ScStreamConverter::StreamFromString(linkContent));
  const auto & edgeDCommon = context->CreateEdge(ScType::EdgeDCommonConst, timezone, link);
  const auto & tempArc = context->CreateArc(ScType::EdgeAccessConstPosTemp, relation, edgeDCommon);
  return {link, relation, edgeDCommon, tempArc};

}

void DateTimeManager::removeTempLinks(ScAddr timezone) const
{
  const auto & tempLinksIterator =
      context->Iterator5(
          timezone,
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

}  // namespace dateTimeAgentsModule
