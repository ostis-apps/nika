import InternalAnchor from './Anchor';
import AnchorLink from './AnchorLink';
export { AnchorProps } from './Anchor';
export { AnchorLinkProps } from './AnchorLink';
declare type InternalAnchorType = typeof InternalAnchor;
interface AnchorInterface extends InternalAnchorType {
    Link: typeof AnchorLink;
}
declare const Anchor: AnchorInterface;
export default Anchor;
