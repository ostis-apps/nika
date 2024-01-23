import styled from 'styled-components';
import { ChatPageWrapper } from '@components/ChatPageWrapper';

export const Wrapper = styled(ChatPageWrapper)`
    display: block;
`;

export const ChatWrapper = styled.div`
    grid-area: chat;
`;

export const WrapperCircle = styled.div`
    position: absolute;
    top: -20%;
    right: -15%;
    height: 80vmin;
    width: 80vmin;
    border-radius: 50%;
    background: #538689;
`;

export const WrapperInf = styled.div`
    width: 100vw;
    height: 100vh;
    position: relative;
    top: 0;
    left: 0;
    backdrop-filter: blur(100px);
`;

export const ContainerInf = styled.div`
    max-width: 1000px;
    margin: auto;
    padding: 10px 0;
    width: 100%;
`;

export const WrapperHead = styled.div`
    display: flex;
    background: rgba(51, 52, 54, 50%);
    backdrop-filter: blur(10px);
    border-radius: 0 0 10px 10px;
    color: white;
`;

export const WrapperBtns = styled.div``;

export const WrapperWidget = styled.div``;

export const ContentHead = styled.div`
    display: flex;
    align-items: center;
    padding: 5px;
    justify-content: space-between;
    width: 100%;
`;

export const LangBtn = styled.button`
    cursor: pointer;
    background: none;
    border: none;
    transition: all 0.5s ease;
    :hover {
        opacity: 0.5;
    }
`;

export const UserName = styled.a`
    // Colour
    cursor: pointer;
    background: rgba(255, 255, 255, 0.3);
    backdrop-filter: blur(10px);
    padding: 5px;
    color: white;
    border-radius: 10px;
    font-size: 20px;
    font-weight: 900;
    transition: all 0.5s ease;
    :hover {
        opacity: 0.5;
    }
`;

export const BtnChat = styled.a``;
export const BtnSaved = styled.a``;
export const BtnGames = styled.a``;
