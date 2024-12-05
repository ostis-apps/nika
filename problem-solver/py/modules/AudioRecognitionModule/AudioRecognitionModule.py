from sc_kpm import ScModule
from .AudioRecognitionAgent import AudioRecognitionAgent


class AudioRecognitionModule(ScModule):
    def __init__(self):
        super().__init__(AudioRecognitionAgent())
