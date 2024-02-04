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
    margin: auto;
`;

export const WrapperLinks = styled.div`
    display: grid;
    grid-template-columns: 1fr 1fr;
    width: 100%;
    gap: 10px;
    @media screen and (max-width: 500px) {
        grid-template-columns: 1fr;
    }
`;

export const DeleteButton = styled.button`
    background: none;
    border: none;
    flex-basis: content;
    transition: all 0.5s ease;
    :hover {
        color: red;
    }
`;

export const WrLink = styled.div`
    background: rgba(256, 256, 256, 0.1);
    display: flex;
    border-radius: 10px;
    gap: 5px;
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
