import { useEffect, useState } from 'react'

function App(): JSX.Element {
  const start_engine_instance = (): void => window.electron.ipcRenderer.send('engine-start')
  const send_message_to_engine = (): void => window.electron.ipcRenderer.send('engine-message')
  useEffect(() => {
    window.electron.ipcRenderer.on('engine-output', (_event, data: string) => {
      setEngineMessage(data)
    })

    return (): void => {
      window.electron.ipcRenderer.removeAllListeners('engine-output')
    }
  }, [])

  const [engineMessage, setEngineMessage] = useState<string>('')

  return (
    <>
      <h1>Engine has said: </h1>
      <p>{engineMessage}</p>
      <button onClick={start_engine_instance}>Start engine</button>
      <button onClick={send_message_to_engine}>Send a default message</button>
    </>
  )
}

export default App
