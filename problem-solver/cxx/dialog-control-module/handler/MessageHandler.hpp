#include "generator/MessageConstructionsGenerator.hpp"
#include "searcher/LanguageSearcher.hpp"
#include "searcher/PhraseSearcher.hpp"
#include <common/searcher/MessageSearcher.hpp>

namespace dialogControlModule
{
class MessageHandler
{
public:
  explicit MessageHandler(ScAgentContext * context);

  bool processReplyMessage(
      ScAddr const & replyMessageNode,
      ScAddr const & logicRuleNode,
      ScAddr const & langNode,
      ScAddr const & parametersNode);

  bool processAtomicMessage(
      ScAddr const & messageNode,
      ScAddr const & phraseClassNode,
      ScAddr const & parametersNode,
      ScAddr const & langNode);

  bool processNonAtomicMessage(
      ScAddr const & replyMessageNode,
      ScAddr const & logicRuleNode,
      ScAddr const & parametersNode,
      ScAddr const & langNode);

private:
  const int PHRASE_GENERATION_AGENT_WAIT_TIME = 6000;

  ScAgentContext * context;
  std::unique_ptr<LanguageSearcher> languageSearcher;
  std::unique_ptr<MessageConstructionsGenerator> messageConstructionsGenerator;
  std::unique_ptr<commonModule::MessageSearcher> messageSearcher;
  std::unique_ptr<PhraseSearcher> phraseSearcher;

  void clearSemanticAnswer();

  ScAddr generateLinkByPhrase(
      ScAddr const & replyMessageNode,
      ScAddr const & phraseClassNode,
      ScAddr const & parametersNode,
      ScAddr const & langNode);

  static std::string getClassNameForLog();
};
}  // namespace dialogControlModule
