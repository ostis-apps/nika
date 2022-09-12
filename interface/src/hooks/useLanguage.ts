import { selectLanguage } from '@store/commonSlice';
import { useSelector } from '@hooks';

export const useLanguage = () => useSelector(selectLanguage);
