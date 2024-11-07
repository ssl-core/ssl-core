import { ReactNode } from 'react'
import './styles.css'

interface Props {
  label: string;
  children: ReactNode
}

function FormFieldset({
  label,
  children,
}: Props) {
  return (
    <fieldset className='form-fieldset'>
      <legend><pre>{label}</pre></legend>
      {children}
    </fieldset>
  )
}

export default FormFieldset