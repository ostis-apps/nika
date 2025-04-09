#pragma once

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_template.hpp>

#include <string>

namespace messageReplyModule
{
class MessageReplyTemplates
{
public:
  static std::string const REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS;
  static std::string const REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS;
  static std::string const REPLY_MESSAGE_ALIAS;
  static std::string const ANSWER_ALIAS;
  static std::string const SUGGESTION_REPLIES_PAIR_ARC;
  static std::string const SUGGESTED_REPLIES_TUPLE_ALIAS;
  static std::string const SUGGESTION_REPLIES_ACCESS_ARC;
  static std::string const SUGGESTED_REPLY;
  static std::string const NEXT_MESSAGE_ARC_ALIAS;
};
}  // namespace messageReplyModule
