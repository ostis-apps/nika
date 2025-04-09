#include "LanguageSearcher.hpp"

#include "searcher/TokenDomainSearcher.hpp"
#include <common/keynodes/Keynodes.hpp>

using namespace commonModule;
using namespace dialogControlModule;

LanguageSearcher::LanguageSearcher(ScMemoryContext * context)
  : context(context)
{
}

ScAddr LanguageSearcher::getMessageLanguage(const ScAddr & messageNode)
{
  TokenDomainSearcher searcher(context);
  ScAddr langNode;

  try
  {
    ScAddr link = searcher.getMessageText(messageNode);
    if (link.IsValid())
      langNode = getLanguage(link);
  }
  catch (std::runtime_error & ex)
  {
    SC_LOG_ERROR(ex.what());
  }

  return langNode;
}

ScAddr LanguageSearcher::getLanguage(const ScAddr & node)
{
  std::string lang = "_lang";

  ScAddr langNode;
  ScTemplate templateLang;
  ScTemplateSearchResult result;
  templateLang.Triple(ScType::VarNodeClass >> lang, ScType::VarPermPosArc, node);
  templateLang.Triple(Keynodes::languages, ScType::VarPermPosArc, lang);

  if (context->SearchByTemplate(templateLang, result))
    langNode = result[0][lang];

  return langNode;
}
