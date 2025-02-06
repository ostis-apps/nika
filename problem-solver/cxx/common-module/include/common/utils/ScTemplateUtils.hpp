#pragma once

#include <sc-memory/sc_template.hpp>

class ScTemplateUtils
{
public:
  static ScAddrVector getAllWithKey(ScMemoryContext * context, const ScTemplate & scTemplate, const std::string & key);
};
