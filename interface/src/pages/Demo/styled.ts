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
    box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
    border-radius: 15px;
    height: 100%;
    display: flex;
    flex-direction: column;
    justify-content: space-around;
    align-items: center;

    .button{
        background-color: rgba(255, 255, 255, 1);
        color: black;
        border-radius: 10px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid black;
        width: 30%;
        height: 6%;
    }
`;

export const OurWrapper = styled.div`
    position: absolute;
    bottom: 7.9vh;
    left: 50.5%;
    box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
    border-radius: 15px;
    height: 31%;
    width: 48.3%;

    display: flex;
    flex-direction: column;
    justify-content: space-around;
    align-items: center;
    
    color: black;
    
    .input{ 
    width: 90%;
    height: 4vh;
    border-radius: 10px;
    box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
    }
`;

export const PopupWrapper = styled.div`
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.5); /* Прозрачный цвет фона */
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 9999; /* Установка высокого значения z-index для отображения поверх других элементов */

  .popup{
    background-color: rgba(255, 255, 255, 1);
    width: 40%;
    height: 60%;
    display: flex;
    flex-direction: column;
    align-items: center;
    color: black;
    border: 2px solid black;
    border-radius: 12px;
    position:relative;
    
    .input{
        width: 90%;
        height: 4vh;
        border-radius: 10px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid black;
        margin-bottom: 7px;

        &::placeholder{
            color: gray;
        }
    }

    .textarea{
        width: 90%;
        height: 40%;
        resize: none;
        border-radius: 10px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid black;

        &::placeholder{
            color: gray;
        }
    }

    .button{
        background-color: rgba(255, 255, 255, 1);
        color: black;
        border-radius: 10px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid black;
        width: 30%;
        height: 6%;
        position: absolute;
        bottom: 10px;
    }

    .chips{
        width: 90%;
    }

    .chip-div{
        display: flex;
    }

    .chip{
        background-color: rgba(255, 255, 255, 1);
        color: black;
        border-radius: 10px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid black;
        width: 100%;
        padding-left: 20px;
        margin-bottom: 5px;
        height: 25px;
    }

    .chip-delete{
        background-color: rgba(255, 255, 255, 1);
        color: black;
        border-radius: 25px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid black;
        height: 25px;
        width: 25px;
        text-align: center;
        margin-left: 5px;
    }

    .chip-input{
        width: 100%;
        height: 4vh;
        border-radius: 10px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid black;
        margin-top: 5px;
        margin-bottom: 10px;

        &::placeholder{
            color: gray;
        }
    }

  }
`;