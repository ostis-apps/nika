from abc import ABC

from sc_kpm import ScKeynodes

from auth.base.agents import IntegrationAgent


class CalendarAgent(IntegrationAgent, ABC):
    def __init__(self, action: str):
        super().__init__(action)
        self.rrel_event_summary = ScKeynodes.get("rrel_event_summary")
        self.rrel_start_time = ScKeynodes.get("rrel_start_time")
        self.rrel_end_time = ScKeynodes.get("rrel_end_time")
