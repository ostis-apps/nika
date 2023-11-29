/// <reference types="react" />
import Dragger from './Dragger';
import type { UploadProps } from './Upload';
import InternalUpload from './Upload';
export { DraggerProps } from './Dragger';
export { RcFile, UploadChangeParam, UploadFile, UploadListProps, UploadProps } from './interface';
declare type InternalUploadType = typeof InternalUpload;
interface UploadInterface<T = any> extends InternalUploadType {
    <U extends T>(props: React.PropsWithChildren<UploadProps<U>> & React.RefAttributes<any>): React.ReactElement;
    Dragger: typeof Dragger;
    LIST_IGNORE: string;
}
declare const Upload: UploadInterface<any>;
export default Upload;
