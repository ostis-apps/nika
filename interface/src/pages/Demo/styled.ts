import styled from 'styled-components';
import { ChatPageWrapper } from '@components/ChatPageWrapper';

export const Wrapper = styled(ChatPageWrapper)`
    display: block;
`;

export const ChatWrapper = styled.div`
    grid-area: chat;
`;

export const BackBtn = styled.a`
    z-index: 100;
    width: 30px;
    height: 30px;
    position: fixed;
    top: 5px;
    left: 5px;
    background: red;
    transition: all 0.5s ease;
    border-radius: 10px;
    :hover {
        opacity: 0.5;
    }
`;
