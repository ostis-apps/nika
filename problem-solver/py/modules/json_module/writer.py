import structures
import sc_api


class Writer:
    @staticmethod
    def write(instance_: structures.Instance):
        sc_api.Client.create_element(instance_)
