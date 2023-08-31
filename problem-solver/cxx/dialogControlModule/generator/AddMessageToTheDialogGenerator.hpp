#pragma once

#include <memory>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

namespace dialogControlModule
{
class AddMessageToTheDialogGenerator
{
public:
  explicit AddMessageToTheDialogGenerator(ScMemoryContext * ms_context);

  void addMessageToDialog(ScAddr const & dialogAddr, ScAddr const & messageAddr);

  std::unique_ptr<ScTemplate> createNotFirstMessageInDialogTemplate(
      ScAddr const & dialogAddr,
      ScAddr const & lastMessageAddr,
      ScAddr const & messageAddr);

  static std::unique_ptr<ScTemplate> createFirstMessageInDialogTemplate(
      ScAddr const & dialogAddr,
      ScAddr const & messageAddr);

private:
  ScMemoryContext * context;
};

}  // namespace dialogControlModule
