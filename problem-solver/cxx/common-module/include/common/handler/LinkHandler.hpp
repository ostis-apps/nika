#include <sc-memory/sc_memory.hpp>

#include <string>

namespace commonModule
{
class LinkHandler
{
public:
  explicit LinkHandler(ScMemoryContext * context);

  ScAddr createLink(const std::string & text);

private:
  ScMemoryContext * context;
};
}  // namespace commonModule
