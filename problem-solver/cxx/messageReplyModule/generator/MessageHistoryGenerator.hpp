#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

#include <memory>

namespace messageReplyModule
{
class MessageHistoryGenerator
{
public:
  explicit MessageHistoryGenerator(ScMemoryContext * ms_context);

  void addMessageToDialog(const ScAddr & dialogAddr, const ScAddr & messageAddr);

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

}  // namespace messageReplyModule
