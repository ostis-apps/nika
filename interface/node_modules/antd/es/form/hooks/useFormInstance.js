import { useContext } from 'react';
import { FormContext } from '../context';
export default function useFormInstance() {
  var _useContext = useContext(FormContext),
      form = _useContext.form;

  return form;
}