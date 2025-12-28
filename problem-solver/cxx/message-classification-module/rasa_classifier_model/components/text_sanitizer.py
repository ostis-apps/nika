print('Loading Text Sanitizer Component')

import re
from rasa.engine.recipes.default_recipe import DefaultV1Recipe
from rasa.engine.graph import ExecutionContext, GraphComponent
from rasa.engine.storage.storage import ModelStorage
from rasa.engine.storage.resource import Resource
from rasa.shared.nlu.training_data.message import Message
from rasa.shared.nlu.training_data.training_data import TrainingData
from typing import Any, Dict, List, Text, Optional

@DefaultV1Recipe.register(
    DefaultV1Recipe.ComponentType.MESSAGE_FEATURIZER,
    is_trainable=False
)
class TextSanitizer(GraphComponent):
    def __init__(self, config: Dict[Text, Any], model_storage: ModelStorage, resource: Optional[Resource] = None) -> None:
        super().__init__()
        self.allowed_pattern = re.compile(r'[^\x00-\x7Fа-яА-ЯёЁ0-9 .,!?()\[\]{}\-+=<>:;\'"@#$%^&*_~№]')

    def process(self, messages: List[Message]) -> List[Message]:
        for message in messages:
            text = message.get('text')
            if text:
                cleaned_text = self.allowed_pattern.sub('', text)
                if cleaned_text != text:
                    print(f"[TextSanitizer] Cleaned: '{text}' → '{cleaned_text}'")
                message.set('text', cleaned_text)
        return messages

    def process_training_data(self, training_data: TrainingData) -> TrainingData:
        return training_data

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
