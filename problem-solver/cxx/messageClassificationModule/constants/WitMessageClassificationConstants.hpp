#pragma once

#include <string>

namespace messageClassificationModule
{
class WitAiConstants
{
public:
  static std::string const INTENTS;
  static std::string const TRAITS;
  static std::string const ENTITIES;
  static std::string const NAME;
  static std::string const VALUE;
  static std::string const BODY;
};

class MessageClassificationAliasConstants
{
public:
  static std::string const SET_OF_TRAITS_CLASS_ALIAS;
  static std::string const TRAIT_INCLUDED_CLASS_LINK_ALIAS;
  static std::string const SET_OF_TRAITS_CLASS_LINK_ALIAS;
  static std::string const ENTITY_ROLE_ALIAS;
  static std::string const ENTITY_SET_ALIAS;
  static std::string const ENTITY_ROLE_LINK_ALIAS;
  static std::string const ENTITY_CLASS_LINK_ALIAS;
};

}  // namespace messageClassificationModule
