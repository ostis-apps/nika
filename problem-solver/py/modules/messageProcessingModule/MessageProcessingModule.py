from sc_kpm import ScModule
from .WeatherAgent import WeatherAgent
from .HistoryAgent import HistoryAgent
from .RandomAgent import RandomAgent
from .TravellingAgent import TravellingAgent
from .FindSomePlacesAgent import FindSomePlacesAgent


class MessageProcessingModule(ScModule):
    def __init__(self):
        super().__init__(HistoryAgent(), RandomAgent(), WeatherAgent(), TravellingAgent(), FindSomePlacesAgent())

