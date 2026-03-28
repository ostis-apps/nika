import logging

from abc import ABC, abstractmethod

from sc_client.models import ScAddr
from sc_kpm.identifiers import CommonIdentifiers
from sc_kpm.sc_sets import ScStructure
from sc_kpm.utils import (
    get_link_content_data,
    search_element_by_non_role_relation,
)
from sc_kpm.utils.action_utils import (
    execute_agent,
    get_action_result,
)

from auth.base.agents.integration_agent import IntegrationAgent
from auth.base.models import User
from smtp.base.services import SMTPService


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class MailAgent(IntegrationAgent, ABC):
    def __init__(self, action):
        super().__init__(action)

    @property
    @abstractmethod
    def service(self) -> SMTPService:
        pass

    def get_contact(self, name_link: ScAddr) -> User | None:
        action_class_name = "action_find_contact"
        logging.info("Trying to find contact...")
        action, is_successful = execute_agent(
            arguments={name_link: False, self.author_node: False},
            concepts=[
                CommonIdentifiers.ACTION,
                action_class_name,
            ],
        )
        if is_successful:
            contact_node = ScStructure(
                set_node=get_action_result(action),
            ).elements_set.pop()
            name_link = search_element_by_non_role_relation(
                contact_node,
                self.nrel_name,
            )
            email_link = search_element_by_non_role_relation(
                contact_node,
                self.nrel_email,
            )
            name = get_link_content_data(name_link)
            email = get_link_content_data(email_link)
            logging.info("Successfully find contact!")
            return User(name=name, email=email)
        logging.error("Did not find contact")
        return None
