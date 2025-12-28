#pragma once

#include <string>

namespace messageClassificationModule
{
class RasaConstants
{
public:
  static std::string const INTENT;
  static std::string const ENTITIES;
  static std::string const VALUE;
  static std::string const NAME;
  static std::string const ENTITY_ROLE;
  static std::string const UNKNOWN_INTENT;
};

class RasaMessageClassificationAliasConstants
{
public:;
  static std::string const ENTITY_ROLE_ALIAS;
  static std::string const ENTITY_SET_ALIAS;
  static std::string const ENTITY_ROLE_LINK_ALIAS;
  static std::string const ENTITY_CLASS_LINK_ALIAS;
};

}  // namespace messageClassificationModule
