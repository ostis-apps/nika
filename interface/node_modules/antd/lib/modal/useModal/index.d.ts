import * as React from 'react';
import type { ModalStaticFunctions } from '../confirm';
export default function useModal(): [Omit<ModalStaticFunctions, 'warn'>, React.ReactElement];
