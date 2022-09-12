#include "LanguageSearcher.hpp"

#include "keynodes/Keynodes.hpp"
#include "searcher/TokenDomainSearcher.hpp"

using namespace commonModule;
using namespace dialogControlModule;
using namespace std;

LanguageSearcher::LanguageSearcher(ScMemoryContext * ms_context)
{
  this->context = ms_context;
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
  catch (runtime_error & ex)
  {
    SC_LOG_ERROR(ex.what());
  }

  return langNode;
}

ScAddr LanguageSearcher::getLanguage(const ScAddr & node)
{
  string lang = "_lang";

  ScAddr langNode;
  ScTemplate templateLang;
  ScTemplateSearchResult result;
  templateLang.Triple(ScType::NodeVarClass >> lang, ScType::EdgeAccessVarPosPerm, node);
  templateLang.Triple(Keynodes::languages, ScType::EdgeAccessVarPosPerm, lang);

  if (context->HelperSearchTemplate(templateLang, result))
    langNode = result[0][lang];

  return langNode;
}
