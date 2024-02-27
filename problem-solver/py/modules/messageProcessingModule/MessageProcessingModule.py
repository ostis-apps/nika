from sc_kpm import ScModule
from .WeatherAgent import WeatherAgent
from .CourseAgent import CourseAgent

class MessageProcessingModule(ScModule):
    def __init__(self):
        super().__init__(WeatherAgent(), CourseAgent())
