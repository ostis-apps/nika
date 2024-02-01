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
    InformationText,
    openStyle,
    Loading,
    SpanLoader,
    Error,
} from './styled';
import { getUserSettings } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import { ScAddr } from 'ts-sc-client';
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
    const [pageTitle, setPageTitle] = useState<String>('');
    const [loadError, setLoadError] = useState<Boolean>(false);

    useEffect(() => {
        const queryParams = new URLSearchParams(window.location.search);
        const coordinatesString = (queryParams.get('coord') ?? '53.931986, 27.668021').split(',');
        setCoordCenter([coordinatesString[0], coordinatesString[1]]);
        // Get details and other information
        const urlXids = (queryParams.get('xids') ?? '').split(',');
    }, []);

    useEffect(() => {
        (async () => {
            setAccentColor((await getUserSettings(cookieUserAddr))['nrel_accent_color']);
        })();
    }, []);

    const getInformation = async (coords: String[]) => {
        try {
            let title = '';
            let description = '';
            let image = '';

            const url = `https://api.openrouteservice.org/geocode/reverse?api_key=${API_KEY}&point.lon=${coords[1]}&point.lat=${coords[0]}`;

            const response = await fetch(url),
                result = (await response.json())['features'];
            console.log(result);

            title = result[0]['properties']['label'].split(',')[0];
            setPageTitle(title);
        } catch {
            setLoadError(true);
        }
    };

    const open = (index: number) => async (event: React.MouseEvent<HTMLDivElement>) => {
        setLoadError(false);
        const desireCoordinates = coordinates[index];
        setPageTitle('');
        setOpenMenu(true);
        await getInformation(desireCoordinates);
    };

    const close = () => {
        setOpenMenu(false);
    };

    return (
        <>
            <NavLink href={routes.HOME} className="nav">
                <Arrow></Arrow>
                <Linktitle className="title">Назад</Linktitle>
            </NavLink>
            <YMaps>
                <Map defaultState={{ center: coordCenter, zoom: zoomValue }} width="100%" height="100%">
                    {coordinates.map((coordinate, index) => (
                        <Placemark key={index} geometry={coordinate} onClick={open(index)} />
                    ))}
                </Map>
            </YMaps>
            <Menu style={openMenu ? openStyle : {}} className="container_menu">
                <Loading style={pageTitle == '' ? { opacity: 1 } : { opacity: 0 }}>
                    <SpanLoader style={{ background: accentColor }}></SpanLoader>
                    <Error style={loadError ? { opacity: 1 } : { opacity: 0 }}>Ошибка сети.</Error>
                </Loading>
                <Information>
                    <InformationHeader>
                        <Inf>
                            <p style={{ color: 'black', fontSize: '20px' }}>{pageTitle}</p>
                        </Inf>
                        <CloseBtn onClick={close}></CloseBtn>
                    </InformationHeader>
                    <InformationText></InformationText>
                </Information>
            </Menu>
        </>
    );
};

export default MapPage;
