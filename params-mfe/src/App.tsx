import { useState } from 'react';
import { Tabs } from './types';

import './App.css'
import DynamicTab from './components/DynamicTab/DynamicTab';

function App() {
  const [currentTab, setCurrentTab] = useState(Tabs.DECISION);

  console.log(currentTab);
  
  return (
    <article className='container'>
      <aside className='sidebar'>
        <nav>
          <ul className='sidebar__list'>
            {Object.values(Tabs).map((tab) => (
              <li key={tab} className='sidebar__list-item'>
                <button 
                  type='button' 
                  className={`tab-nav-btn ${currentTab === tab ? 'selected' : ''}`}
                  onClick={() => setCurrentTab(tab)}
                >
                  {tab}
                </button>
              </li>
            ))}
          </ul>
        </nav>
      </aside>
      <section className='tab-section'>
        <DynamicTab tab={currentTab} />
      </section>
    </article>
  )
}

export default App
