from sc_kpm import ScModule
from json_agent import JSONAgent


class JSONModule(ScModule):
    def __init__(self):
        super().__init__(
            JSONAgent(),
        )
