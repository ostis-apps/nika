#pragma once

#include <sc-memory/sc_memory.hpp>

namespace commonModule
{
class MessageSearcher
{
public:
  explicit MessageSearcher(ScMemoryContext * context);

  ScAddr getFirstMessage(const ScAddr & nonAtomicMessageNode);

  ScAddr getNextMessage(const ScAddr & messageNode);

  ScAddr getMessageAuthor(const ScAddr & messageNode);

  ScAddr getMessageTheme(const ScAddr & messageNode);

  ScAddrVector getMessageLinks(ScAddr const & message, ScAddrVector const & linkClasses = {});

  ScAddr getMessageLink(ScAddr const & message, ScAddrVector const & linkClasses = {});

private:
  ScMemoryContext * context;
};
}  // namespace commonModule
