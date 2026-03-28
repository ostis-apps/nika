from auth.base.agents import IntegrationAgent


class GoogleAgent(IntegrationAgent):
    @property
    def check_token_agent_action(self) -> str:
        return "action_check_google_token"
