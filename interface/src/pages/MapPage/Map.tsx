import { routes } from '@constants';
import React, { useEffect, useState } from 'react';
import { YMaps, Map, Placemark } from 'react-yandex-maps';
import {
    Arrow,
    Linktitle,
    NavLink,
    Menu,
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
    BtnSavePoint,
} from './styled';
import { getUserSettings } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import { ScAddr, ScConstruction, ScTemplate, ScType, ScLinkContent, ScLinkContentType } from 'ts-sc-client';
import { client } from '@api/sc';
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
    const [zoomValue, setZoomValue] = useState<Number>(10);

    const [accentColor, setAccentColor] = useState<string | undefined>('black');
    const [loadError, setLoadError] = useState<Boolean>(false);
    const [startPr, setStartPr] = useState<Boolean>(false);

    const [pageInform, setPageInform] = useState<String[][]>([]);
    const [activeTitle, setActiveTitle] = useState<String>('');
    const [activeDescription, setActiveDescription] = useState<string>('');
    const [imageStyles, setImageStyles] = useState<{}>({});
    const [imageTextStyles, setTextImageStyles] = useState<{}>({});

    const [modalSave, setModalSave] = useState<Boolean>(false);
    const [mapName, setMapName] = useState<String>('');

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
            setAccentColor((await getUserSettings(cookieUserAddr))['nrel_accent_color']);

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

    const open = (index: number) => async (event: React.MouseEvent<HTMLDivElement>) => {
        setLoadError(false);
        const desireCoordinates = index;
        console.log(pageInform[index]);
        setActiveTitle(pageInform[index][1]);
        setActiveDescription(pageInform[index][2] as string);

        if (pageInform[index][3] != '') {
            const st = {
                backgroundImage: `url(${pageInform[index][3]})`,
                backgroundRepeat: 'no-repeat',
                backgroundPosition: 'center',
                minHeight: '350px',
                alignItems: 'end',
                backgroundSize: '100%',
            };
            console.log(st);
            setImageStyles(st);
            setTextImageStyles({
                marginBottom: '10px',
                color: 'white',
            });
        } else {
            setImageStyles({
                background: accentColor,
                backdropFilter: 'opacity(30%)',
            });
            setTextImageStyles({ color: 'black' });
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
        construction.createEdge(ScType.EdgeAccessConstPosPerm, keynodes['nrel_main'], 'edge1');
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
        setMapName(e.target.value);
    };

    return (
        <>
            <NavLink href={routes.HOME} className="nav">
                <Arrow></Arrow>
                <Linktitle className="title">Назад</Linktitle>
            </NavLink>
            <SaveMap onClick={openModal}>S</SaveMap>
            <WrapperSavingName style={modalSave ? { zIndex: 1 } : { zIndex: -1 }}>
                <CloseBtnModal onClick={closeModal} style={{ zIndex: 11 }}></CloseBtnModal>
                <ModalName>
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
            <Menu style={openMenu ? { top: 0, right: 0 } : {}} className="container_menu">
                <Loading style={!startPr ? { opacity: 1, zIndex: 100 } : { opacity: 0, zIndex: -1 }}>
                    <SpanLoader style={{ background: accentColor }}></SpanLoader>
                    <Error style={loadError ? { opacity: 1 } : { opacity: 0 }}>Ошибка сети.</Error>
                </Loading>
                <Information>
                    <InformationHeader style={imageStyles}>
                        <Inf style={imageTextStyles}>
                            <p style={{ fontSize: '20px' }}>{activeTitle}</p>
                        </Inf>
                        <CloseBtn onClick={close}></CloseBtn>
                    </InformationHeader>
                    <InformationText>
                        <div dangerouslySetInnerHTML={{ __html: activeDescription }} />
                        <Line></Line>
                        <div style={{ width: '100%', display: 'flex', justifyContent: 'right' }}>
                            <BtnSavePoint>L</BtnSavePoint>
                        </div>
                    </InformationText>
                </Information>
            </Menu>
        </>
    );
};

export default MapPage;
