import Dragger from './Dragger';
import InternalUpload, { LIST_IGNORE } from './Upload';
var Upload = InternalUpload;
Upload.Dragger = Dragger;
Upload.LIST_IGNORE = LIST_IGNORE;
export default Upload;