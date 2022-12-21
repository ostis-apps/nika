#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"

#include "keynodes/ExampleKeynodes.hpp"

#include "ExampleAgent.hpp"

using namespace utils;
using namespace exampleModule;


SC_AGENT_IMPLEMENTATION(ExampleAgent) {
    ScAddr actionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);

    if (!checkActionClass(actionNode)) {
        return SC_RESULT_OK;
    }

    SC_LOG_DEBUG("ExampleAgent: start");

    ScAddr message = IteratorUtils::getAnyByOutRelation(
            &m_memoryCtx, actionNode, scAgentsCommon::CoreKeynodes::rrel_1);

    if (!message.IsValid()) {
        SC_LOG_ERROR("Message node is not valid.");
        AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
        return SC_RESULT_ERROR;
    }

    auto messageValue = CommonUtils::getLinkContent(&m_memoryCtx, message);

    ScAddr messageClass = receiveMessageClass(messageValue);

    ScAddr edge = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, messageClass, message);
    ScAddrVector answerVector = {messageClass, edge, message};

    SC_LOG_DEBUG("ExampleAgent: finish");

    AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, true);
    return SC_RESULT_OK;
}

bool ExampleAgent::checkActionClass(const ScAddr &actionNode) {
    return m_memoryCtx.HelperCheckEdge(
            ExampleKeynodes::action_example, actionNode, ScType::EdgeAccessConstPosPerm);
}

ScAddr ExampleAgent::receiveMessageClass(std::string messageValue) {
    if (messageValue.find('?') != std::string::npos)
        return ExampleKeynodes::concept_interrogative_sentence;
    else if (messageValue.find('!') != std::string::npos)
        return ExampleKeynodes::concept_exclamatory_sentence;
    else
        return ExampleKeynodes::concept_declarative_sentence;
}
