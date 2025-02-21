#include "common/searcher/MessageSearcher.hpp"

#include "common/keynodes/Keynodes.hpp"
#include <algorithm>
#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace commonModule;

MessageSearcher::MessageSearcher(ScMemoryContext * context)
  : context(context)
{
}

ScAddr MessageSearcher::getFirstMessage(const ScAddr & nonAtomicMessageNode)
{
  const std::string VAR_TUPLE = "_tuple";
  const std::string VAR_MESSAGE = "_message";
  ScTemplate templ;
  templ.Quintuple(
      ScType::VarNodeTuple >> VAR_TUPLE,
      ScType::VarCommonArc,
      nonAtomicMessageNode,
      ScType::VarPermPosArc,
      Keynodes::nrel_message_decomposition);
  templ.Quintuple(
      VAR_TUPLE, ScType::VarPermPosArc, ScType::VarNode >> VAR_MESSAGE, ScType::VarPermPosArc, ScKeynodes::rrel_1);

  ScTemplateSearchResult result;
  context->SearchByTemplate(templ, result);

  ScAddr resultMessageNode;
  if (result.Size() == 1)
  {
    resultMessageNode = result[0][VAR_MESSAGE];
    SC_LOG_DEBUG("MessageSearcher: The first message node found");
  }
  else
  {
    SC_LOG_DEBUG("MessageSearcher: The first message node not found");
  }

  return resultMessageNode;
}

ScAddr MessageSearcher::getNextMessage(const ScAddr & messageNode)
{
  const std::string VAR_TUPLE = "_tuple", VAR_EDGE_1 = "_edge_1", VAR_EDGE_2 = "_edge_2",
                    VAR_D_COMMON_EDGE = "_d_common_edge", VAR_MESSAGE = "_message";
  ScTemplate templ;
  templ.Triple(ScType::VarNodeTuple >> VAR_TUPLE, ScType::VarPermPosArc >> VAR_EDGE_1, messageNode);
  templ.Quintuple(
      VAR_EDGE_1,
      ScType::VarCommonArc >> VAR_D_COMMON_EDGE,
      ScType::VarPermPosArc >> VAR_EDGE_2,
      ScType::VarPermPosArc,
      Keynodes::nrel_message_sequence);
  templ.Triple(VAR_TUPLE, VAR_EDGE_2, ScType::VarNode >> VAR_MESSAGE);

  ScTemplateSearchResult result;
  context->SearchByTemplate(templ, result);

  ScAddr resultMessageNode;
  if (result.Size() > 0)
  {
    resultMessageNode = result[0][VAR_MESSAGE];
    SC_LOG_DEBUG("MessageSearcher: Next message node found");
  }
  else
  {
    SC_LOG_DEBUG("MessageSearcher: Next message node not found");
  }

  return resultMessageNode;
}

ScAddr MessageSearcher::getMessageAuthor(const ScAddr & messageNode)
{
  ScTemplate templ;
  const std::string VAR_AUTHOR = "_author";
  templ.Quintuple(
      messageNode, ScType::VarCommonArc, ScType::VarNode >> VAR_AUTHOR, ScType::VarPermPosArc, Keynodes::nrel_authors);

  ScTemplateSearchResult result;
  context->SearchByTemplate(templ, result);

  ScAddr resultAuthorNode;
  if (result.Size() > 0)
  {
    resultAuthorNode = result[0][VAR_AUTHOR];
    SC_LOG_DEBUG("MessageSearcher: Author set node found");
  }
  else
  {
    SC_LOG_DEBUG("MessageSearcher: Author set node not found");
  }

  return resultAuthorNode;
}

ScAddr MessageSearcher::getMessageTheme(const ScAddr & messageNode)
{
  ScTemplate templ;
  const std::string VAR_THEME = "_theme";
  templ.Quintuple(
      messageNode,
      ScType::VarPermPosArc,
      ScType::VarNode >> VAR_THEME,
      ScType::VarPermPosArc,
      Keynodes::rrel_message_theme);

  ScTemplateSearchResult result;
  context->SearchByTemplate(templ, result);

  ScAddr resultThemeNode;
  if (result.Size() > 0)
  {
    resultThemeNode = result[0][VAR_THEME];
    SC_LOG_DEBUG("MessageSearcher: Message theme node found");
  }
  else
  {
    SC_LOG_DEBUG("MessageSearcher: Message theme node not found");
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
    SC_LOG_WARNING("MessageSearcher: Text translation node not found");
    return {};
  }

  ScIterator3Ptr const linkIterator =
      context->CreateIterator3(translationNode, ScType::ConstPermPosArc, ScType::ConstNodeLink);
  while (linkIterator->Next())
  {
    ScAddr const & linkAddr = linkIterator->Get(2);
    bool result = std::all_of(linkClasses.cbegin(), linkClasses.cend(), [this, &linkAddr](auto const & addr) {
      return context->CheckConnector(addr, linkAddr, ScType::ConstPermPosArc);
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
