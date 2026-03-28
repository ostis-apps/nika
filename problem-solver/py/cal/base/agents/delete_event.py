import logging

from abc import ABC, abstractmethod

from sc_client.models import ScAddr
from sc_kpm import ScResult
from sc_kpm.utils import (
    get_link_content_data,
    search_element_by_role_relation,
)
from sc_kpm.utils.action_utils import (
    finish_action_with_status,
    generate_action_result,
    get_action_arguments,
)

from cal.base.agents.calendar_agent import (
    CalendarAgent,
)
from cal.base.models import Event
from cal.base.services import EventService


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class DeleteEventAgent(CalendarAgent, ABC):
    @property
    @abstractmethod
    def event_service(self) -> EventService:
        pass

    def on_event(
        self,
        event_element: ScAddr,  # noqa: ARG002
        event_edge: ScAddr,  # noqa: ARG002
        action_element: ScAddr,
    ) -> ScResult:
        try:
            result = self.run(action_element)
            is_successful = result == ScResult.OK
            finish_action_with_status(action_element, is_successful)
            self.logger.info(
                "Finished %s",
                "successfully" if is_successful else "unsuccessfully",
            )
            return result
        except Exception as e:
            self.logger.info("Finished with an error %s", e)
            return ScResult.ERROR

    def run(self, action_node: ScAddr) -> ScResult:
        message_addr, self.author_node = get_action_arguments(
            action_node,
            2,
        )
        token = self.get_token()
        if token is None:
            self.logger.error("Did not get token")
            return ScResult.ERROR
        if not message_addr:
            self.logger.error("Did not have message address")
            return ScResult.ERROR

        self.event_service.token = token
        event = self._find_event_params(message_addr)

        if event is None:
            self.logger.error("Did not get event")
            return ScResult.ERROR

        event_with_id = self.event_service.search_event(event)
        if not event_with_id:
            self.logger.info("Did not find event in Google Calendar")
            return ScResult.UNKNOWN

        self.logger.info(f"Find event: {event_with_id.summary}")

        deletion_result = self.event_service.delete_event(
            event_with_id,
        )
        if deletion_result is not True:
            self.logger.info("Did not delete event in Google Calendar")
            return ScResult.ERROR

        self.logger.info("Delete event")

        summary_addr = search_element_by_role_relation(
            message_addr,
            self.rrel_event_summary,
        )
        generate_action_result(action_node, summary_addr)
        return ScResult.OK

    def _find_event_params(self, message_addr: ScAddr) -> Event:
        summary_link = search_element_by_role_relation(
            message_addr,
            self.rrel_event_summary,
        )
        self.logger.info("Find event summary link")

        summary = get_link_content_data(summary_link)
        event = Event(summary=summary)
        return event
