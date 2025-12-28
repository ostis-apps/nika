# This files contains your custom actions which can be used to run
# custom Python code.
#
# See this guide on how to implement these action:
# https://rasa.com/docs/rasa/custom-actions

from typing import Any

from rasa_sdk import Action, Tracker
from rasa_sdk.executor import CollectingDispatcher

class ActionChangeColorRename(Action):
    def name(self) -> str:
        return "action_change_color_rename"

    def run(self, dispatcher: CollectingDispatcher, tracker, domain):
        user_message = tracker.latest_message.get('text')

        updated_message = user_message.replace("хедера", "хедер")
        dispatcher.utter_message(text=updated_message)
        return []

