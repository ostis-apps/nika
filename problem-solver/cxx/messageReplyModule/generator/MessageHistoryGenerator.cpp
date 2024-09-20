#include "MessageHistoryGenerator.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/MessageReplyKeynodes.hpp"
#include "templates/MessageReplyTemplates.hpp"

using namespace utils;
using namespace std;

namespace messageReplyModule
{
MessageHistoryGenerator::MessageHistoryGenerator(ScMemoryContext * context)
  : context(context)
{
}

void MessageHistoryGenerator::addMessageToDialog(const ScAddr & dialogAddr, const ScAddr & messageAddr)
{
  auto scTemplate = std::make_unique<ScTemplate>();
  ScAddr lastMessageAddr;

  ScIterator5Ptr iterator5Ptr = context->CreateIterator5(
      dialogAddr,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      MessageReplyKeynodes::rrel_last);

  if (iterator5Ptr->Next())
    lastMessageAddr = iterator5Ptr->Get(2);

  if (lastMessageAddr.IsValid())
  {
    ScIterator5Ptr it5 = context->CreateIterator5(
        dialogAddr,
        ScType::EdgeAccessConstPosPerm,
        lastMessageAddr,
        ScType::EdgeAccessConstPosPerm,
        MessageReplyKeynodes::rrel_last);

    if (it5->Next())
      context->EraseElement(it5->Get(3));

    scTemplate = createNotFirstMessageInDialogTemplate(dialogAddr, lastMessageAddr, messageAddr);
  }
  else
    scTemplate = createFirstMessageInDialogTemplate(dialogAddr, messageAddr);

  ScTemplateGenResult genResult;

  context->GenerateByTemplate(*scTemplate, genResult);
}

std::unique_ptr<ScTemplate> MessageHistoryGenerator::createNotFirstMessageInDialogTemplate(
    const ScAddr & dialogAddr,
    const ScAddr & lastMessageAddr,
    const ScAddr & messageAddr)
{
  string const NEXT_MESSAGE_ARC_ALIAS = "_next_message_arc";

  ScAddr messageEdge;
  ScIterator3Ptr iterator3Ptr = context->CreateIterator3(dialogAddr, ScType::EdgeAccessConstPosPerm, lastMessageAddr);
  if (iterator3Ptr->Next())
    messageEdge = iterator3Ptr->Get(1);

  auto scTemplate = std::make_unique<ScTemplate>();
  scTemplate->Quintuple(
      dialogAddr,
      ScType::EdgeAccessVarPosPerm >> NEXT_MESSAGE_ARC_ALIAS,
      messageAddr,
      ScType::EdgeAccessVarPosPerm,
      MessageReplyKeynodes::rrel_last);
  scTemplate->Quintuple(
      messageEdge,
      ScType::EdgeDCommonVar,
      NEXT_MESSAGE_ARC_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      MessageReplyKeynodes::nrel_message_sequence);

  return scTemplate;
}

std::unique_ptr<ScTemplate> MessageHistoryGenerator::createFirstMessageInDialogTemplate(
    const ScAddr & dialogAddr,
    const ScAddr & messageAddr)
{
  auto scTemplate = std::make_unique<ScTemplate>();
  scTemplate->Quintuple(
      dialogAddr, ScType::EdgeAccessVarPosPerm, messageAddr, ScType::EdgeAccessVarPosPerm, ScKeynodes::rrel_1);
  scTemplate->Quintuple(
      dialogAddr,
      ScType::EdgeAccessVarPosPerm,
      messageAddr,
      ScType::EdgeAccessVarPosPerm,
      MessageReplyKeynodes::rrel_last);
  return scTemplate;
}

}  // namespace messageReplyModule
