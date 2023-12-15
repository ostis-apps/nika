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
import { ScEventParams, ScEventType, ScTemplate, ScType } from "ts-sc-client";
import { client } from "@api";

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

        const [mainBgColor, setMainBgColor] = useState<string>('#ffffff');
        const [inputBgColor, setInputBgColor] = useState<string>('#ffffff');
        const [buttonBgColor, setButtonBgColor] = useState<string>('#ffffff');
        const [buttonActiveColor, setButtonActiveColor] = useState<string>('#ffffff');
        const [inputTextColor, setInputTextColor] = useState<string>('#ffffff');
        const funcChange = [setMainBgColor, setInputBgColor, setButtonBgColor, setInputTextColor, setButtonActiveColor]

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

        async function fetchColorValue() {
            const conceptMain = 'concept_data_output_component';
            const conceptDataInputComponent = 'concept_data_input_component';
            const conceptButton = 'concept_button'
            const conceptActiveButton = 'concept_active_button'
            const conceptInputText = 'concept_input_text'
            const componentColor = 'nrel_component_color';
        
            const baseKeynodes = [
                { id: conceptMain, type: ScType.NodeConstClass },
                { id: conceptDataInputComponent, type: ScType.NodeConstClass },
                { id: conceptButton, type: ScType.NodeConstClass },
                { id: conceptInputText, type: ScType.NodeConstClass },
                { id: conceptActiveButton, type: ScType.NodeConstClass },
            ];
        
            const helpKeynodes = [
                { id: componentColor, type: ScType.NodeConstNoRole },
            ];
        
            const colorAlias = '_color';
            const componentAlias = '_component'
             
            const keynodes = await client.resolveKeynodes(baseKeynodes);
            const hKeynodes = await client.resolveKeynodes(helpKeynodes);
        
            for (var i = 0; i < baseKeynodes.length; i++) {
                const template = new ScTemplate();
                template.triple(
                    keynodes[baseKeynodes[i].id],
                    ScType.EdgeAccessVarPosPerm,
                    [ScType.NodeVar, componentAlias],
                );
                template.tripleWithRelation(
                    componentAlias,
                    ScType.EdgeDCommonVar,
                    [ScType.LinkVar, colorAlias],
                    ScType.EdgeAccessVarPosPerm,
                    hKeynodes[componentColor],
                );
                const resultColorLink = await client.templateSearch(template);
                
                if (resultColorLink.length) {
                    const colorLink = resultColorLink[0].get(colorAlias);
                    const resultColor = await client.getLinkContents([colorLink]);
                    if (resultColor.length) {
                        let color = resultColor[0].data;
                        funcChange[i](color as any);
                        const eventParams = new ScEventParams(colorLink, ScEventType.ChangeContent, fetchColorValue);
                        await client.eventsCreate([eventParams]); 
                    }
                }    
            }
        }

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

        useEffect(() => {
            fetchColorValue();
        }, []);
    
        const mainStyles = {
            background: mainBgColor,
        };

        const inputStyles = {
            background: inputBgColor,
            color: inputTextColor,
        };

        const buttonStyles = {
            background: buttonBgColor,
            stroke: buttonActiveColor,
        };

        return (
            <Wrapper className={className} style={ mainStyles }>
                <SearchBar/>
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
                            style={ inputStyles }
                            autoFocus={true}
                            ref={inputRef}
                            value={messageInput}
                            onChange={onInputChange}
                            onKeyPress={onInputKeyDown}
                            type="text"
                            placeholder={textPlaceholder[hookLanguage]}
                        />
                        <FooterSend onClick={onButtonClick} type="submit" style={ buttonStyles }>
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
