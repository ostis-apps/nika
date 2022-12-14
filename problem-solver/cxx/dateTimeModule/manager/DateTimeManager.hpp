#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

#include <nlohmann/json.hpp>

namespace dateTimeAgentsModule
{

class DateTimeManager
{
public:
  explicit DateTimeManager(ScMemoryContext * context);

  ScAddrVector manageAddition(ScAddrVector const & processParameters) const;
  ScAddrVector manageRemoval(ScAddrVector const & processParameters) const;

protected:
  ScMemoryContext * context;

  std::string getTimezoneName(ScAddr timezone) const;
  ScAddrVector addTempLinkWithRelation(ScAddr timezone, const std::string& linkContent, ScAddr relation) const;
  void removeTempLinks(ScAddr timezone) const;
  nlohmann::json getDatetime(ScAddr & timezone) const;
};

}  // namespace dateTimeAgentsModule

