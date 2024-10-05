#include "MessageSearcher.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "keynodes/MessageKeynodes.hpp"
#include <algorithm>

using namespace dialogControlModule;

MessageSearcher::MessageSearcher(ScMemoryContext * context) : context(context)
{
}

ScAddr MessageSearcher::getFirstMessage(const ScAddr & nonAtomicMessageNode)
{
  const std::string VAR_TUPLE = "_tuple";
  const std::string VAR_MESSAGE = "_message";
  ScTemplate templ;
  templ.Quintuple(
      ScType::NodeVarTuple >> VAR_TUPLE,
      ScType::EdgeDCommonVar,
      nonAtomicMessageNode,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::nrel_message_decomposition);
  templ.Quintuple(
      VAR_TUPLE,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> VAR_MESSAGE,
      ScType::EdgeAccessVarPosPerm,
      ScKeynodes::rrel_1);

  ScTemplateSearchResult result;
  context->SearchByTemplate(templ, result);

  ScAddr resultMessageNode;
  if (result.Size() == 1)
  {
    resultMessageNode = result[0][VAR_MESSAGE];
    SC_LOG_DEBUG("The first message node found");
  }
  else
  {
    SC_LOG_DEBUG("The first message node not found");
  }

  return resultMessageNode;
}

ScAddr MessageSearcher::getNextMessage(const ScAddr & messageNode)
{
  const std::string VAR_TUPLE = "_tuple", VAR_EDGE_1 = "_edge_1", VAR_EDGE_2 = "_edge_2",
                    VAR_D_COMMON_EDGE = "_d_common_edge", VAR_MESSAGE = "_message";
  ScTemplate templ;
  templ.Triple(ScType::NodeVarTuple >> VAR_TUPLE, ScType::EdgeAccessVarPosPerm >> VAR_EDGE_1, messageNode);
  templ.Quintuple(
      VAR_EDGE_1,
      ScType::EdgeDCommonVar >> VAR_D_COMMON_EDGE,
      ScType::EdgeAccessVarPosPerm >> VAR_EDGE_2,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::nrel_message_sequence);
  templ.Triple(VAR_TUPLE, VAR_EDGE_2, ScType::NodeVar >> VAR_MESSAGE);

  ScTemplateSearchResult result;
  context->SearchByTemplate(templ, result);

  ScAddr resultMessageNode;
  if (result.Size() > 0)
  {
    resultMessageNode = result[0][VAR_MESSAGE];
    SC_LOG_DEBUG("Next message node found");
  }
  else
  {
    SC_LOG_DEBUG("Next message node not found");
  }

  return resultMessageNode;
}

ScAddr MessageSearcher::getMessageAuthor(const ScAddr & messageNode)
{
  ScTemplate templ;
  const std::string VAR_AUTHOR = "_author";
  templ.Quintuple(
      messageNode,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> VAR_AUTHOR,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::nrel_authors);

  ScTemplateSearchResult result;
  context->SearchByTemplate(templ, result);

  ScAddr resultAuthorNode;
  if (result.Size() > 0)
  {
    resultAuthorNode = result[0][VAR_AUTHOR];
    SC_LOG_DEBUG("Author set node found");
  }
  else
  {
    SC_LOG_DEBUG("Author set node not found");
  }

  return resultAuthorNode;
}

ScAddr MessageSearcher::getMessageTheme(const ScAddr & messageNode)
{
  ScTemplate templ;
  const std::string VAR_THEME = "_theme";
  templ.Quintuple(
      messageNode,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> VAR_THEME,
      ScType::EdgeAccessVarPosPerm,
      MessageKeynodes::rrel_message_theme);

  ScTemplateSearchResult result;
  context->SearchByTemplate(templ, result);

  ScAddr resultThemeNode;
  if (result.Size() > 0)
  {
    resultThemeNode = result[0][VAR_THEME];
    SC_LOG_DEBUG("Message theme node found");
  }
  else
  {
    SC_LOG_DEBUG("Message theme node not found");
  }

  return resultThemeNode;
}

ScAddrVector MessageSearcher::getMessageLinks(ScAddr const & message, ScAddrVector const & linkClasses)
{
  ScAddrVector messageLinks;
  ScAddr const translationNode =
      utils::IteratorUtils::getAnyByInRelation(context, message, commonModule::Keynodes::nrel_sc_text_translation);
  if (!translationNode.IsValid())
  {
    SC_LOG_WARNING("Text translation node not found");
    return {};
  }

  ScIterator3Ptr const linkIterator =
      context->CreateIterator3(translationNode, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);
  while (linkIterator->Next())
  {
    ScAddr const & linkAddr = linkIterator->Get(2);
    bool result = std::all_of(linkClasses.cbegin(), linkClasses.cend(), [this, &linkAddr](auto const & addr) {
      return context->CheckConnector(addr, linkAddr, ScType::EdgeAccessConstPosPerm);
    });

    if (result == SC_TRUE)
    {
      messageLinks.push_back(linkAddr);
    }
  }
  return messageLinks;
}

ScAddr MessageSearcher::getMessageLink(ScAddr const & message, ScAddrVector const & linkClasses)
{
  ScAddr messageLink;
  ScAddrVector messageLinks = getMessageLinks(message, linkClasses);
  if (!messageLinks.empty())
    messageLink = messageLinks.at(0);
  return messageLink;
}
