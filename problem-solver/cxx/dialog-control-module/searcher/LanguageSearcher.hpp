#include <sc-memory/sc_agent.hpp>

namespace dialogControlModule
{

class LanguageSearcher
{
public:
  explicit LanguageSearcher(ScMemoryContext * context, utils::ScLogger * logger);

  ScAddr getMessageLanguage(const ScAddr & messageNode);

  ScAddr getLanguage(const ScAddr & node);

private:
  ScMemoryContext * context;
  utils::ScLogger * logger;
};

}  // namespace dialogControlModule
