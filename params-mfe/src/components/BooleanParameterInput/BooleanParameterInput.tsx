import './styles.css'

interface Props {
  label: string;
  checked: boolean;
  defaultChecked: boolean;
  onChange: (name: string, value: boolean) => void;
}

function BooleanParameterInput({
  label, 
  checked, 
  defaultChecked, 
  onChange
}: Props) {
  return (
    <div className="switch-container">
      <span><pre>{label}</pre></span>
      <label className="switch" htmlFor={label}>
        <input 
          id={label} 
          type="checkbox" 
          checked={checked} 
          defaultChecked={defaultChecked} 
          onChange={(e) => onChange(label, e.target.checked)}
        />
        <span className="slider round"></span>
      </label>
    </div>
  )
}

export default BooleanParameterInput