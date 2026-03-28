import logging

from sc_client.client import (
    search_by_template,
)
from sc_client.constants import sc_type
from sc_client.models import (
    ScAddr,
    ScTemplate,
)
from sc_kpm import ScKeynodes, ScResult
from sc_kpm.utils import (
    get_link_content_data,
    search_element_by_non_role_relation,
)
from sc_kpm.utils.action_utils import (
    finish_action_with_status,
    generate_action_result,
    get_action_arguments,
)

from auth.base.agents.integration_agent import IntegrationAgent


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(name)s | %(message)s",
    datefmt="[%d-%b-%y %H:%M:%S]",
)


class FindContactAgent(IntegrationAgent):
    def __init__(self):
        super().__init__("action_find_contact")
        self.nrel_contacts = ScKeynodes.resolve(
            "nrel_contacts",
            sc_type.CONST_NODE_NON_ROLE,
        )

    @property
    def check_token_agent_action(self) -> str:
        pass

    def on_event(
        self,
        event_element: ScAddr,  # noqa: ARG002
        event_edge: ScAddr,  # noqa: ARG002
        action_element: ScAddr,
    ) -> ScResult:
        try:
            result = self.run(action_element)
            is_successful = result != ScResult.ERROR
            finish_action_with_status(action_element, is_successful)
            self.logger.info(
                "Finished %s",
                "successfully" if is_successful else "unsuccessfully",
            )
            return result
        except Exception as e:
            self.logger.error("Finished with error: %s", e)

    def run(self, action_node: ScAddr) -> ScResult:
        name_link, self.author_node = get_action_arguments(action_node, 2)
        if not any([name_link, self.author_node]):
            return ScResult.ERROR

        name = get_link_content_data(name_link)
        self.logger.info("Name link content: %s", name)

        contact_node = self.get_contact(name)
        if contact_node is None:
            return ScResult.ERROR

        generate_action_result(action_node, contact_node)

        return ScResult.OK

    def get_contact(self, name: str) -> ScAddr:
        template = ScTemplate()

        tuple_alias = "_contacts_tuple"
        contact_alias = "_contact"

        template.quintuple(
            self.author_node,
            sc_type.VAR_COMMON_ARC,
            sc_type.VAR_NODE_TUPLE >> tuple_alias,
            sc_type.VAR_PERM_POS_ARC,
            self.nrel_contacts,
        )
        template.triple(
            tuple_alias,
            sc_type.VAR_PERM_POS_ARC,
            sc_type.VAR_NODE >> contact_alias,
        )

        results = search_by_template(template)
        if results:
            for res in results:
                contact_node = res.get(contact_alias)
                name_link = search_element_by_non_role_relation(
                    contact_node,
                    self.nrel_name,
                )
                if name_link and get_link_content_data(name_link) == name:
                    return contact_node
            self.logger.error(
                "Did not find contact with name %s",
                name,
            )
        else:
            self.logger.error("Did not find contacts")
            return None
