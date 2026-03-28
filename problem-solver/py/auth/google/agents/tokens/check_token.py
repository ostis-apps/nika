import logging

from auth.base.agents import CheckTokenAgent
from auth.config import client
from auth.google.services import GoogleOauthTokenService


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class CheckGoogleTokenAgent(CheckTokenAgent):
    def __init__(self):
        super().__init__("action_check_google_token")
        self._service = GoogleOauthTokenService(client)

    @property
    def service(self) -> GoogleOauthTokenService:
        return self._service
