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
  background-color: rgba(0, 0, 0, 0.5);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 9999;

  .popup{
    background-color: rgba(255, 255, 255, 1);
    width: 35%;
    height: 80%;
    display: flex;
    flex-direction: column;
    padding: 10px;
    color: black;
    border: 1px solid gray;
    border-radius: 20px;
    position:relative;
    
    h2{
        font-weight: bold;
        margin-top: 10px;
    }
    
    h4{
        padding-left: 10px;
    }

    .numdiv{
        display: flex; 
        justify-content: center; 
        align-items: center;
    }

    .numdiv p{
        width: 40px;
        height: 40px;
        border-radius: 40px;
        color: white;
        display: flex;
        justify-content: center;
        align-items: center;
        font-size: 20px;
        font-weight: bold;
        
    }
    
    .state-1{
        margin-right: 40px;
        background: #88AFE2;
        border: 1px solid #88AFE2;
    }
    
    .state-2{
        background: #3C547B;
        border: 1px solid #3C547B;
    }

    .state-3{
        margin-right: 40px;
        background: #3C547B;
        border: 1px solid #3C547B;
    }
    
    .state-4{
        background: rgb(203, 203, 203);
        border: 1px solid rgb(203, 203, 203);
    }

    input{
        width: 96%;
        height: 4vh;
        border-radius: 20px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid gray;
        margin-bottom: 7px;
        padding-left: 10px;
    }

    input::placeholder {
        color: black;
    }

    .close_button{
        color: black;
        background: none;
        border: none;
        padding: 0;
        font-size: 30px;
        position: absolute;
        right: 20px;
        top: 16px;
        width: 30px;
        height: 30px;
        
    }

    .form{
        padding-left: 10px;
    }

    .button{
        color: white;
        border-radius: 20px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        width: 94%;
        height: 6%;
        position: absolute;
        bottom: 10px;
        left: 3%;
    }

    .next{
        background-color: rgb(149 149 149);
        border: 1px solid rgb(149 149 149);
    }

    .save{
        background-color: #3C547B;
        border: 1px solid #3C547B;
    }

    .chips{
        width: 96%;
    }

    .chip-div{
        display: flex;
    }

    .chip{
        background-color: rgba(255, 255, 255, 1);
        color: black;
        border-radius: 20px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid gray;
        width: 95%;
        padding-left: 10px;
        margin-left: 10px;
        margin-bottom: 5px;
        height: 4vh;
        display: flex;
        align-items:center;
    }

    .chip-delete{
        background-color: rgba(255, 255, 255, 1);
        color: gray;
        border-radius: 25px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid gray;
        height: 4vh;
        width: 4vh;
        margin-left: 5px;
        display: flex;
        align-items:center;
        justify-content: center;
    }

    .chip-input{
        width: 98%;
        height: 4vh;
        border-radius: 20px;
        box-shadow: 10px 4px 15px rgb(0 0 0 / 25%);
        border: 1px solid gray;
        margin-left: 10px;
        margin-bottom: 10px;

        &::placeholder{
            color: gray;
        }
    }

  }
`;