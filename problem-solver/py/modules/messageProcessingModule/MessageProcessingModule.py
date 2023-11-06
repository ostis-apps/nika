from sc_kpm import ScModule
from .AgentArtificialNeuralNetwork import AgentArtificialNeuralNetwork


class MessageProcessingModule(ScModule):
    def __init__(self):
        super().__init__(AgentArtificialNeuralNetwork())
