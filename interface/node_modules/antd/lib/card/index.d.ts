import InternalCard from './Card';
import Grid from './Grid';
import Meta from './Meta';
export { CardProps, CardTabListType } from './Card';
export { CardGridProps } from './Grid';
export { CardMetaProps } from './Meta';
declare type InternalCardType = typeof InternalCard;
export interface CardInterface extends InternalCardType {
    Grid: typeof Grid;
    Meta: typeof Meta;
}
declare const Card: CardInterface;
export default Card;
