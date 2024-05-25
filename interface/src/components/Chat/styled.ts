import styled from 'styled-components';

export const WrapperGlobalSpinner = styled.div`
    height: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
`;

export const Wrapper = styled.div`
    height: 100%;
    display: flex;
    flex-direction: column;
    justify-content: space-between;
    background: white;
    box-sizing: border-box;
    box-shadow: 0px 4px 15px rgba(0, 0, 0, 0.25);
    backdrop-filter: blur(10px);
    border-radius: 15px;
`;
export const SearchBar = styled.div`
    display: flex;
    justify-content: flex-end;
    background: rgba(99, 132, 102, 0.43);
`;
export const Main = styled.div`
    height: 100%;
    position: relative;
    padding-top: 16px;
    display: flex;
    flex-direction: column;
    overflow: auto;
    color: rgba(0, 0, 0, 0);
    transition: color 0.3s ease;
    &::-webkit-scrollbar,
    &::-webkit-scrollbar-thumb {
        width: 12px;
        border-radius: 13px;
        background-clip: padding-box;
        border: 4px solid transparent;
    }
    &::-webkit-scrollbar-thumb {
        box-shadow: inset 0 0 0 10px;
    }
    &:hover {
        color: rgba(122, 142, 137, 1);
    }
`;
export const WrapperSpinner = styled.div`
    width: 142px;
    height: 31px;
    margin: auto;
    display: flex;
    padding: 8px 12px;
    margin-bottom: 12px;
    background: #daeddc;
    border-radius: 3px;
`;
export const FetchingSpinnerText = styled.div`
    margin-left: 6px;
    font-family: 'Roboto';
    font-style: normal;
    font-weight: 400;
    font-size: 12px;
    line-height: 14px;
    color: #707070;
`;

export const AnswerSpinnerText = styled.div`
    margin-left: 8px;
    font-family: 'Robot';
    font-style: normal;
    font-weight: 400;
    font-size: 14px;
    line-height: 18px;
    color: #949494;
`;
export const ArrowWrapper = styled.div`
    position: absolute;
    bottom: 80px;
    right: 13px;
    width: 50px;
    height: 50px;
    display: flex;
    justify-content: center;
    align-items: center;
    border-radius: 50%;
    background: #f9f3f0;
    z-index: 5;
    cursor: pointer;
    &:hover {
        background: #e4e0dd;
    }
`;
export const WrapperIcon = styled.div`
    width: 22px;
    height: 25px;
`;
export const WrapperAgentAnswer = styled.div`
    height: 25px;
    display: flex;
`;
export const WrapperWaitingSpinner = styled.div`
    display: flex;
    align-items: center;
`;
export const WrapperFooter = styled.div`
    display: flex;
`;
export const Footer = styled.div`
    margin-left: 9px;
    margin-right: 12px;
    padding-bottom: 9px;
`;
export const FooterInput = styled.input`
    width: 100%;
    padding: 15px 15px 15px 18px;
    box-shadow: 2px 2px 2px rgba(0, 0, 0, 0.25);
    border: 2px solid rgba(100, 150, 150, 0.2);
    border-radius: 60px;
    outline: 0;
    font-family: 'Roboto';
    font-style: normal;
    font-weight: 400;
    font-size: 16px;
    line-height: 10px;
    color: #000000;
`;
export const FooterSend = styled.button`
    padding: 13px 12px 13px 6px;
    margin-left: 12px;
    border-radius: 50%;
    box-shadow: 1.44px 1.44px 1.44px rgba(0, 0, 0, 0.25);
    border: 2px solid rgba(100, 150, 150, 0.2);
    cursor: pointer;
    &:hover {
        background: #849299;
    }
    &:hover path {
        stroke: white;
    }
    &:active {
        box-shadow: 0px 0px 10px #627599;
    }
`;
export const WrapperSendIcon = styled.div`
    width: 33px;
    height: 27px;
`;

export const MicroButton = styled.button<{ isSpeaking: boolean }>`
    padding: 13px 12px 13px 6px;
    margin-left: 12px;
    border-radius: 50%;
    box-shadow: 1.44px 1.44px 1.44px rgba(0, 0, 0, 0.25);
    background-color: ${p => p.isSpeaking ? `#c6c6c6` : `#ece5e5`};
    border: 2px solid rgba(100, 150, 150, 0.2);
    cursor: pointer;
    &:hover {
        background: #849299;
    }
    &:hover path {
        stroke: white;
    }
    &:active {
        box-shadow: 0px 0px 10px #627599;
    }
`;
