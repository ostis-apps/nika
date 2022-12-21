#pragma once

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-memory/kpm/sc_agent.hpp"

#include "ExampleAgent.generated.hpp"

namespace exampleModule {
    class ExampleAgent : public ScAgent {
        SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
        SC_GENERATED_BODY()

    private:
        bool checkActionClass(const ScAddr &actionNode);

        ScAddr receiveMessageClass(std::string messageValue);
    };
}  // namespace exampleModule
