# from sc_client import client
from sc_client.models import ScAddr
from sc_kpm import ScAgentClassic, ScResult, ScKeynodes

from reader import Reader
from writer import Writer


class JSONAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("json_agent")
        self._keynodes = ScKeynodes()

    def on_event(self, class_node: ScAddr, edge: ScAddr, action_node: ScAddr) -> ScResult:
        if not self._confirm_action_class(action_node):
            return ScResult.SKIP

        status = ScResult.OK

        path = "data"
        reader = Reader(path)
        for instance in reader.collect():
            Writer.write(instance)

        return status
