import React, { useEffect } from 'react';
import { YMaps, Map } from '@pbe/react-yandex-maps';

export const MapPage = () => {
    // const queryParams = new URLSearchParams(window.location.search);
    // const coordinatesString = queryParams.get('x') ?? '27.668021,53.931986';
    // const coordinates = coordinatesString.split(',');

    // useEffect(() => {
    //     // Initialize the map
    //     load().then((mapglAPI) => {
    //         const map = new mapglAPI.Map('container', {
    //             center: [parseFloat(coordinates[0]), parseFloat(coordinates[1])],
    //             zoom: 13,
    //             key: 'cff13584-2b22-4186-9b1d-14d703c594c2',
    //             style: 'e05achttps://api-maps.yandex.ru/2.1/?lang=en_RU&amp;apikey=b3b592a1-27f8-4994-a2a0-5e83e5c5c2af437-fcc2-4845-ad74-b1de9ce07555',
    //         });

    //         for (var i = 2; i < coordinates.length; i += 2) {
    //             const marker = new mapglAPI.Marker(map, {
    //                 coordinates: [parseFloat(coordinates[i]), parseFloat(coordinates[i + 1])],
    //             });
    //         }
    //     });
    // }, [parseFloat(coordinates[0]), parseFloat(coordinates[1])]);

    return (
        <>
            {/* <a href="../index.html" onclick="javascript:history.back(); return false;" class="nav">
                <div class="arrow"></div>
                <p id="title">Назад</p>
            </a>
            <div id="map"></div>
            <div class="container_menu" id="menu">
                <div class="information">
                    <div class="information_header">
                        <div class="inf"></div>
                        <div class="close-btn" id="close-btn"></div>
                    </div>
                    <div class="information_text"></div>
                </div>
            </div>
            <script src="https://api-maps.yandex.ru/2.1/?lang=en_RU&amp;apikey=b3b592a1-27f8-4994-a2a0-5e83e5c5c2af"></script>
            <script src="https://yandex.st/jquery/2.2.3/jquery.min.js"></script> */}

            <YMaps>
                <div>
                    This is a page.
                    <Map defaultState={{ center: [55.75, 37.57], zoom: 9 }} />
                </div>
            </YMaps>
        </>
    );
};
