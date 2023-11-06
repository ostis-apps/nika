import numpy as np
from numpy import exp,sin,cos,tan,arcsin,arccos,arctan
from typing import List

class FNN:
    def __init__(self,weights: np.ndarray[np.ndarray[np.float64]],input_values: np.ndarray[np.ndarray[np.float64]],output_neurons: int, activation_function:str) -> None:
        self._weights : np.ndarray[np.ndarray[np.float64]] = weights 
        self._input_values : np.ndarray[np.ndarray[np.float64]] = input_values
        self._output_neurons : int = output_neurons
        self._activation_function: str = activation_function
    
    def predict(self):
        for data in self._input_values:
            print(self.sigmoid(data.dot(self._weights)))

    def sigmoid(self,x):
        return eval(self._activation_function)