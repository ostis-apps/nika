from sc_kpm import ScModule

from .launch_app_agent import LaunchAppAgent


class LaunchModule(ScModule):
    def __init__(self):
        super().__init__(LaunchAppAgent())
