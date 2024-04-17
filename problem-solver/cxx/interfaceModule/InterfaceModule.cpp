#include "InterfaceModule.hpp"

using namespace interfaceModule;

SC_IMPLEMENT_MODULE(InterfaceModule)

sc_result InterfaceModule::InitializeImpl()
{
  if (!interfaceModule::InterfaceKeynodes::InitGlobal())
  {
    return SC_RESULT_ERROR;
  }

  ScMemoryContext ctx(sc_access_lvl_make_min, "InterfaceModule");
  if (ActionUtils::isActionDeactivated(&ctx, InterfaceKeynodes::action_change_interface))
  {
    SC_LOG_ERROR("action_change_interface is deactivated");
  }
  else
  {
    SC_AGENT_REGISTER(ChangeInterfaceColorAgent);
  }

  if (ActionUtils::isActionDeactivated(&ctx, InterfaceKeynodes::action_create_message_class_and_phrase_template))
  {
    SC_LOG_ERROR("action_create_message_class_and_phrase_template is deactivated");
  }
  else
  {
    SC_AGENT_REGISTER(CreateAnswerTemplateAgent);
  }

  if (ActionUtils::isActionDeactivated(&ctx, InterfaceKeynodes::action_create_class_instance))
  {
    SC_LOG_ERROR("action_create_class_instance");
  }
  else
  {
    SC_AGENT_REGISTER(CreateClassInstanceAgent);
  }
  
  if (ActionUtils::isActionDeactivated(&ctx, InterfaceKeynodes::action_create_class))
  {
    SC_LOG_ERROR("action_create_class");
  }
  else
  {
    SC_AGENT_REGISTER(CreateClassAgent);
  }

  if (ActionUtils::isActionDeactivated(&ctx, InterfaceKeynodes::action_create_relation))
  {
    SC_LOG_ERROR("action_create_relation");
  }
  else
  {
    SC_AGENT_REGISTER(CreateRelationAgent);
  }


  return SC_RESULT_OK;
}

sc_result InterfaceModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(ChangeInterfaceColorAgent);
  SC_AGENT_UNREGISTER(CreateAnswerTemplateAgent);
  SC_AGENT_UNREGISTER(CreateClassInstanceAgent);
  SC_AGENT_UNREGISTER(CreateClassAgent);
  SC_AGENT_UNREGISTER(CreateRelationAgent);


  return SC_RESULT_OK;
}
