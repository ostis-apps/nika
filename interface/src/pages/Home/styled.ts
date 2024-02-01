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

export const WrapperBtns = styled.div`
    display: flex;
    gap: 5px;
`;

export const BtnsContainer = styled.div`
    max-width: 1000px;
    gap: 5px;
    display: flex;
    flex-direction: column;
    width: 100%;
    margin: auto;
    padding: 5px;
`;

export const ContentHead = styled.div`
    display: flex;
    align-items: center;
    padding: 5px;
    justify-content: space-between;
    width: 100%;
`;

export const LangBtn = styled.a`
    cursor: pointer;
    background: none;
    border: none;
    transition: all 0.5s ease;
    :hover {
        opacity: 0.5;
    }
`;

export const UserName = styled.button`
    // Colour
    cursor: pointer;
    background: rgba(255, 255, 255, 0.3);
    backdrop-filter: blur(10px);
    padding: 5px;
    color: white;
    border: none;
    border-radius: 10px;
    font-size: 20px;
    font-weight: 900;
    transition: all 0.5s ease;
    :hover {
        opacity: 0.5;
    }
`;

export const BtnChat = styled.a`
    padding: 20px;
    text-align: center;
    width: 100%;
    background: rgba(51, 52, 54, 50%);
    backdrop-filter: blur(10px);
    color: white;
    font-weight: 700;
    border-radius: 10px;
    transition: all 0.5s ease;
    :hover {
        color: white;
        opacity: 0.5;
    }
`;
export const BtnSaved = styled.a`
    padding: 10px;
    text-align: center;
    flex-basis: content;
    background: rgba(51, 52, 54, 50%);
    backdrop-filter: blur(10px);
    color: white;
    font-weight: 700;
    border-radius: 10px;
    transition: all 0.5s ease;
    :hover {
        color: white;
        opacity: 0.5;
    }
`;
export const BtnGames = styled.a`
    padding: 20px;
    text-align: center;
    flex-basis: 100%;
    background: rgba(51, 52, 54, 50%);
    backdrop-filter: blur(10px);
    color: white;
    font-weight: 700;
    border-radius: 10px;
    transition: all 0.5s ease;
    :hover {
        color: white;
        opacity: 0.5;
    }
`;

export const WrapperWidget = styled.div`
    max-width: 1000px;
    width: 100%;
    margin: auto;
    display: flex;
    gap: 5px;
    max-height: 500px;
    height: 100%;
    padding: 5px;
`;

export const WidgetWeather = styled.div`
    display: flex;
    align-items: center;
    justify-content: center;
    font-weight: 700;
    width: 100%;
    border-radius: 10px;
    background: rgba(51, 52, 54, 50%);
    backdrop-filter: blur(10px);
`;
export const WidgetMap = styled.div`
    display: flex;
    align-items: center;
    justify-content: center;
    font-weight: 700;
    width: 100%;
    border-radius: 10px;
    background: rgba(51, 52, 54, 50%);
    backdrop-filter: blur(10px);
`;

export const langStyles = {
    width: '30px',
    height: '30px',
};

export const savedStyles = {
    height: '100%',
    width: '30px',
};

export const WrapperSaved = styled.div``;
