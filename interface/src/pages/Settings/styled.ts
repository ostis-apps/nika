import styled from 'styled-components';
import { ChatPageWrapper } from '@components/ChatPageWrapper';

export const Wrapper = styled(ChatPageWrapper)`
    display: block;
`;

export const ChatWrapper = styled.div`
    grid-area: chat;
`;

export const WrapperSettings = styled.div`
    display: flex;
    flex-direction: column;
    gap: 10px;
    align-items: center;
`;

export const Setting = styled.div`
    display: flex;
    justify-content: space-between;
    width: 100%;
    align-items: center;
    p {
        font-weight: 700;
        font-size: 16px;
        margin: 0;
    }
    select {
        background: none;
        border-radius: 10px;
        border: 1px solid white;
        padding: 5px;
    }
`;

export const Container = styled.div`
    max-width: 1010px;
    width: 100%;
    margin: auto;
    padding: 10px;
    text-align: center;
    color: white;
`;

export const SettingsText = styled.h2`
    font-weight: 900;
    color: white;
`;

export const SaveSettings = styled.button`
    width: fit-content;
    padding: 15px 20px;
    outline: none;
    border-radius: 10px;
    cursor: pointer;
    font-weight: 900;
    background: #538689;
    color: white;
    font-size: 14px;
    transition: all 0.5s ease;
    border: none;
    :hover {
        opacity: 0.5;
    }
`;
