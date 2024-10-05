#include "generator/MessageConstructionsGenerator.hpp"
#include "searcher/LanguageSearcher.hpp"
#include "searcher/MessageSearcher.hpp"
#include "searcher/PhraseSearcher.hpp"

namespace dialogControlModule
{
class MessageHandler
{
public:
  explicit MessageHandler(ScAgentContext * context);

  bool processReplyMessage(
      const ScAddr & replyMessageNode,
      const ScAddr & logicRuleNode,
      const ScAddr & langNode,
      const ScAddr & parametersNode);

  bool processAtomicMessage(
      const ScAddr & messageNode,
      const ScAddr & phraseClassNode,
      const ScAddr & parametersNode,
      const ScAddr & langNode);

  bool processNonAtomicMessage(
      const ScAddr & replyMessageNode,
      const ScAddr & logicRuleNode,
      const ScAddr & parametersNode,
      const ScAddr & langNode);

private:
  const int PHRASE_GENERATION_AGENT_WAIT_TIME = 6000;

  ScAgentContext * context;
  std::unique_ptr<LanguageSearcher> languageSearcher;
  std::unique_ptr<MessageConstructionsGenerator> messageConstructionsGenerator;
  std::unique_ptr<MessageSearcher> messageSearcher;
  std::unique_ptr<PhraseSearcher> phraseSearcher;

  void clearSemanticAnswer();

  ScAddr generateLinkByPhrase(
      const ScAddr & replyMessageNode,
      const ScAddr & phraseClassNode,
      const ScAddr & parametersNode,
      const ScAddr & langNode);
};
}  // namespace dialogControlModule
