import { useState } from 'react'
import robloxLogo from './assets/roblox.svg'
import vascoLogo from './assets/vasco.svg'
import aespaLogo from './assets/aespa.svg'
import './App.css'

function App() {
  const [vascoCount, setVascoCount] = useState(0)
  const [robloxCount, setRobloxCount] = useState(0)
  const [aespaCount, setAespaCount] = useState(0)

  return (
    <>
      <div className='container-logos'>
        <a href="https://vasco.com.br/" target="_blank">
          <img onClick={() => setVascoCount((vascoCount) => vascoCount + 1)} src={vascoLogo} className="logo" alt="Vasco logo" style={{ width: '100px', height: '100px' }} />
          <div>
            vasco tem {vascoCount} votos
            </div>
        </a>
        <a href="https://www.roblox.com/pt/" target="_blank">
          <img onClick={() => setRobloxCount((robloxCount) => robloxCount + 1)} src={robloxLogo} className="logo" alt="Roblox logo" style={{ width: '100px', height: '100px' }} />
          <div>
            roblox tem {robloxCount} votos
            </div>
        </a>
        <a href="https://aespa.com/" target="_blank">
          <img onClick={() => setAespaCount((aespaCount) => aespaCount + 1)} src={aespaLogo} className="logo" alt="Aespa logo" style={{ width: '150px', height: '100px' }} />
          <div>
            aespa tem {aespaCount} votos
            </div>
        </a>
      </div>
    </>
  )
}

export default App
