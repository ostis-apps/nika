#include <sc-memory/sc_memory.hpp>

#include <vector>

namespace dialogControlModule
{
class PhraseSearcher
{
public:
  explicit PhraseSearcher(ScMemoryContext * context);

  std::vector<ScAddr> getPhrases(const ScAddr & phraseClassNode, const ScAddr & langNode);

  ScAddr getFirstPhraseClass(const ScAddr & logicRuleNode);

  ScAddr getNextPhraseClass(const ScAddr & phraseClassNode);

private:
  ScMemoryContext * context;
};
}  // namespace dialogControlModule
