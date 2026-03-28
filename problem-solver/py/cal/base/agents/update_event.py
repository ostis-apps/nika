import logging

from abc import ABC, abstractmethod

from sc_client.models import ScAddr
from sc_kpm import ScKeynodes, ScResult
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
from cal.base.models import (
    Date,
    Event,
    EventWithDate,
)
from cal.base.services import EventService


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class UpdateEventAgent(CalendarAgent, ABC):
    def __init__(self, action):
        super().__init__(action)
        self.rrel_new_event_summary = ScKeynodes.get(
            "rrel_new_event_summary",
        )

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
            is_successful = result != ScResult.ERROR
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
        event = self._find_old_event_params(message_addr)

        if event is None:
            self.logger.error("Did not get event")
            return ScResult.ERROR
        old_event = self.event_service.search_event(event)
        new_event = self._find_new_event_params(message_addr, old_event)
        if old_event is None:
            self.logger.info("Event hasn't been found in Google Calendar")
            return ScResult.ERROR
        self.logger.info(f"Found event: {old_event.summary}")

        res = self.event_service.update_event(
            old_event,
            new_event,
        )
        if res is None:
            self.logger.error("Do not update event!")
            return ScResult.ERROR
        self.logger.info("New event %s", res)
        # check other parameters

        summary_addr = search_element_by_role_relation(
            message_addr,
            self.rrel_event_summary,
        )
        generate_action_result(action_node, summary_addr)
        return ScResult.OK

    def _find_old_event_params(self, message_addr: ScAddr) -> Event:
        summary_link = search_element_by_role_relation(
            message_addr,
            self.rrel_event_summary,
        )
        self.logger.info("Find event summary link")

        summary = get_link_content_data(summary_link)
        event = Event(summary=summary)
        return event

    def _find_new_event_params(
        self,
        message_addr: ScAddr,
        old_event: Event,
    ) -> EventWithDate:
        summary = ""
        start_date: Date | None = None
        end_date: Date | None = None

        # Поиск ссылок
        new_summary_link = search_element_by_role_relation(
            message_addr,
            self.rrel_new_event_summary,
        )
        start_time_link = search_element_by_role_relation(
            message_addr,
            self.rrel_start_time,
        )
        end_time_link = search_element_by_role_relation(
            message_addr,
            self.rrel_end_time,
        )

        # Получение данных из ссылок
        if new_summary_link:
            summary = get_link_content_data(new_summary_link)

        if start_time_link:
            start_time = get_link_content_data(start_time_link)
            start_date = Date(iso=start_time)

        if end_time_link:
            end_time = get_link_content_data(end_time_link)
            end_date = Date(iso=end_time)

        if start_date is None:
            raise ValueError("Start date is required")

        event = EventWithDate(
            summary=summary if summary else old_event.summary,
            start_date=start_date,
            end_date=end_date,
        )

        return event
