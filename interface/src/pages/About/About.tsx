import React, { useEffect } from 'react';
import { load } from '@2gis/mapgl';

export const About = () => {
    const queryParams = new URLSearchParams(window.location.search)
    const coordinatesString = queryParams.get("x") ?? "27.668021,53.931986"
    const coordinates = coordinatesString.split(',')
    
    useEffect(() => {
        // Initialize the map
        load().then((mapglAPI) => {
            const map = new mapglAPI.Map('container', {
                center: [parseFloat(coordinates[0]), parseFloat(coordinates[1])],
                zoom: 13,
                key: 'cff13584-2b22-4186-9b1d-14d703c594c2',
                style: 'e05ac437-fcc2-4845-ad74-b1de9ce07555',
            });

            for (var i = 2; i < coordinates.length; i += 2) {
                const marker = new mapglAPI.Marker(map, {
                    coordinates: [parseFloat(coordinates[i]), parseFloat(coordinates[i+1])],
                });
            }


        });
    }, [parseFloat(coordinates[0]), parseFloat(coordinates[1])]);

    return <div id="container" style={{ width: '100%', height: '100vh' }} />;
};