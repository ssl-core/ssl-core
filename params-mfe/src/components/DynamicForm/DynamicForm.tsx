import { useEffect, useState } from "react";
import BooleanParameterInput from "../BooleanParameterInput/BooleanParameterInput";
import NumericParameterInput from "../NumericParameterInput/NumericParameterInput";
import FormFieldset from "../FormFieldset/FormFieldset";

import './styles.css'

type Field = {
  value: number | boolean;
  min?: number;
  max?: number;
};

interface NestedFormData {
  [key: string]: Field | NestedFormData;
}

function DynamicForm({ data }: { data: NestedFormData }) {
  const [formData, setFormData] = useState<NestedFormData>(data);

  const handleChange = (name: string, value: number | boolean) => {
    const updatedData = { ...formData };

    const updateFirstMatch = (obj: NestedFormData, name: string): boolean => {
      for (const key in obj) {
        const field = obj[key];
        if (typeof field === 'object' && 'value' in field) {
          if (key === name) {
            (field as Field).value = value;
            return true;
          }
        } else if (typeof field === 'object') {
          if (updateFirstMatch(field as NestedFormData, name)) return true;
        }
      }
      return false;
    };

    updateFirstMatch(updatedData, name);
    setFormData(updatedData);
  };

  function render(formData: NestedFormData) {
    return Object.keys(formData).map((key) => {
      const item = formData[key];

      if (typeof item === 'object' && 'value' in item) {
        if (typeof item.value === 'number') {
          return (
            <NumericParameterInput 
              key={key}
              defaultValue={item.value}
              value={item.value}
              label={key}
              max={item.max as number}
              min={item.min as number}
              onChange={handleChange}
            />
          );
        } else if (typeof item.value === 'boolean') {
          return (
            <BooleanParameterInput 
              key={key}  
              label={key}
              checked={item.value}
              defaultChecked={item.value}
              onChange={handleChange}  
            />
          );
        }
      } else if (typeof item === 'object') {
        return (
          <FormFieldset key={key} label={key}>
            {render(item as NestedFormData)}
          </FormFieldset>
        );
      }
      return null;
    });
  }

  useEffect(() => {
    setFormData(data);
  }, [data]);

  return (
    <form>
      {render(formData)}
      <button className="form-submit-btn" type="submit">Save</button>
    </form>
  );
}

export default DynamicForm
