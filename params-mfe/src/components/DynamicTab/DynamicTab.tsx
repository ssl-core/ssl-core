import { Tabs } from '../../types'
import DynamicForm from '../DynamicForm/DynamicForm'

interface Props {
  tab: Tabs
}

const jsonData1 = {
  forward: {
    ball_is_moving_velocity: {
      value: 1.99,
      min: 0,
      max: 100,
    },
    another_float_param: {
      value: 1.99,
      min: 0,
      max: 100,
    },
    // any other field
  },
  general: {
    boolean_param: {
      value: true
    }
    // any other field
  }
  // any other fieldset
};

const jsonData2 = {
  forward: {
    ball_is_moving_velocity: {
      value: 1.99,
      min: 0,
      max: 100,
    },
  }
};

function DynamicTab({
  tab,
}: Props) {
  function getParameters(tab: Tabs) {
    switch (tab) {
      case Tabs.DECISION:
        return jsonData1
        break;
      case Tabs.NAVIGATION:
        return jsonData2
        break;
      default:
        return jsonData2
        break;
    }
  }

  return (
    <>
      <h1>{tab}</h1>
      <DynamicForm 
        data={getParameters(tab)} 
      />
    </>
  )
}

export default DynamicTab