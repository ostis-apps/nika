#include "PhraseSearcher.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/MessageKeynodes.hpp"
#include <algorithm>
#include <chrono>
#include <random>

using namespace dialogControlModule;

PhraseSearcher::PhraseSearcher(ScMemoryContext * ms_context)
{
  this->context = ms_context;
}

ScAddrVector PhraseSearcher::getPhrases(const ScAddr & phraseClassNode, const ScAddr & langNode)
{
  ScTemplate phraseTemplate;
  const std::string VAR_PHRASE = "_phrase";
  phraseTemplate.Triple(phraseClassNode, ScType::EdgeAccessVarPosPerm, ScType::Link >> VAR_PHRASE);
  phraseTemplate.Triple(langNode, ScType::EdgeAccessVarPosPerm, VAR_PHRASE);

  ScTemplateSearchResult result;
  context->HelperSearchTemplate(phraseTemplate, result);

  size_t resultSize = result.Size();
  ScAddrVector phrasesLinks;
  if (resultSize != 0)
  {
    for (size_t index = 0; index < resultSize; index++)
      phrasesLinks.push_back(result[index][VAR_PHRASE]);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(phrasesLinks.begin(), phrasesLinks.end(), std::default_random_engine(seed));
  }

  return phrasesLinks;
}

ScAddr PhraseSearcher::getFirstPhraseClass(const ScAddr & logicRuleNode)
{
  ScAddr firstPhraseNode;
  ScAddr const & answerPatternTuple = utils::IteratorUtils::getAnyByOutRelation(
        context, logicRuleNode, MessageKeynodes::nrel_answer_pattern);
  if (answerPatternTuple.IsValid())
  {
    firstPhraseNode = utils::IteratorUtils::getAnyByOutRelation(
          context, answerPatternTuple, scAgentsCommon::CoreKeynodes::rrel_1);
  }

  return firstPhraseNode;
}

ScAddr PhraseSearcher::getNextPhraseClass(const ScAddr & phraseClassNode)
{
  const std::string VAR_TUPLE = "_tuple", VAR_EDGE_1 = "_edge_1", VAR_EDGE_2 = "_edge_2",
                    VAR_D_COMMON_EDGE = "_d_common_edge", VAR_PHRASE_CLASS = "_phrase_class";

  ScTemplate templ;
  templ.Triple(ScType::NodeVarTuple >> VAR_TUPLE, ScType::EdgeAccessVarPosPerm >> VAR_EDGE_1, phraseClassNode);
  templ.Triple(VAR_EDGE_1, ScType::EdgeDCommonVar >> VAR_D_COMMON_EDGE, ScType::EdgeAccessVarPosPerm >> VAR_EDGE_2);
  templ.Triple(scAgentsCommon::CoreKeynodes::nrel_basic_sequence, ScType::EdgeAccessVarPosPerm, VAR_D_COMMON_EDGE);
  templ.Triple(VAR_TUPLE, VAR_EDGE_2, ScType::NodeVar >> VAR_PHRASE_CLASS);

  ScTemplateSearchResult result;
  context->HelperSearchTemplate(templ, result);

  ScAddr nextPhraseNode;
  if (result.Size() == 1)
  {
    nextPhraseNode = result[0][VAR_PHRASE_CLASS];
    SC_LOG_DEBUG("MessageSearcher: the next phrase class found");
  }
  else
  {
    SC_LOG_DEBUG("MessageSearcher: the next phrase class not found");
  }

  return nextPhraseNode;
}
