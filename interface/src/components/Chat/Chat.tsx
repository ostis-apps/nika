import {
    useRef,
    PropsWithChildren,
    useState,
    useEffect,
    ChangeEvent,
    KeyboardEvent,
    forwardRef,
    useMemo,
    useCallback,
} from 'react';
import {
    Wrapper,
    SearchBar,
    Main,
    WrapperSpinner,
    FetchingSpinnerText,
    Footer,
    FooterInput,
    FooterSend,
    ArrowWrapper,
    WrapperGlobalSpinner,
    WrapperWaitingSpinner,
    AnswerSpinnerText,
    WrapperSendIcon,
    WrapperIcon,
    WrapperAgentAnswer,
    WrapperFooter,
} from './styled';
import { ReactComponent as SendIcon } from '@assets/icon/send-icon.svg';
import { ReactComponent as ArrowIcon } from '@assets/icon/arrowBackToLastMessage.svg';
import { Spinner } from '@components/Spinners/LoadSpinner';
import { WaitingSpinner } from '@components/Spinners/WaitingSpinner';
import { refSetter, throttle } from '@utils';
import { useLanguage } from '@hooks/useLanguage';
import { ScAddr } from 'ts-sc-client';
import { getUserSettings } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import styled from 'styled-components';

interface IProps {
    onSend: (message: string) => void;
    onFetching: () => Promise<boolean | undefined>;
    className?: string;
    isLoading?: boolean;
    isAgentAnswer?: boolean;
}
const textPlaceholder = {
    en: 'What Ragneda can?',
    ru: 'Что умеет Рагнеда?',
};
const textLoad = {
    en: 'Load messages',
    ru: 'Загрузка сообщений',
};
const textAgentAnswer = {
    en: 'typing',
    ru: 'Печатаю',
};

export const Chat = forwardRef<HTMLDivElement, PropsWithChildren<IProps>>(
    ({ onSend, onFetching, isAgentAnswer, children, className, isLoading: isFetchingChatLoading }, chatRef) => {
        const [messageInput, setMessageInput] = useState('');

        const [showArrow, setShowArrow] = useState(false);
        const [scrollHappened, setScrollHappened] = useState(false);
        const [shouldLoadMoreMessages, setShouldLoadMoreMessages] = useState(false);
        const [hasMoreMessages, setHasMoreMessages] = useState(true);
        const [isLoading, setIsLoading] = useState(false);
        const [beforeFetchingScrollHeight, setBeforeFetchingScrollHeight] = useState<number | null>(null);
        const [userTheme, setUserTheme] = useState<String>('');
        const [accentColor, setAccentColor] = useState<String>('');

        const inputRef = useRef<HTMLInputElement | null>(null);
        const mainRef = useRef<HTMLDivElement>(null);
        const footerRef = useRef<HTMLDivElement>(null);

        const sendMessage = useCallback(
            (text: string) => {
                onSend(text);
                setMessageInput('');
            },
            [onSend],
        );
        const throttleSendMessage = useMemo(() => throttle(sendMessage, 500), [sendMessage]);

        const onButtonClick = () => {
            if (!messageInput) {
                throttleSendMessage(textPlaceholder[hookLanguage]);
            } else {
                throttleSendMessage(messageInput);
            }
        };

        const scrollToLastMessage = () => mainRef.current?.lastElementChild?.scrollIntoView(false);

        const onArrowClick = () => scrollToLastMessage();

        const penultimateEl = () => {
            const arrHTMLColl = mainRef.current?.children;
            if (!arrHTMLColl) return;
            const arr = Array.from(arrHTMLColl);
            return arr[arr.length - 2];
        };

        const checkMesssageInVisible = (el: Element | null | undefined) => {
            const elementTop = el?.getBoundingClientRect().top;
            const inputTop = footerRef.current?.lastElementChild?.getBoundingClientRect().top;

            if (!elementTop || !inputTop) return false;
            return elementTop - inputTop > -10;
        };

        const checkLastMesssageInVisible = () => checkMesssageInVisible(mainRef.current?.lastElementChild);

        const onScroll = (e: React.UIEvent<HTMLDivElement>) => {
            setScrollHappened(true);
            setShowArrow(checkLastMesssageInVisible());

            const currentScrollFromTop = e.currentTarget.scrollTop;
            if (currentScrollFromTop === 0) setShouldLoadMoreMessages(true);
        };

        const onInputChange = (e: ChangeEvent<HTMLInputElement>) => {
            setMessageInput(e.target.value);
        };

        const onInputKeyDown = (e: KeyboardEvent<HTMLInputElement>) => {
            if (e.key === 'Enter') {
                if (!messageInput) {
                    throttleSendMessage(textPlaceholder[hookLanguage]);
                } else {
                    throttleSendMessage(messageInput);
                }
            }
        };
        const hookLanguage = useLanguage();

        useEffect(() => {
            const LOADING_HEIGHT = 43;
            const heightOnScroll = mainRef.current?.scrollHeight;

            if (!heightOnScroll || !beforeFetchingScrollHeight) return;

            const diff = heightOnScroll - beforeFetchingScrollHeight;

            if (isLoading) return;

            mainRef.current.scroll(0, diff - LOADING_HEIGHT);
            setBeforeFetchingScrollHeight(null);
        }, [beforeFetchingScrollHeight, children, isLoading]);

        useEffect(() => {
            (async () => {
                const cookie = new Cookie();
                const userAddr = cookie.get('userAddr')
                    ? new ScAddr(parseInt(String(cookie.get('userAddr'))))
                    : new ScAddr(0);

                const userSettings = await getUserSettings(userAddr);
                setUserTheme(userSettings['nrel_theme']);
                setAccentColor(userSettings['nrel_accent_color']);
                if (shouldLoadMoreMessages && hasMoreMessages && !isLoading) {
                    setIsLoading(true);
                    setBeforeFetchingScrollHeight(mainRef.current?.scrollHeight || null);
                    setShouldLoadMoreMessages(false);

                    const shouldEnd = await onFetching();

                    setHasMoreMessages(!shouldEnd);
                    setIsLoading(false);
                }
            })();
        }, [hasMoreMessages, shouldLoadMoreMessages, isLoading, onFetching]);

        useEffect(() => {
            if (checkMesssageInVisible(mainRef.current?.lastElementChild) && !scrollHappened) {
                scrollToLastMessage();
            }
        }, [children, scrollHappened]);

        useEffect(() => {
            if (!checkMesssageInVisible(penultimateEl())) {
                scrollToLastMessage();
            }
        }, [children]);

        useEffect(() => {
            const empty = !messageInput.trim();
            if (empty) setMessageInput('');
        }, [messageInput]);

        const Loading = styled.div`
            position: absolute;
            width: 100%;
            height: 100%;
            z-index: 100;
            background: rgba(0, 0, 0, 0.7);
            display: flex;
            justify-content: center;
            align-items: center;
            flex-direction: column;
            gap: 5px;
        `;

        const SpanLoader = styled.span`
            width: 150px;
            height: 150px;
            border-radius: 50%;
            position: relative;
            box-shadow: 0 0 30px 4px rgba(0, 0, 0, 0.5) inset, 0 5px 12px rgba(0, 0, 0, 0.15);
            overflow: hidden;

            :before,
            :after {
                content: '';
                position: absolute;
                width: 100%;
                height: 100%;
                border-radius: 45%;
                top: -40%;
                background-color: #fff;
                animation: wave 5s linear infinite;
            }

            :before {
                border-radius: 30%;
                background: rgba(255, 255, 255, 0.4);
                animation: wave 5s linear infinite;
            }
        `;

        return (
            <Wrapper style={userTheme == 'dark' ? {} : { background: accentColor as string }} className={className}>
                <SearchBar />
                <Main ref={refSetter(mainRef, chatRef)} onScroll={onScroll}>
                    {isLoading && (
                        <WrapperSpinner>
                            <Spinner size={12} />
                            <FetchingSpinnerText>{textLoad[hookLanguage]}</FetchingSpinnerText>
                        </WrapperSpinner>
                    )}

                    {children}
                    {isFetchingChatLoading && (
                        <Loading style={{ opacity: 1, background: 'rgba(0, 0, 0, 0.8)' }}>
                            <SpanLoader style={{ background: accentColor as string }}></SpanLoader>
                        </Loading>
                    )}
                </Main>
                {showArrow && (
                    <ArrowWrapper onClick={onArrowClick}>
                        <WrapperIcon>
                            <ArrowIcon />
                        </WrapperIcon>
                    </ArrowWrapper>
                )}
                <Footer ref={footerRef}>
                    <WrapperAgentAnswer>
                        {isAgentAnswer && (
                            <WrapperWaitingSpinner>
                                <WaitingSpinner />
                                <AnswerSpinnerText>{textAgentAnswer[hookLanguage]}</AnswerSpinnerText>
                            </WrapperWaitingSpinner>
                        )}
                    </WrapperAgentAnswer>
                    <WrapperFooter>
                        <FooterInput
                            autoFocus={true}
                            ref={inputRef}
                            value={messageInput}
                            onChange={onInputChange}
                            onKeyPress={onInputKeyDown}
                            type="text"
                            placeholder={textPlaceholder[hookLanguage]}
                        />
                        <FooterSend style={{ background: accentColor as string }} onClick={onButtonClick} type="submit">
                            <WrapperSendIcon>
                                <SendIcon />
                            </WrapperSendIcon>
                        </FooterSend>
                    </WrapperFooter>
                </Footer>
            </Wrapper>
        );
    },
);
Chat.displayName = 'Chat';
