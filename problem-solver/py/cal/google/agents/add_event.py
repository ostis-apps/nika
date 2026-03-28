from auth.google.agents import GoogleAgent
from cal.base.agents import AddEventAgent
from cal.google.services import GoogleEventService


class AddGoogleEventAgent(AddEventAgent, GoogleAgent):
    def __init__(self):
        super().__init__("action_add_google_calendar_event")
        self._service = GoogleEventService(self.logger)

    @property
    def event_service(self):
        return self._service
