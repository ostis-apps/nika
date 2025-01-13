#pragma once

#include "sc-memory/sc_addr.hpp"

namespace messageClassificationModule
{
class MessageTopicClassificationManagerInterface
{
public:
//   explicit WitMessageTopicClassificationManager(ScAgentContext * context);

  virtual ScAddrVector manage(ScAddrVector const & processParameters) const = 0;
};

}  // namespace messageClassificationModule
