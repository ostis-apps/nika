import pathlib
import json
import typing
import structures


class ValidConcepts:
    concepts = {
        "animal": structures.Concept([
            "animal_class", "habitat", "animal_food_preferences", "recommended_type_of_cartridges"
        ]),
        "ammo": structures.Concept(["ammo_type", "material"]),
        "biocoenosis": structures.Concept(["biocoenosis_type"]),
    }


class Folder:
    __extension = ".json"

    def __init__(self, path_: [str | pathlib.Path]) -> None:
        self.__path = pathlib.Path(path_)

    def __iter__(self) -> typing.Generator[pathlib.Path, None, None]:
        for file in self.__path.iterdir():
            if file.suffix == self.__extension:
                yield pathlib.Path(file)


class Reader:
    __valid_concepts_only = True

    def __init__(self, path_: [str | pathlib.Path]) -> None:
        self.__path = pathlib.Path(path_)

    def collect(self):
        if self.__path.is_dir():
            for file in Folder(self.__path):
                yield self.__read_json(file)
        elif self.__path.is_file():
            yield self.__read_json(self.__path)
        else:
            return None

    def __read_json(self, path_: pathlib.Path):
        with open(
                path_,
                mode="r",
                encoding="utf-8"
        ) as file:
            file = json.load(
                file
            )

            self.__check_file(file, path_)

            return structures.Instance(**file)

    def __check_file(self, file_: dict, path_: pathlib.Path):
        if not Reader.__check_required_fields_in_file(file_):
            print(
                f"🔴 | {path_.name} | Fields Error. Required fields: {structures.Concept.required_fields}"
            )

        elif self.__valid_concepts_only and not Reader.__check_concept_in_file(file_):
            print(f"🔴 | {path_.name} | Concept error. Allowed concepts: {ValidConcepts.concepts.keys()}")

        elif self.__valid_concepts_only and not Reader.__check_relations_in_file(file_):
            print(
                f"🔴 | {path_.name} | Relationship error. Must have relations: "
                f"{ValidConcepts.concepts[file_['instance_of']].additional_fields}"
            )

        else:
            print(f"🟢 | {path_.name} | Instance loaded successfully")

    @staticmethod
    def __check_required_fields_in_file(file_: dict) -> bool:
        return set(structures.Concept.required_fields).issubset(file_.keys())

    @staticmethod
    def __check_concept_in_file(file_: dict) -> bool:
        return file_["instance_of"] in ValidConcepts.concepts.keys()

    @staticmethod
    def __check_relations_in_file(file_: dict) -> bool:
        return (
                set(file_.keys()) - set(structures.Concept.required_fields)
                ==
                set(ValidConcepts.concepts[file_["instance_of"]].additional_fields)
        )
