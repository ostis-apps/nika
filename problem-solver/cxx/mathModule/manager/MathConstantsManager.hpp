#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

namespace mathModule
{

class MathConstantsManager
{
public:
  explicit MathConstantsManager(ScMemoryContext * context);

  ScAddrVector manage(ScAddrVector const & processParameters) const;

protected:
  ScMemoryContext * context;

  std::string getMathConstantName(ScAddr mathConstantAddr) const;

  std::string getMathConstantValue(const std::string & mathConstantName) const;

  ScAddrVector addTempLinkWithRelation(ScAddr addr, const std::string & linkContent, ScAddr relation) const;
};

}  // namespace mathModule
