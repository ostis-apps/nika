import logging

from sc_client.client import generate_by_template, search_by_template
from sc_client.constants import sc_type
from sc_client.models import ScAddr, ScTemplate
from sc_kpm import ScKeynodes, ScResult
from sc_kpm.utils import (
    generate_link,
    get_link_content_data,
    search_element_by_role_relation,
)
from sc_kpm.utils.action_utils import (
    finish_action_with_status,
    get_action_arguments,
)

from auth.base.agents.integration_agent import IntegrationAgent
from auth.base.models import User


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class CreateContactAgent(IntegrationAgent):
    def __init__(self):
        super().__init__("action_create_contact")
        self.nrel_contacts = ScKeynodes.resolve(
            "nrel_contacts",
            sc_type.CONST_NODE_NON_ROLE,
        )
        self.rrel_name = ScKeynodes.get("rrel_name")
        self.rrel_email = ScKeynodes.get("rrel_email")

    @property
    def check_token_agent_action(self) -> str:
        pass

    def on_event(
        self,
        event_element: ScAddr,  # noqa: ARG002
        event_edge: ScAddr,  # noqa: ARG002
        action_element: ScAddr,
    ) -> ScResult:
        result = self.run(action_element)
        is_successful = result != ScResult.ERROR
        finish_action_with_status(action_element, is_successful)
        self.logger.info(
            "Finished %s",
            "successfully" if is_successful else "unsuccessfully",
        )
        return result

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("Create contact agent started")

        try:
            args = get_action_arguments(action_node, 2)

            self.author_node = args[1]
            message_addr = args[0]

            name_link = search_element_by_role_relation(
                message_addr,
                self.rrel_name,
            )
            email_link = search_element_by_role_relation(
                message_addr,
                self.rrel_email,
            )

            if not any([self.author_node, name_link, email_link]):
                self.logger.error(
                    "Miss required action params: "
                    f"{self.author_node=}, {name_link=}, {email_link=}",
                )
                return ScResult.ERROR

            name = get_link_content_data(name_link)
            email = get_link_content_data(email_link)

            contact = User(name=name, email=email)
            self.logger.info("%s", contact)

            contact_node = self.save_contact(contact)
            if contact_node is not None:
                self.logger.info("Save contact")
                return ScResult.OK

        except Exception as e:
            self.logger.info("Finished with an error %s", e)
            return ScResult.ERROR

        return ScResult.OK

    def save_contact(self, contact: User):
        template = ScTemplate()
        contact_alias = "_contact"
        tuple_node = self.get_contact_tuple_node()

        # generate new contact node
        template.triple(
            tuple_node,
            sc_type.VAR_PERM_POS_ARC,
            sc_type.VAR_NODE >> contact_alias,
        )

        # connect with name and email
        template.quintuple(
            contact_alias,
            sc_type.VAR_COMMON_ARC,
            generate_link(contact.name),
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_name,
        )
        template.quintuple(
            contact_alias,
            sc_type.VAR_COMMON_ARC,
            generate_link(contact.email),
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_email,
        )
        res = generate_by_template(template)
        return res.get(contact_alias)

    def get_contact_tuple_node(
        self,
    ):
        template = ScTemplate()
        alias = "_contact_tuple"
        template.quintuple(
            self.author_node,
            sc_type.VAR_COMMON_ARC,
            sc_type.VAR_NODE_TUPLE >> alias,
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_contacts,
        )
        res = search_by_template(template)

        if res:
            self.logger.info("Find contact tuple for this author")
            return res[0].get(alias)
        else:
            self.logger.info("Generate contact tuple for this author")
            res = generate_by_template(template)
            return res.get(alias)
