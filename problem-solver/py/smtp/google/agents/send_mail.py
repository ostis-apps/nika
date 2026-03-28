import logging

from auth.google.agents import GoogleAgent
from smtp.base.agents import SendMailAgent
from smtp.google.services import GoogleSMTPService


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class SendGoogleMailAgent(SendMailAgent, GoogleAgent):
    def __init__(self):
        self._service = GoogleSMTPService()
        super().__init__("action_send_google_mail")

    @property
    def service(self):
        return self._service
