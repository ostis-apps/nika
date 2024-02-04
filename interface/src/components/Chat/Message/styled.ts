import styled from 'styled-components';

export const TextWrapper = styled.div`
    display: flex;
`;
export const Text = styled.div`
    font-family: 'Montserrat';
    font-style: normal;
    font-weight: 700;
    font-size: 16px;
    line-height: 21px;
    color: #ffffff;
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
    font-family: 'Montserrat';
    font-style: normal;
    font-weight: 400;
    font-size: 12px;
    line-height: 14px;
    color: #ffffff;
    opacity: 0.8;
`;
export const WrapperLoadingIcon = styled.div`
    position: absolute;
    display: flex;
    right: -16px;
    bottom: 4px;
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
