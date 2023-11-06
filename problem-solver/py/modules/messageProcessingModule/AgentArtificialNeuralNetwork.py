from sc_client.models import ScAddr
import sc_kpm
import numpy as np
from sc_kpm import ScAgentClassic
from sc_kpm.sc_result import ScResult
from sc_kpm.sc_sets import ScSet, ScStructure
from sc_client.models import *
from sc_client.constants import sc_types
from sc_client.client import *
from typing import List, Union
from .FNN import FNN

class AgentArtificialNeuralNetwork(ScAgentClassic):
    
    def __init__(self) -> None:
        print('Created Agent')
        super().__init__("action_solve_artificial_neural_network")
        print('Initialized')

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        print('TEST')
        template = ScTemplate()
        template.triple_with_relation(
            action_element,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            sc_types.NODE_VAR,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            "rrel_1"
        )
        result = template_search(template)
        self.__ann_name: str = sc_kpm.utils.get_system_idtf(result[0][2])
        template = ScTemplate()
        template.triple_with_relation(
            action_element,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            sc_types.NODE_VAR,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            "rrel_2"
        )
        result = template_search(template)
        self.__data_set_name: str = sc_kpm.utils.get_system_idtf(result[0][2])
        print(event_element)
        print(event_edge)
        print(action_element)
        self.run()
        return 

    def run(self)->None:
        self._network: ScAddr = self.find_network()
        self._layers: List[ScAddr] = self.find_layers()
        self._input_neurons: List[ScAddr] = self.get_input_neurons()
        self._output_neurons: List[ScAddr] = self.get_output_neurons()
        self._weights: np.ndarray[np.ndarray[np.float64]] = self.get_weights()
        self._input_values: np.ndarray[np.ndarray[np.float64]] = self.get_input_values()
        self._activation_function: str

    def find_network(self) -> ScAddr:
        main_node = sc_kpm.ScKeynodes[self.__ann_name]
        template = ScTemplate()
        template.triple(  # searching for ann structure
            main_node, sc_types.EDGE_ACCESS_VAR_POS_PERM, sc_types.NODE_VAR_STRUCT
        )
        search_results = template_search(template)
        return search_results[0][2]

    def find_layers(self) -> List[ScAddr]:
        template = ScTemplate()
        template.triple_with_relation(  # searching for layers
            self._network,
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.NODE_VAR_STRUCT,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            sc_types.NODE_VAR_NOROLE,
        )
        search_results = template_search(template)
        return [search_results[0][2], search_results[1][2]]

    def identificators(self, addresses: Union[ScAddr, List[ScAddr]]) -> None:
        if isinstance(addresses, ScAddr):
            print(addresses, "=", sc_kpm.utils.get_system_idtf(addresses))
        elif isinstance(addresses, List):
            for address in addresses:
                if isinstance(address, ScAddr):
                    print(address, "=", sc_kpm.utils.get_system_idtf(address))

    def get_input_neurons(self) -> Union[List[ScAddr], ScAddr]:
        neuron_list = []
        target_layer: ScAddr
        for target_layer in self._layers:
            if sc_kpm.utils.get_system_idtf(target_layer) == "distribution_layer":
                break
        template = ScTemplate()
        template.triple_with_relation(  # searching for first neuron
            target_layer,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            sc_types.NODE_VAR,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            "rrel_1",
        )
        result = template_search(template)
        target_edge = result[0][1]
        neuron_list.append(result[0][2])
        while True:  # searching for other neurons
            template = ScTemplate()
            template.triple_with_relation(
                target_edge,
                sc_types.EDGE_D_COMMON_VAR,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                sc_types.EDGE_ACCESS_VAR_POS_PERM,
                "nrel_goto",
            )
            result = template_search(template)
            if len(result) == 0:
                break
            target_edge = result[0][2]
            template = ScTemplate()
            template.triple(target_layer, target_edge, sc_types.NODE_VAR)
            result = template_search(template)
            neuron_list.append(result[0][2])
        return neuron_list

    def get_output_neurons(self) -> Union[List[ScAddr], ScAddr]:
        target_layer: ScAddr
        for target_layer in self._layers:
            if sc_kpm.utils.get_system_idtf(target_layer) == "processing_layer":
                break
        template = ScTemplate()
        template.triple_with_relation(
            target_layer,
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.NODE_VAR,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            "nrel_activation_function"
        )
        result = template_search(template)
        template = ScTemplate()
        template.triple_with_relation(
            result[0][2],
            sc_types.EDGE_D_COMMON_VAR,
            sc_types.LINK_VAR,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            "nrel_text_translation"
        )
        result = template_search(template)
        self._activation_function = sc_kpm.utils.get_link_content_data(result[0][2])
        template = ScTemplate()
        template.triple(  # searching for output neurons
            target_layer, sc_types.EDGE_ACCESS_VAR_POS_PERM, sc_types.NODE_VAR
        )
        neurons = template_search(template)
        return [neuron[2] for neuron in neurons]

    def get_weights(self) -> np.ndarray[np.ndarray[np.float64]]:
        weights = []
        for input_neuron in self._input_neurons:
            weights_for_neuron = []
            for output_neuron in self._output_neurons:
                template = ScTemplate()
                template.triple(input_neuron, sc_types.EDGE_D_COMMON_VAR, output_neuron)
                result = template_search(template)
                for edge in result:
                    template = ScTemplate()
                    template.triple_with_relation(
                        sc_types.NODE_VAR_CLASS,
                        sc_types.EDGE_ACCESS_VAR_POS_PERM,
                        edge[1],
                        sc_types.EDGE_ACCESS_VAR_POS_PERM,
                        "rrel_weigth",
                    )
                    weight_node_template_result = template_search(template)
                    template = ScTemplate()
                    template.triple(
                        weight_node_template_result[0][0],
                        sc_types.EDGE_D_COMMON_VAR,
                        sc_types.LINK_VAR,
                    )
                    weight_node = template_search(template)
                    weights_for_neuron.append(
                        np.float64(
                            sc_kpm.utils.get_link_content_data(weight_node[0][2])
                        )
                    )
            weights.append(weights_for_neuron)
        return np.array(weights)

    def get_input_values(self) -> np.ndarray[np.ndarray[np.float64]]:
        data_set_node = sc_kpm.ScKeynodes["training_set"]
        input_values = []
        template = ScTemplate()
        template.triple(
            data_set_node, sc_types.EDGE_ACCESS_VAR_POS_PERM, self.__data_set_name
        )
        result = template_search(template)
        template = ScTemplate()
        template.triple(
            result[0][2], sc_types.EDGE_ACCESS_VAR_POS_PERM, sc_types.LINK_VAR
        )
        result = template_search(template)
        for set in result:
            input_values.append(sc_kpm.utils.get_link_content_data(set[2]).split(";"))
        return np.array(input_values, dtype=np.float64)


# def main():
#     url = "ws://localhost:8090/ws_json"
#     connect(url)
#     ann = AgentArtificialNeuralNetwork()
#     print(ann._layers, "layers ids")
#     ann.identificators(ann._layers)
#     print(ann._input_neurons, "input neurons ids")
#     print(ann._output_neurons, "output neurons ids")
#     print(ann._weights, "weights")
#     print(ann._input_values, "input values")
#     print(ann._activation_function, 'activation function')
#     fnn = FNN(ann._weights,ann._input_values,len(ann._output_neurons),ann._activation_function)
#     fnn.predict()
#     disconnect()


# if __name__ == "__main__":
#     main()
