import { routes } from '@constants';
import React, { useEffect, useState } from 'react';
import { YMaps, Map, Placemark } from 'react-yandex-maps';
import {
    Arrow,
    Linktitle,
    NavLink,
    Information,
    InformationHeader,
    Inf,
    CloseBtn,
    CloseBtnModal,
    InformationText,
    openStyle,
    Loading,
    SpanLoader,
    Error,
    SaveMap,
    WrapperSavingName,
    ModalName,
    NameInput,
    SaveNameButton,
    Line,
    Gradient,
    Menu,
} from './styled';
import { getFontSizeFromSettings, getUserSettings } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import { ScAddr, ScConstruction, ScTemplate, ScType, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { client } from '@api/sc';
import styled from 'styled-components';
import { overflow } from '@pages/Intro/styled';
const API_KEY = '5ae2e3f221c38a28845f05b6c5d9bf667efa63f94dcb0e435b058e95';

export const MapPage = () => {
    const cookie = new Cookie();
    const cookieUserAddr = cookie.get('userAddr')
        ? new ScAddr(parseInt(String(cookie.get('userAddr'))))
        : new ScAddr(0);

    const [openMenu, setOpenMenu] = useState<boolean>(false);

    const [coordinates, setCoordinates] = useState<String[][]>([[]]);
    const [xids, setXids] = useState<String[]>([]);
    const [coordCenter, setCoordCenter] = useState<String[]>([]);
    const [zoomValue, setZoomValue] = useState<Number>(12);

    const [accentColor, setAccentColor] = useState<string | undefined>('black');
    const [loadError, setLoadError] = useState<Boolean>(false);
    const [startPr, setStartPr] = useState<Boolean>(false);

    const [pageInform, setPageInform] = useState<String[][]>([]);
    const [activeTitle, setActiveTitle] = useState<String>('');
    const [activeXid, setActiveXid] = useState<String>('');
    const [activeDescription, setActiveDescription] = useState<string>('');
    const [imageStyles, setImageStyles] = useState<{}>({});
    const [imageTextStyles, setTextImageStyles] = useState<{}>({});

    const [modalSave, setModalSave] = useState<Boolean>(false);
    const [mapName, setMapName] = useState<String>('');

    const [liked, setLiked] = useState<Boolean>(false);
    const [index, setIndex] = useState<number>(0);

    const [userTheme, setUserTheme] = useState<String>('dark');
    const [useGradient, setUseGradient] = useState<Boolean>(false);

    const [params, setParams] = useState<{}>({});

    const getDataFromXids = async (xid: String) => {
        try {
            const url = `http://api.opentripmap.com/0.1/ru/places/xid/${xid}?apikey=${API_KEY}`;
            const response = await fetch(url);
            return response.json();
        } catch {
            return {};
        }
    };

    const des_list = async (urlXids: String[]) => {
        let pageInf: String[][] = [];
        let coords: String[][] = [];
        for (let ind = 0; ind < urlXids.length; ind++) {
            if (urlXids[ind]) {
                const result = await getDataFromXids(urlXids[ind]);

                console.log(result);
                let name = '',
                    description = '',
                    image = '',
                    coord = '';

                try {
                    name = result['name'];
                } catch {}

                try {
                    coord = `${result['point']['lat']},${result['point']['lon']}`;
                    coords.push(coord.split(','));
                } catch {}

                try {
                    description = `<p><b>Улица: </b>`;
                    if (result['address']['city_district']) description += `${result['address']['city_district']}, `;
                } catch {}

                try {
                    description += `${result['address']['road'] ? result['address']['road'] + ',' : ''} `;
                    description += `${result['address']['house_number'] ? result['address']['house_number'] : ''}`;
                } catch {}

                if (description == '<b>Улица: </b>') description = '<p>';

                try {
                    description += result['wikipedia_extracts']['html'];
                } catch {}

                try {
                    if (result['url'])
                        description += `<p style="disply: block;"><b>Сайт: </b><a style="text-decoration: underline;" href="${result['url']}">Ознакомиться</a></p>`;
                } catch {}

                try {
                    description += `<p><b><a style="text-align: center;" href="${result['wikipedia']}">Больше информации</a></b></p>`;
                } catch {}

                description += '</p>';

                try {
                    image = result['preview']['source'];
                } catch {}

                pageInf.push([urlXids[ind], name, description, image]);
            }
        }
        return [pageInf, coords];
    };

    useEffect(() => {
        (async () => {
            // Get Settings
            const userSettings = await getUserSettings(cookieUserAddr);
            setParams(userSettings);
            setAccentColor(userSettings['nrel_accent_color']);
            setUserTheme(userSettings['nrel_theme']);

            const queryParams = new URLSearchParams(window.location.search);
            const coordinatesString = (queryParams.get('coord') ?? '53.931986, 27.668021').split(',');
            setCoordCenter([coordinatesString[0], coordinatesString[1]]);

            if (queryParams.get('type') == 'des_list') {
                const urlXids = (queryParams.get('xids') ?? '').split(',');
                const ans = await des_list(urlXids);
                setPageInform(ans[0]);
                setCoordinates(ans[1]);
                setStartPr(true);
            } else {
            }
        })();
    }, []);

    const checkLikedXid = async (xidF: String) => {
        const template = new ScTemplate();
        const baseKeynodes = [
            { id: 'nrel_saved_points', type: ScType.NodeConstNoRole },
            { id: 'nrel_point_xid', type: ScType.NodeConstNoRole },
        ];
        const keynodes = await client.resolveKeynodes(baseKeynodes);

        template.tripleWithRelation(
            cookieUserAddr,
            ScType.EdgeDCommonVar,
            [ScType.NodeVar, '_saved_points'],
            ScType.EdgeAccessVarPosPerm,
            keynodes['nrel_saved_points'],
        );
        template.triple('_saved_points', ScType.EdgeDCommonVar, [ScType.NodeVar, '_point']);
        template.tripleWithRelation(
            '_point',
            ScType.EdgeDCommonVar,
            [ScType.LinkVar, '_xid'],
            ScType.EdgeAccessVarPosPerm,
            keynodes['nrel_point_xid'],
        );
        const result = await client.templateSearch(template);

        if (result.length == 0) {
            console.log('I');
            return false;
        }

        for (let i = 0; i < result.length; i++) {
            const element = result[i];
            const xid = String((await client.getLinkContents([element.get('_xid')]))[0].data);
            console.log('>', xid);

            if (xid === xidF) {
                console.log('find');
                return true;
            }
            return false;
        }

        return false;
    };

    const open = (index: number) => async (event: React.MouseEvent<HTMLDivElement>) => {
        setLoadError(false);
        setUseGradient(false);
        const desireCoordinates = index;
        setActiveTitle(pageInform[index][1]);
        setActiveDescription(pageInform[index][2] as string);
        setActiveXid(pageInform[index][0]);
        setIndex(index);

        setLiked(await checkLikedXid(pageInform[index][0]));

        if (pageInform[index][3] != '') {
            const st = {
                backgroundImage: `url(${pageInform[index][3]})`,
                backgroundRepeat: 'no-repeat',
                backgroundPosition: 'center',
                minHeight: '350px',
                alignItems: 'end',
                backgroundSize: '100%',
                overflow: 'hidden',
            };
            console.log(st);
            setImageStyles(st);
            setUseGradient(true);
            setTextImageStyles({
                marginBottom: '10px',
                color: 'white',
            });
        } else {
            setImageStyles({
                background: '#00000010',
                backdropFilter: 'opacity(30%)',
            });
            setTextImageStyles({ color: 'black', marginRight: '50px' });
        }
        setOpenMenu(true);
    };

    const close = () => {
        setOpenMenu(false);
    };

    const saveMap = async () => {
        const template = new ScTemplate();
        const baseKeynodes = [
            { id: 'nrel_saved', type: ScType.NodeConstNoRole },
            { id: 'nrel_main_idtf', type: ScType.NodeConstNoRole },
            { id: 'nrel_link', type: ScType.NodeConstNoRole },
        ];
        const keynodes = await client.resolveKeynodes(baseKeynodes);

        template.tripleWithRelation(
            cookieUserAddr,
            ScType.EdgeDCommonVar,
            [ScType.NodeVar, '_saved'],
            ScType.EdgeAccessVarPosPerm,
            keynodes['nrel_saved'],
        );
        const result = await client.templateSearch(template);
        if (result.length == 0) return;

        if (mapName == '') setMapName('Карта');

        const savedAddr = result[0].get('_saved');
        console.log(savedAddr);

        const construction = new ScConstruction();
        construction.createNode(ScType.NodeConst, 'saved_maps');
        construction.createEdge(ScType.EdgeDCommonConst, savedAddr, 'saved_maps');
        construction.createLink(
            ScType.LinkConst,
            new ScLinkContent(mapName as string, ScLinkContentType.String),
            'name_link',
        );
        construction.createLink(
            ScType.LinkConst,
            new ScLinkContent(window.location.href as string, ScLinkContentType.String),
            'href_link',
        );
        construction.createEdge(ScType.EdgeDCommonConst, 'saved_maps', 'name_link', 'edge1');
        construction.createEdge(ScType.EdgeDCommonConst, 'saved_maps', 'href_link', 'edge2');
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['nrel_main_idtf'], 'edge1');
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['nrel_link'], 'edge2');
        await client.createElements(construction);
        setMapName('');
        setModalSave(false);
    };

    const openModal = () => {
        setModalSave(true);
    };

    const closeModal = () => {
        setModalSave(false);
    };

    const updateInputName = (e) => {
        console.log(e.target.value);
        setMapName(e.target.value);
    };

    const savePoint = async () => {
        const template = new ScTemplate();
        const baseKeynodes = [
            { id: 'nrel_saved_points', type: ScType.NodeConstNoRole },
            { id: 'nrel_point_xid', type: ScType.NodeConstNoRole },
            { id: 'nrel_point_coord', type: ScType.NodeConstNoRole },
        ];
        const keynodes = await client.resolveKeynodes(baseKeynodes);

        if (liked) {
            template.tripleWithRelation(
                cookieUserAddr,
                ScType.EdgeDCommonVar,
                [ScType.NodeVar, '_saved_points'],
                ScType.EdgeAccessVarPosPerm,
                keynodes['nrel_saved_points'],
            );
            template.triple('_saved_points', ScType.EdgeDCommonVar, [ScType.NodeVar, '_point']);
            template.tripleWithRelation(
                '_point',
                ScType.EdgeDCommonVar,
                [ScType.LinkVar, '_xid'],
                ScType.EdgeAccessVarPosPerm,
                keynodes['nrel_point_xid'],
            );
            const result = await client.templateSearch(template);

            if (result.length == 0) return;

            result.forEach(async (element) => {
                const xidAddr = element.get('_xid');
                const xid = String((await client.getLinkContents([xidAddr]))[0].data);

                if (xid == activeXid) {
                    await client.deleteElements([xidAddr]);
                    setLiked(false);
                }
            });
        } else {
            template.tripleWithRelation(
                cookieUserAddr,
                ScType.EdgeDCommonVar,
                [ScType.NodeVar, '_saved_points'],
                ScType.EdgeAccessVarPosPerm,
                keynodes['nrel_saved_points'],
            );
            const result = await client.templateSearch(template);
            if (result.length == 0) return;

            const savedAddr = result[0].get('_saved_points');
            console.log(savedAddr);

            const construction = new ScConstruction();
            construction.createNode(ScType.NodeConst, 'point');
            construction.createEdge(ScType.EdgeDCommonConst, savedAddr, 'point');
            construction.createLink(
                ScType.LinkConst,
                new ScLinkContent(activeXid as string, ScLinkContentType.String),
                'point_xid',
            );
            construction.createLink(
                ScType.LinkConst,
                new ScLinkContent(coordinates[index].join(',') as string, ScLinkContentType.String),
                'point_coord',
            );
            construction.createEdge(ScType.EdgeDCommonConst, 'point', 'point_xid', 'edge1');
            construction.createEdge(ScType.EdgeDCommonConst, 'point', 'point_coord', 'edge2');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['nrel_point_xid'], 'edge1');
            construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['nrel_point_coord'], 'edge2');

            await client.createElements(construction);
            setLiked(true);
        }
    };

    const SaveB = styled.div`
        padding: 10px;
        border-radius: 100px;
        cursor: pointer;

        path {
            fill: #00000050;
            transition: all 0.5s ease;
        }
        svg {
            transform: translate(1px, 1px);
            width: 50px;
            height: 50px;
        }
        :hover svg path {
            fill: red;
        }
    `;
    const Theme = styled.div`
        height: 100%;
        background: ${userTheme == 'dark' ? '#413d3d' : 'white'};
        * {
            color: ${userTheme == 'dark' ? 'white' : 'black'};
        }
    `;

    return (
        <>
            <NavLink href={routes.HOME} className="nav">
                <Arrow></Arrow>
                <Linktitle className="title">Назад</Linktitle>
            </NavLink>
            <SaveMap onClick={openModal}>+</SaveMap>
            <WrapperSavingName style={modalSave ? { zIndex: 1 } : { zIndex: -1 }}>
                <CloseBtnModal onClick={closeModal} style={{ zIndex: 11 }}></CloseBtnModal>
                <ModalName style={{ background: '#413d3d' }}>
                    <h2>Сохранить карту</h2>
                    <NameInput placeholder="Название" onChange={(e) => updateInputName(e)}></NameInput>
                    <SaveNameButton style={{ background: accentColor }} onClick={() => saveMap()}>
                        Сохранить
                    </SaveNameButton>
                </ModalName>
            </WrapperSavingName>
            <Loading
                style={
                    !startPr
                        ? { opacity: 1, background: 'rgba(0, 0, 0, 0.8)' }
                        : { opacity: 0, background: 'rgba(0, 0, 0, 0.8)', zIndex: 0 }
                }
            >
                <SpanLoader style={{ background: accentColor }}></SpanLoader>
                <Error style={loadError ? { opacity: 1 } : { opacity: 0 }}>Ошибка сети.</Error>
            </Loading>
            <YMaps>
                <Map defaultState={{ center: coordCenter, zoom: zoomValue }} width="100%" height="100%">
                    {coordinates.map((coordinate, index) => (
                        <Placemark key={index} geometry={coordinate} onClick={open(index)} />
                    ))}
                </Map>
            </YMaps>
            <Menu style={openMenu ? { right: 0 } : { right: '-800px' }} className="container_menu">
                <Theme>
                    <Loading style={!startPr ? { opacity: 1, zIndex: 100 } : { opacity: 0, zIndex: -1 }}>
                        <SpanLoader style={{ background: accentColor }}></SpanLoader>
                        <Error style={loadError ? { opacity: 1 } : { opacity: 0 }}>Ошибка сети.</Error>
                    </Loading>
                    <Information>
                        <InformationHeader style={imageStyles}>
                            <Inf style={imageTextStyles}>
                                <p
                                    style={{
                                        zIndex: 2,
                                        fontSize: getFontSizeFromSettings(params['nrel_font_size'], 'h'),
                                    }}
                                >
                                    {activeTitle}
                                </p>
                            </Inf>
                            <Gradient style={useGradient ? { opacity: 1 } : {}}></Gradient>
                            <CloseBtn onClick={close}></CloseBtn>
                        </InformationHeader>
                        <InformationText>
                            <div
                                style={{ fontSize: getFontSizeFromSettings(params['nrel_font_size']) }}
                                dangerouslySetInnerHTML={{ __html: activeDescription }}
                            />
                            <div style={{ padding: '10px' }}>
                                <Line></Line>
                            </div>
                            <div style={{ width: '100%', display: 'flex', justifyContent: 'right' }}>
                                <SaveB onClick={savePoint}>
                                    <svg
                                        id="Capa_1"
                                        xmlns="http://www.w3.org/2000/svg"
                                        x="0px"
                                        y="0px"
                                        width="533.333px"
                                        height="533.334px"
                                        viewBox="0 0 533.333 533.334"
                                    >
                                        <path
                                            style={
                                                liked
                                                    ? { fill: 'red' }
                                                    : { fill: params['nrel_theme'] == 'dark' ? 'white' : '' }
                                            }
                                            d="M533.333,186.54c0,44.98-19.385,85.432-50.256,113.46h0.256L316.667,466.667C300,483.333,283.333,500,266.667,500
		c-16.667,0-33.333-16.667-50-33.333L50,300h0.255C19.384,271.972,0,231.52,0,186.54C0,101.926,68.593,33.333,153.206,33.333
		c44.98,0,85.432,19.384,113.46,50.255c28.028-30.871,68.48-50.255,113.46-50.255C464.74,33.333,533.333,101.926,533.333,186.54z"
                                        />
                                    </svg>
                                </SaveB>
                            </div>
                        </InformationText>
                    </Information>
                </Theme>
            </Menu>
        </>
    );
};

export default MapPage;
