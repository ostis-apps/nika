import logging

from auth.base.agents import CreateTokensAgent
from auth.config import client
from auth.google.services import GoogleOauthTokenService


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class GoogleCreateTokensAgent(CreateTokensAgent):
    def __init__(self):
        super().__init__("action_create_google_tokens")
        self._service = GoogleOauthTokenService(client)

    @property
    def service(self) -> GoogleOauthTokenService:
        return self._service
