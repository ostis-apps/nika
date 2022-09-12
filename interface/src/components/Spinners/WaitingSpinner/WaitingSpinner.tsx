import styled, { keyframes } from 'styled-components';

const spin = keyframes`
   100% { transform: scale(1.4); }
`;
const Circle = styled.svg`
    animation: ${spin} 0.6s ease-in-out alternate infinite;
`;
const Circle2 = styled.svg`
    animation: ${spin} 0.6s ease-in-out alternate 0.2s infinite;
`;
const Circle3 = styled.svg`
    animation: ${spin} 0.6s ease-in-out alternate 0.4s infinite;
`;

const Wrapper = styled.div`
    display: flex;
    margin-left: 16px;
`;
const WrapperFirst = styled.div`
    margin: auto;
`;
const WrapperSecond = styled.div`
    padding-left: 4px;
    margin: auto;
`;
const WrapperThird = styled.div`
    padding-left: 4px;
    margin: auto;
`;

export const WaitingSpinner = () => (
    <Wrapper>
        <WrapperFirst>
            <Circle width="6" height="6" viewBox="0 0 6 6" fill="none" xmlns="http://www.w3.org/2000/svg">
                <circle cx="3" cy="3" r="3" fill="#ACACAC" />
            </Circle>
        </WrapperFirst>
        <WrapperSecond>
            <Circle2 width="6" height="6" viewBox="0 0 8 8" fill="none" xmlns="http://www.w3.org/2000/svg">
                <circle cx="4" cy="4" r="4" fill="#ACACAC" />
            </Circle2>
        </WrapperSecond>
        <WrapperThird>
            <Circle3 width="6" height="6" viewBox="0 0 10 10" fill="none" xmlns="http://www.w3.org/2000/svg">
                <circle cx="5" cy="5" r="5" fill="#ACACAC" />
            </Circle3>
        </WrapperThird>
    </Wrapper>
);
