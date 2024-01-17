import styled from 'styled-components';
import { ChatPageWrapper } from '@components/ChatPageWrapper';

export const Wrapper = styled(ChatPageWrapper)`
    display: flex;
    justify-content: center;
    align-items: center;
    position: relative;
    top: 0;
    max-height: 100% !important;
    left: 0;
    width: 100vw;
    height: 100vh;
    backdrop-filter: blur(100px);
`;

export const Circle = styled.div`
    position: absolute;
    bottom: -20%;
    right: -5%;
    height: 80vmin;
    width: 80vmin;
    border-radius: 50%;
    background: #538689;
`;

export const Circle1 = styled.div`
    position: absolute;
    top: 20%;
    left: 5%;
    height: 20vmin;
    width: 20vmin;
    border-radius: 50%;
    background: #538689;
`;

export const WrapperContent = styled.div`
    display: block;
    background: #ebfeff62;
    color: white;
    text-align: center;
    border-radius: 20px;
    width: 450px;

    backdrop-filter: blur(100px);
`;

export const Form = styled.form`
    display: flex;
    flex-direction: column;
    align-items: center;
    gap: 10px;
    color: white;
    padding: 20px 0 ;
`;

export const FormText = styled.h2`
    font-weight: 900; 
    color: white;
    padding: 20px 0 0 0;
`;

export const Input = styled.input`
    width: calc(100% - 30px);
    border: none;
    border-radius: 10px;
    outline: none;
    background: #333436;
    padding: 15px 10px;
    color: white;
    font-weight: 700;
    font-size: 14px;
`;

export const FormBtn = styled.button`
    padding: 15px 20px;
    outline: none;
    border-radius: 10px;
    cursor: pointer;
    font-weight: 900;
    background: #538689;
    color: white;
    font-size: 14px;
    transition: all .5s ease;
    border: none;
    :hover {
        opacity: 0.5;
    }
`;

export const Link = styled.a`
    text-decoration: underline;
    color: white;
    font-size: 14px;
    font-weight: 700;
    margin: 5px 0 0 0;
    :hover {
        color: white;
    }
`;

export const Error = styled.p`
    color: rgb(184, 67, 67);
    font-weight: 700;
    margin-bottom: 0;
`;