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

interface IProps {
    onSend: (message: string) => void;
    onFetching: () => Promise<boolean | undefined>;
    className?: string;
    isLoading?: boolean;
    isAgentAnswer?: boolean;
}
const textPlaceholder = {
    en: 'What Nika can?',
    ru: 'Что умеет Ника?',
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

        return (
            <Wrapper className={className}>
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
                        <WrapperGlobalSpinner>
                            <Spinner />
                        </WrapperGlobalSpinner>
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
                        <FooterSend onClick={onButtonClick} type="submit">
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
