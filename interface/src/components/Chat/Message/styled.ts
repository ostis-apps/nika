import styled from 'styled-components';

export const WrapperMessage = styled.div<{ isLeft: boolean }>`
    position: relative;
    width: fit-content;
    max-width: 65%;
    margin: 0px 18px 8px 16px;
    padding: 8px 8px 8px 8px;
    align-self: ${(props) => (props.isLeft ? 'start' : 'end')};
    background: ${(props) => (props.isLeft ? '#DFDBD0' : '#E2E9E7')};
    border-radius: 10px;
    word-break: break-word;

    &:first-child {
        margin-top: auto;
    }
`;
export const TextWrapper = styled.div`
    display: flex;
`;
export const Text = styled.div`
    font-family: 'Roboto';
    font-style: normal;
    font-weight: 400;
    font-size: 16px;
    line-height: 21px;
    color: #000000;
    text-align: justify;
`;
export const Info = styled.div`
    margin-top: 4px;
    display: flex;
    justify-content: space-between;
    align-items: center;
`;
export const Time = styled.div`
    display: flex;
    align-items: center;
    font-family: 'Roboto';
    font-style: normal;
    font-weight: 400;
    font-size: 12px;
    line-height: 14px;
    color: #696969;
`;
export const WrapperLoadingIcon = styled.div`
    position: absolute;
    display: flex;
    right: -16px;
    bottom: 4px;
`;

//компонент для кнопки озвучивания
export const SpeakButton = styled.button`
    position: absolute;
    right: 10px; /* сдвигаем кнопку левее, чтобы она была рядом с иконкой загрузки */
    bottom: 4px;
    background: transparent;
    border: none;
    cursor: pointer;
    color: #4a5875;
    font-size: 16px;

    &:hover {
        color: #2c3e50; /* цвет при наведении */
    }
`;

export const WrapperRebootError = styled.div`
    margin: -4px 16px 6px 0px;
    padding: 3px;
    display: flex;
    justify-content: end;
    align-items: center;
    align-self: flex-end;
    border-radius: 3px;
    cursor: pointer;
    &:hover {
        background: #daeddc;
    }
`;
export const TextReboot = styled.div`
    margin-left: 4px;
    padding-top: 2px;
    font-family: 'Roboto';
    font-style: normal;
    font-weight: 400;
    font-size: 12px;
    line-height: 15px;
    color: #4a5875;
`;

export const ImgWrapper = styled.div``;
export const Img = styled.img`
    height: auto;
    max-width: 100%;
`;
