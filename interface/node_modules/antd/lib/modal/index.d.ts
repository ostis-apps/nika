import type { ModalStaticFunctions } from './confirm';
import { modalGlobalConfig } from './confirm';
import OriginModal from './Modal';
import useModal from './useModal';
export { ModalFuncProps, ModalProps } from './Modal';
declare type ModalType = typeof OriginModal & ModalStaticFunctions & {
    useModal: typeof useModal;
    destroyAll: () => void;
    config: typeof modalGlobalConfig;
};
declare const Modal: ModalType;
export default Modal;
