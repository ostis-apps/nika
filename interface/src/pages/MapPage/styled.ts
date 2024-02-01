import styled from 'styled-components';
import { ChatPageWrapper } from '@components/ChatPageWrapper';

export const Wrapper = styled(ChatPageWrapper)`
    display: block;
`;

export const ChatWrapper = styled.div`
    grid-area: chat;
`;

export const NavLink = styled.a`
    display: flex;
    position: absolute;
    text-decoration: none;
    z-index: 1;
    top: 10px;
    left: 10px;
    gap: 12px;
    border-radius: 10px;
    align-items: center;
    height: 40px;
    color: white;
    margin-right: 10px;
    background: #413d3d;
    -webkit-box-shadow: 0px 0px 46px 21px rgba(34, 60, 80, 0.2);
    -moz-box-shadow: 0px 0px 46px 21px rgba(34, 60, 80, 0.2);
    box-shadow: 0px 0px 46px 21px rgba(34, 60, 80, 0.2);
    transition: all 0.5s ease-out;
    :hover {
        transform: translate(-1%, 0);
`;

export const Arrow = styled.div`
    border: solid rgb(255, 255, 255);
    border-width: 0 3px 3px 0;
    display: inline-block;
    padding: 3px;
    width: 5px;
    height: 5px;
    margin-right: 5px;
    transform: translate(100%, 0) rotate(135deg);
    -webkit-transform: translate(100%, 0) rotate(135deg);
    cursor: pointer;
`;

export const Linktitle = styled.div`
    color: rgb(255, 255, 255);
    font-weight: 700;
    padding: 0 10px 0 0;
`;

export const Menu = styled.div`
    display: block;
    position: absolute;
    z-index: 1000;
    right: -800px;
    top: 0;
    background: white;
    width: 700px;
    height: 100vh;
    overflow-y: scroll;
    overflow-x: hidden;
    border-radius: 15px 0px 0 15px;
    box-shadow: -8px 10px 89px -20px rgba(89, 89, 89, 0.75);
    -webkit-box-shadow: -8px 10px 89px -20px rgba(89, 89, 89, 0.75);
    -moz-box-shadow: -8px 10px 89px -20px rgba(89, 89, 89, 0.75);
    transition: all 0.5s ease-in-out;
`;
export const Information = styled.div`
    display: block;
    width: 100%;
`;
export const InformationHeader = styled.div`
    display: flex;
    min-height: 50px;
    background: #b3b3b3;
    background-repeat: 'no-repeat';
    background-position: 'center';
    background-size: 'cover';
    border-radius: 0 0 10px 10px;
    p {
        padding-left: 10px;
        font-weight: 700;
    }
`;
export const Inf = styled.div``;
export const CloseBtn = styled.div`
    position: absolute;
    top: 10px;
    right: 10px;
    z-index: 101;
    width: 30px;
    height: 30px;
    cursor: pointer;
    :before,
    :after {
        content: '';
        position: absolute;
        top: 50%;
        left: 50%;
        width: 20px;
        height: 2px;
        background-color: rgb(63, 63, 63);
        transform: translate(-50%, -50%) rotate(45deg);
    }
    :after {
        transform: translate(-50%, -50%) rotate(-45deg);
    }
`;
export const InformationText = styled.div`
    p {
        margin: 10px;
    }
`;

export const openStyle = {
    top: 0,
    right: 0,
};

export const nothinTitleStyles = {};

export const Loading = styled.div`
    position: absolute;
    width: 100%;
    height: 100%;
    z-index: 100;
    // background: rgba(0, 0, 0, 0.7);
    display: flex;
    justify-content: center;
    align-items: center;
`;

export const SpanLoader = styled.span`
    width: 150px;
    height: 150px;
    border-radius: 50%;
    position: relative;
    box-shadow: 0 0 30px 4px rgba(0, 0, 0, 0.5) inset, 0 5px 12px rgba(0, 0, 0, 0.15);
    overflow: hidden;

    :before,
    :after {
        content: '';
        position: absolute;
        width: 100%;
        height: 100%;
        border-radius: 45%;
        top: -40%;
        background-color: #fff;
        animation: wave 5s linear infinite;
    }

    :before {
        border-radius: 30%;
        background: rgba(255, 255, 255, 0.4);
        animation: wave 5s linear infinite;
    }
`;
