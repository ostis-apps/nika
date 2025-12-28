print('Loading Intent Validator Component')

from rasa.engine.recipes.default_recipe import DefaultV1Recipe
from rasa.engine.graph import ExecutionContext, GraphComponent
from rasa.engine.storage.storage import ModelStorage
from rasa.shared.nlu.training_data.message import Message
from rasa.engine.storage.resource import Resource
from typing import Any, Dict, List, Text, Optional
import yaml


@DefaultV1Recipe.register(
    DefaultV1Recipe.ComponentType.INTENT_CLASSIFIER,
    is_trainable=False
)
class IntentValidator(GraphComponent):
    def __init__(self, config: Dict[Text, Any], model_storage: ModelStorage, resource: Optional[Resource] = None) -> None:
        super().__init__()
        self.nlu_threshold: float = 0.9

    def process(self, messages: List[Message]) -> List[Message]:
        for message in messages:
            intent: Dict = message.get('intent', {})
            confidence: float = intent.get('confidence', 0.)

            if confidence < self.nlu_threshold:
                message.set('intent', {
                    'name': 'unknown_intent',
                    'confidence': 1.
                })
                message.set('entities', [])

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
