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
    nothinTitleStyles,
    Loading,
    SpanLoader,
} from './styled';
import { getUserSettings } from '@api/sc/checkUser';
import Cookie from 'universal-cookie';
import { ScAddr } from 'ts-sc-client';

export const MapPage = () => {
    const cookie = new Cookie();
    const cookieUserAddr = cookie.get('userAddr')
        ? new ScAddr(parseInt(String(cookie.get('userAddr'))))
        : new ScAddr(0);

    const [openMenu, setOpenMenu] = useState<boolean>(false);

    const [coordinates, setCoordinates] = useState<String[][]>([[]]);
    const [coordCenter, setCoordCenter] = useState<String[]>([]);
    const [zoomValue, setZoomValue] = useState<Number>(10);

    const [accentColor, setAccentColor] = useState<string | undefined>('black');
    const [pageTitle, setPageTitle] = useState<String>('');


    useEffect(() => {
        const queryParams = new URLSearchParams(window.location.search);
        const coordinatesString = (queryParams.get('coord') ?? '53.931986, 27.668021').split(',');
        setCoordCenter([coordinatesString[0], coordinatesString[1]]);

        let resultCoordinates: String[][] = [];
        for (let i = 2; i < coordinatesString.length; i += 2)
            resultCoordinates.push([coordinatesString[i], coordinatesString[i + 1]]);

        setCoordinates(resultCoordinates);
    }, []);

    useEffect(() => {
        (async () => {
            setAccentColor((await getUserSettings(cookieUserAddr))['nrel_accent_color']);
        })();
    }, []);

    
    const getInformation = async (coords: String[]) => {
        let title = '';
        let description = '';
        let image = '';


    };

    const open = (index: number) => async (event: React.MouseEvent<HTMLDivElement>) => {
        const desireCoordinates = coordinates[index];
        setPageTitle('');
        await getInformation(desireCoordinates);
        setOpenMenu(true);
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
                </Loading>
                <Information>
                    <InformationHeader>
                        <Inf>
                            <p style={pageTitle == '' ? nothinTitleStyles : {}}>{pageTitle}</p>
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
