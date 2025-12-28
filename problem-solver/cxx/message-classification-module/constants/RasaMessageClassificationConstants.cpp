#include "RasaMessageClassificationConstants.hpp"

namespace messageClassificationModule
{
std::string const RasaConstants::INTENT = "intent";
std::string const RasaConstants::ENTITIES = "entities";
std::string const RasaConstants::VALUE = "value";
std::string const RasaConstants::NAME = "name";
std::string const RasaConstants::ENTITY_ROLE = "entity";
std::string const RasaConstants::UNKNOWN_INTENT = "unknown_intent";

std::string const RasaMessageClassificationAliasConstants::ENTITY_ROLE_ALIAS = "_entity_role";
std::string const RasaMessageClassificationAliasConstants::ENTITY_SET_ALIAS = "_entity_set";
std::string const RasaMessageClassificationAliasConstants::ENTITY_CLASS_LINK_ALIAS = "_entity_class_link";
std::string const RasaMessageClassificationAliasConstants::ENTITY_ROLE_LINK_ALIAS = "_entity_role_link";

}  // namespace messageClassificationModule
