import logging
from unittest import TestCase

from sc_client.client import generate_elements_by_scs
from sc_kpm import ScServer
from pathlib import Path
from tests import TESTS_DIR

logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)
SC_SERVER_URL = "ws://localhost:8090/ws_json"

class BaseTestCase(TestCase):
    def setUp(self) -> None:
        self.server = ScServer(SC_SERVER_URL)
        self.server.connect()

    @staticmethod
    def load_scs(scs_file_path):
        with open(scs_file_path, "r") as file:
            scs_file = file.read()
            separator = ";;"
            scs_lines = [
                item.replace("\n", "").replace("   ", " ") + separator
                for item in scs_file.split(separator)
                if item and item != "\n"
            ]
            return generate_elements_by_scs(scs_lines)

    def tearDown(self) -> None:
        self.server.disconnect()
