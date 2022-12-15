import dataclasses


class RequiredFields:
    required_fields = ("main_idtf", "system_idtf", "instance_of")


@dataclasses.dataclass
class Concept:
    additional_fields: list
    required_fields: list = RequiredFields.required_fields


class Instance:
    def __init__(self, **kwargs):
        self.__attributes = kwargs.keys()

        for key, value in kwargs.items():
            setattr(self, key, value)

        for attribute in RequiredFields.required_fields:
            if attribute not in self.__attributes:
                raise TypeError(f"Instance.__init__ missing 1 required positional argument: '{attribute}'")

    def items(self):
        for key in self.__attributes:
            yield key, getattr(self, key)
