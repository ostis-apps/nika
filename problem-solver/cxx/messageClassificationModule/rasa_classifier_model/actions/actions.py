# This files contains your custom actions which can be used to run
# custom Python code.
#
# See this guide on how to implement these action:
# https://rasa.com/docs/rasa/custom-actions


# This is a simple example for a custom action which utters "Hello World!"

from typing import Any, Text, Dict, List

from rasa_sdk import Action, Tracker
from rasa_sdk.executor import CollectingDispatcher
#
#
# class ActionHelloWorld(Action):
#
#     def name(self) -> Text:
#         return "action_hello_world"
#
#     def run(self, dispatcher: CollectingDispatcher,
#             tracker: Tracker,
#             domain: Dict[Text, Any]) -> List[Dict[Text, Any]]:
#
#         dispatcher.utter_message(text="Hello World!")
#
#         return []

# from rasa_sdk import Action
# from rasa_sdk.executor import CollectingDispatcher
# from rasa_sdk.events import SlotSet

class ActionChangeColorRename(Action):
    def name(self) -> str:
        return "action_change_color_rename"

    def run(self, dispatcher: CollectingDispatcher, tracker, domain):
        # Логика вашего действия
        user_message = tracker.latest_message.get('text')

        # Заменяем "хедера" на "хедер"
        updated_message = user_message.replace("хедера", "хедер")
        dispatcher.utter_message(text=updated_message)
        return []

