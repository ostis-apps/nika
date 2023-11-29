import styled from 'styled-components';
import { ChatPageWrapper } from '@components/ChatPageWrapper';

export const Wrapper = styled(ChatPageWrapper)`
    display: block;
`;

export const ChatWrapper = styled.div`
    grid-area: chat;
`;

export const SCgViewerWrapper = styled.div`
    display: none;
`;
