import { useLanguage } from '@hooks/useLanguage';
import styled from 'styled-components';
interface IProps {
    date: string;
}
const Wrapper = styled.div`
    width: 140px;
    margin: auto;
    margin-top: 8px;
    margin-bottom: 16px;
    padding: 6px 10px;
    background: #8fc1cf;
    border-radius: 10px;
    color: black;
    font-weight: 400;
    font-size: 14px;
    line-height: 16px;
`;
const DateInfo = styled.div`
    align-items: center;
    text-align: center;
`;

const months = {
    en: {
        1: 'january',
        2: 'february',
        3: 'march',
        4: 'april',
        5: 'mai',
        6: 'june',
        7: 'july',
        8: 'august',
        9: 'september',
        10: 'october',
        11: 'november',
        12: 'december',
    },
    ru: {
        1: 'января',
        2: 'февраля',
        3: 'марта',
        4: 'апреля',
        5: 'мая',
        6: 'июня',
        7: 'июля',
        8: 'августа',
        9: 'сентября',
        10: 'октября',
        11: 'ноября',
        12: 'декабря',
    },
};

export const Date = ({ date }: IProps) => {
    const hookLanguage = useLanguage();
    const [year, month, day] = date.toString().split('.');
    const currentMonth = months[hookLanguage];

    return (
        <>
            <Wrapper>
                <DateInfo>{`${day} ${currentMonth[Number(month)]} ${year}`}</DateInfo>
            </Wrapper>
        </>
    );
};
