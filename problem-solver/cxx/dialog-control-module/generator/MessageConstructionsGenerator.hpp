#include <sc-memory/sc_memory.hpp>

namespace dialogControlModule
{
class MessageConstructionsGenerator
{
private:
  ScMemoryContext * context;

public:
  explicit MessageConstructionsGenerator(ScMemoryContext * context);

  void generateTextTranslationConstruction(
      const ScAddr & messageAddr,
      const ScAddr & langAddr,
      const std::string & messageText);

  void generateTextTranslationConstruction(const ScAddr & messageAddr, const ScAddr & linkAddr);
};
}  // namespace dialogControlModule
