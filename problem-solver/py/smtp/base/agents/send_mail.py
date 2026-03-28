import logging

from abc import ABC

from sc_client.models import ScAddr
from sc_kpm import ScKeynodes, ScResult
from sc_kpm.utils import (
    get_link_content_data,
    search_element_by_role_relation,
)
from sc_kpm.utils.action_utils import (
    finish_action_with_status,
    get_action_arguments,
)

from auth.base.models import User
from smtp.base.agents import MailAgent
from smtp.base.models import Mail


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class SendMailAgent(MailAgent, ABC):
    def __init__(self, action):
        super().__init__(action)
        self.rrel_name = ScKeynodes.get("rrel_name")
        self.rrel_email = ScKeynodes.get("rrel_email")
        self.rrel_mail = ScKeynodes.get("rrel_mail")

    def on_event(
        self,
        event_element: ScAddr,  # noqa: ARG002
        event_edge: ScAddr,  # noqa: ARG002
        action_element: ScAddr,
    ) -> ScResult:
        try:
            result = self.run(action_element)
            is_successful = result == ScResult.OK
            finish_action_with_status(action_element, is_successful)
            self.logger.info(
                "Finished %s",
                "successfully" if is_successful else "unsuccessfully",
            )
            return result
        except Exception as e:
            self.logger.error("Finished with error: %s", e)

    def run(self, action_node: ScAddr) -> ScResult:
        message_addr, self.author_node = get_action_arguments(
            action_node,
            2,
        )
        mail = self.get_mail(message_addr)
        if mail is None:
            self.logger.error("Did not get mail")
            return ScResult.ERROR
        res = self.service.send_mail(mail)
        if not res:
            return ScResult.ERROR
        return ScResult.OK

    def get_mail(self, message_addr: ScAddr) -> Mail:
        contact: User | None = None
        mail_link = search_element_by_role_relation(
            message_addr,
            self.rrel_mail,
        )
        email_link = search_element_by_role_relation(
            message_addr,
            self.rrel_email,
        )
        name_link = search_element_by_role_relation(
            message_addr,
            self.rrel_name,
        )

        if name_link:
            contact = self.get_contact(name_link)

        if email_link:
            contact = User(
                name=" ",
                email=get_link_content_data(email_link),
            )
        if contact is None:
            return None

        return Mail(
            sender=self.get_author(),
            receiver=contact,
            body=get_link_content_data(mail_link),
        )
