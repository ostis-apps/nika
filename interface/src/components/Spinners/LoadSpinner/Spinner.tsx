import styled, { keyframes } from 'styled-components';

const spin = keyframes`
    0% {
      transform: rotate(0deg);
    }
    100% {
      transform: rotate(360deg);
    }
`;

const Svg = styled.svg`
    animation: ${spin} 1s linear infinite;
    path {
        fill: #638466;
    }
`;

const Wrapper = styled.div<{ spinnerSize: number }>`
    width: ${({ spinnerSize }) => spinnerSize}px;
    height: ${({ spinnerSize }) => spinnerSize}px;
`;

interface IProps {
    size?: number;
    className?: string;
}

export const Spinner = ({ size = 42, className }: IProps) => (
    <Wrapper className={className} spinnerSize={size}>
        <Svg viewBox="0 0 42 42" fill="none" xmlns="http://www.w3.org/2000/svg">
            <path
                d="M42 21C42 25.1534 40.7684 29.2135 38.4609 32.667C36.1534 36.1204 32.8736 38.812 29.0364 40.4015C25.1991 41.9909 20.9767 42.4068 16.9031 41.5965C12.8295 40.7862 9.08766 38.7861 6.15076 35.8492C3.21385 32.9123 1.2138 29.1705 0.403509 25.0969C-0.406781 21.0233 0.00908955 16.8009 1.59853 12.9636C3.18797 9.1264 5.87959 5.84665 9.33302 3.53914C12.7865 1.23163 16.8466 -4.95289e-08 21 0V3.73164C17.5846 3.73164 14.246 4.74441 11.4062 6.64188C8.56644 8.53935 6.35311 11.2363 5.04611 14.3917C3.73911 17.5471 3.39714 21.0192 4.06344 24.3689C4.72975 27.7186 6.3744 30.7956 8.78942 33.2106C11.2044 35.6256 14.2814 37.2703 17.6311 37.9366C20.9808 38.6029 24.4529 38.2609 27.6083 36.9539C30.7637 35.6469 33.4606 33.4336 35.3581 30.5938C37.2556 27.754 38.2684 24.4154 38.2684 21H42Z"
                fill="#638466"
            />
        </Svg>
    </Wrapper>
);
