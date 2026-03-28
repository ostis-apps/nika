import { 
    ScAddr,
    ScTemplate, 
    ScType 
} from 'ts-sc-client';
import { client } from '@api/sc/client';
import { generateLinkText } from './newMessageAgent';

const keynode_images = [
    { id: 'users', type: ScType.ConstNodeClass },
    { id: 'nrel_auth_session', type: ScType.NodeNonRole },
    { id: 'action_create_google_author', type: ScType.ConstNodeClass },
    { id: 'action_create_yandex_author', type: ScType.ConstNodeClass },
    { id: "action", type: ScType.ConstNodeClass },
    { id: "action_initiated", type: ScType.ConstNodeClass },
    { id: "rrel_1", type: ScType.ConstNodeRole },
    { id: "rrel_2", type: ScType.ConstNodeRole }
];


export const call_create_author_agent = async (
    code: string, 
    session: string,
    action: string,
) => {
    try {
        const keynodes = await client.resolveKeynodes(keynode_images);
        const user_node = await find_user_by_session(session, keynodes);
        if (!user_node)
        {
            // generate code and session links in kb
            const code_link = await generateLinkText(code);
            const session_link = await generateLinkText(session);

            // alias for template
            const action_node_alias = "_action_node"

            const template = new ScTemplate();
            
            // call agent (realization in python module)
            template.triple(
                keynodes["action"], 
                ScType.VarPermPosArc, 
                [ScType.VarNode, action_node_alias]
            );
            template.triple(
                keynodes[action], 
                ScType.VarPermPosArc, 
                action_node_alias
            );
            template.triple(
                keynodes["action_initiated"], 
                ScType.VarPermPosArc, 
                action_node_alias
            );

            if (code_link)
                template.quintuple(
                    action_node_alias,
                    ScType.VarPermPosArc,
                    code_link,
                    ScType.VarPermPosArc,
                    keynodes["rrel_2"],
                );
            
            if (session_link)
                template.quintuple(
                    action_node_alias,
                    ScType.VarPermPosArc,
                    session_link,
                    ScType.VarPermPosArc,
                    keynodes["rrel_1"],
                );

            const res = await client.generateByTemplate(template, {});
            return res?.get(action_node_alias)
        }
        else {
            console.log(
                "User with such session: ",
                session, 
                " already exists"
                )
        }

    } catch (error) {
        console.error('Error with user saving in kb:', error);
        throw error;
    }
};


const find_user_by_session = async (
    session: string, 
    keynodes: Record<string, ScAddr>
) => {
    // check if user with such session exists in knowledge base
    const session_link = await client.searchLinksByContents([session])[0];
    if (session_link){
        const template = new ScTemplate();
        const component_user = '_user'
        const component_session = '_auth_session'
        template.quintuple(
            [ScType.VarNode, component_user],
            ScType.VarCommonArc,
            [session_link, component_session],
            ScType.VarPermPosArc,
            keynodes['nrel_auth_session'],
        );
        template.triple(
            keynodes['users'],
            ScType.VarPermPosArc,
            [ScType.VarNode, component_user],
        )

        const search_result = await client.searchByTemplate(template);
        
        if (search_result.length){
            const user_node = search_result[0].get(component_user)
            console.log(
                'User with auth_session: ', 
                session,
                'already exists!'
            )
            return user_node
        }
    }
    return null
}
