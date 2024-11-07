import './styles.css'

interface Props {
  label: string;
  value: number;
  defaultValue: number;
  max: number;
  min: number;
  step?: number;
  onChange: (name: string, value: number) => void;
}

function NumericParameterInput({
  label,
  value,
  defaultValue,
  max,
  min,
  step = 1,
  onChange,
}: Props) {
  return (
    <div className='numeric-input-container'>
      <label htmlFor={label}><pre>{label}</pre></label>
      <input
        className='numeric-input'
        defaultValue={defaultValue}
        value={value}
        type="number"
        id={label}
        max={max}
        min={min}
        step={step}
        onChange={(e) => onChange(
          label, 
          parseFloat(e.target.value)
        )}
      />
    </div>
  )
}

export default NumericParameterInput