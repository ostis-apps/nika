#pragma once

#include <memory>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

namespace dialogControlModule
{
class MessageHistoryGenerator
{
public:
  explicit MessageHistoryGenerator(ScMemoryContext * ms_context);

  void addMessageToSuggestedReplies(const ScAddr & messageAddr, const ScAddr & replyMessageAddr);

  void addMessageToDialog(const ScAddr & dialogAddr, const ScAddr & messageAddr, const ScAddr & authorsAddr);

  std::unique_ptr<ScTemplate> createNotFirstMessageInDialogTemplate(
      const ScAddr & dialogAddr,
      const ScAddr & lastMessageAddr,
      const ScAddr & messageAddr);

  static std::unique_ptr<ScTemplate> createFirstMessageInDialogTemplate(
      const ScAddr & dialogAddr,
      const ScAddr & messageAddr);

private:
  ScMemoryContext * context;
};

}  // namespace dialogControlModule
