import { Spinner } from '@components/Spinners/LoadSpinner';
import React, { Suspense } from 'react';
import styled from 'styled-components';

const FullPageSpinner = styled(Spinner)`
    margin: auto;
`;

// eslint-disable-next-line react/display-name,@typescript-eslint/explicit-module-boundary-types
export const loadingComponent = (C) => () =>
    (
        <Suspense fallback={<FullPageSpinner size={64} />}>
            <C />
        </Suspense>
    );
