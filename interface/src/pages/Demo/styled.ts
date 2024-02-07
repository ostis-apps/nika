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
        transform: translate(-5%, 0);
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
