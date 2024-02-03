import styled from 'styled-components';
import { ChatPageWrapper } from '@components/ChatPageWrapper';

export const ChatWrapper = styled.div`
    grid-area: chat;
`;

export const Wrapper = styled.div`
    width: 100%;
    height: 100%;
`;

export const Container = styled.div`
    display: flex;
    flex-direction: column;
    max-width: 1100px;
    width: 100%;
    padding: 10px;
    align-items: center;
`;

export const WrapperLinks = styled.div`
    display: flex;
    flex-direction: column;
    align-items: center;
    width: 100%;
    gap: 10px;
`;

export const Link = styled.a`
    width: 100%;
`;
