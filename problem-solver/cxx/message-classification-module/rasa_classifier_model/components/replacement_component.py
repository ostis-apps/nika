print('Loading Replacement Component')

from rasa.engine.recipes.default_recipe import DefaultV1Recipe
from rasa.engine.graph import ExecutionContext, GraphComponent
from rasa.engine.storage.storage import ModelStorage
from rasa.shared.nlu.training_data.message import Message
from rasa.engine.storage.resource import Resource
from typing import Any, Dict, List, Text, Optional
import yaml


@DefaultV1Recipe.register(
    DefaultV1Recipe.ComponentType.ENTITY_EXTRACTOR,
    is_trainable=False
)
class EntityReplacer(GraphComponent):
    def __init__(self, config: Dict[Text, Any], model_storage: ModelStorage, resource: Optional[Resource] = None) -> None:
        super().__init__()
        with open('data/replacements.yml', 'r') as replacements_file:
            replacements_data = yaml.safe_load(replacements_file)
        self.replacements = replacements_data.get('replacements', {})

    def process(self, messages: List[Message]) -> List[Message]:
        for message in messages:
            entities = message.get('entities', [])
            for entity in entities:
                if self.replacements.get(entity['value']):
                    original_value = entity['value']
                    replacement = self.replacements.get(original_value)
                    entity['value'] = replacement
        return messages  

    @classmethod
    def create(
        cls,
        config: Dict[Text, Any],
        model_storage: ModelStorage,
        execution_context: ExecutionContext,
        resource: Optional[Resource] = None,
        **kwargs: Any,
    ) -> GraphComponent:
        return cls(config, model_storage)
