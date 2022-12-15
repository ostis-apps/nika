#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

namespace satelliteCoordinatesAgentsModule
{

class CoordinatesManager
{
public:
  explicit CoordinatesManager(ScMemoryContext * context);

  ScAddrVector manageAddition(ScAddrVector const & processParameters) const;
  ScAddrVector manageRemoval(ScAddrVector const & processParameters) const;

protected:
  ScMemoryContext * context;

  std::string getSatelliteId(ScAddr satellite) const;
  ScAddrVector addTempLinkWithRelation(ScAddr satellite, const std::string& linkContent, ScAddr relation) const;
  void removeTempLinks(ScAddr satellite) const;
};

}  // namespace satelliteCoordinatesAgentsModule

