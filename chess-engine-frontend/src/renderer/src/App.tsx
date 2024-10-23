import { useEffect, useState } from 'react'

function App(): JSX.Element {
  const start_engine_instance = (): void => window.electron.ipcRenderer.send('engine-start')
  useEffect(() => {
    // Listen for messages from the main process
    window.electron.ipcRenderer.on('engine-output', (_event, data: string) => {
      setEngineMessage(data)
    })

    // Cleanup listener on component unmount
    return (): void => {
      window.electron.ipcRenderer.removeAllListeners('engine-output')
    }
  }, [])

  const [engineMessage, setEngineMessage] = useState<string>('')

  return (
    <>
      <h1>Engine has said: </h1>
      <p>{engineMessage}</p>
      <button onClick={start_engine_instance}></button>
    </>
  )
}

export default App
