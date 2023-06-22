from sc_kpm import ScModule
from .WeatherAgent import WeatherAgent


class ExampleModule(ScModule):
    def __init__(self):
        super().__init__(WeatherAgent())