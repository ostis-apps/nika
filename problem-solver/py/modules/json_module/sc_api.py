import structures

from sc_client import client


class Client:
    url = "ws://localhost:8090/ws_json"

    @staticmethod
    def create_element(instance_: structures.Instance) -> None:
        client.connect(Client.url)

        if not client.is_connected():
            raise ConnectionError

        instance_ = dict(instance_.items())
        name = instance_.pop('system_idtf').lower().replace(" ", "_").replace("/", "_")

        memory = [
            f"{name} <- concept_{instance_.pop('instance_of')};;",
            f"{name} <- concept_wit_entity;;",
            f"{name} => nrel_main_idtf: [{instance_.pop('main_idtf').lower()}] (* <- lang_ru;; *);;",
        ]

        for key, value in instance_.items():
            memory.append(f"{name} => nrel_{key}: concept_{value};;")

        client.create_elements_by_scs(memory)

        client.disconnect()
