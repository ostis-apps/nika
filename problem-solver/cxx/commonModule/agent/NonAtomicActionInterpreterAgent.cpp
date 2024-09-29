#include "NonAtomicActionInterpreterAgent.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "utils/RelationUtils.hpp"

using namespace commonModule;

ScResult NonAtomicActionInterpreterAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr nonAtomicActionAddr;
  try
  {
    ScAddr const & nonAtomicActionTemplateAddr =
        utils::IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_1);
    if (!nonAtomicActionTemplateAddr.IsValid())
    {
      throw std::runtime_error("Action params are not formed correctly.");
    }
    ScAddr const & argumentsSet = utils::IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_2);

    ScTemplateParams templateParams = createTemplateParams(nonAtomicActionTemplateAddr, argumentsSet);
    nonAtomicActionAddr = replaceNonAtomicAction(nonAtomicActionTemplateAddr, templateParams);
    generateNonAtomicActionTemplate(nonAtomicActionTemplateAddr, templateParams);
  }
  catch (std::exception & ex)
  {
    SC_LOG_ERROR(ex.what());
    return action.FinishUnsuccessfully();
  }

  initFields();
  try
  {
    nonAtomicActionInterpreter->interpret(nonAtomicActionAddr);
  }
  catch (std::runtime_error & ex)
  {
    deleteFields();
    SC_LOG_ERROR(ex.what());
    return action.FinishUnsuccessfully();
  }
  deleteFields();

  return action.FinishSuccessfully();
}

ScAddr NonAtomicActionInterpreterAgent::GetActionClass() const
{
  return Keynodes::action_interpret_non_atomic_action;
}

void NonAtomicActionInterpreterAgent::generateNonAtomicActionTemplate(
    ScAddr const & nonAtomicActionTemplateAddr,
    ScTemplateParams const & templateParams)
{
  ScTemplate nonAtomicActionTemplate;

  m_context.BuildTemplate(nonAtomicActionTemplate, nonAtomicActionTemplateAddr);
  ScTemplateGenResult templateGenResult;

  m_context.GenerateByTemplate(nonAtomicActionTemplate, templateGenResult, templateParams);
}

ScAddr NonAtomicActionInterpreterAgent::getTemplateKeyElement(ScAddr const & templateAddr)
{
  ScAddr templateKeyElement =
      utils::IteratorUtils::getAnyByOutRelation(&m_context, templateAddr, ScKeynodes::rrel_key_sc_element);

  if (!templateKeyElement.IsValid())
  {
    throw std::runtime_error("Template key element not found.");
  }

  return templateKeyElement;
}

ScTemplateParams NonAtomicActionInterpreterAgent::createTemplateParams(
    ScAddr const & nonAtomicActionTemplate,
    ScAddr const & argumentsSet)
{
  ScTemplateParams templateParams;
  if (argumentsSet.IsValid())
  {
    ScAddr templateKeyElement;
    templateKeyElement = getTemplateKeyElement(nonAtomicActionTemplate);
    for (int index = 1;; index++)
    {
      ScAddr role = RelationUtils::getIndexRelation(&m_context, index);
      if (!role.IsValid())
      {
        break;
      }
      ScAddr argument = utils::IteratorUtils::getAnyByOutRelation(&m_context, argumentsSet, role);
      if (!argument.IsValid())
      {
        break;
      }
      ScIterator5Ptr variablesIterator5 = m_context.CreateIterator5(
          templateKeyElement, ScType::EdgeAccessVarPosPerm, ScType::NodeVar, ScType::EdgeAccessVarPosPerm, role);
      if (variablesIterator5->Next())
      {
        templateParams.Add(m_context.GetElementSystemIdentifier(variablesIterator5->Get(2)), argument);
      }
    }
  }

  return templateParams;
}

ScAddr NonAtomicActionInterpreterAgent::replaceNonAtomicAction(
    ScAddr const & templateAddr,
    ScTemplateParams & templateParams)
{
  ScAddr keyElementReplacement = m_context.GenerateNode(ScType::NodeConst);
  ScAddr templateKeyElement;
  templateKeyElement = getTemplateKeyElement(templateAddr);
  templateParams.Add(m_context.GetElementSystemIdentifier(templateKeyElement), keyElementReplacement);

  return keyElementReplacement;
}

void NonAtomicActionInterpreterAgent::initFields()
{
  this->nonAtomicActionInterpreter = new NonAtomicActionInterpreter(&m_context);
}

void NonAtomicActionInterpreterAgent::deleteFields()
{
  delete this->nonAtomicActionInterpreter;
}
