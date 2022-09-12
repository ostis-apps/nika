import styled from 'styled-components';
import { ChatPageWrapper } from '@components/ChatPageWrapper';

export const Wrapper = styled(ChatPageWrapper)`
    grid-template-areas:
        'chat scgViewer'
        'chat scgViewer';
    grid-gap: 11px;
`;

export const ChatWrapper = styled.div`
    grid-area: chat;
`;

export const SCgViewerWrapper = styled.div`
    grid-area: scgViewer;
    height: 100%;
    box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
    border-radius: 15px;
`;
