import { useEffect, useState } from 'react'
import { INIT_MESSAGE } from '../../dtos/client.dto'

function App(): JSX.Element {
  const start_engine_instance = (): void => window.electron.ipcRenderer.send('engine-start')
  const send_message_to_engine = (): void => {
    // place holder
    const msg: INIT_MESSAGE = { "OPERATION": "INIT", engine_color: "white", "FEN": "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" }
    window.electron.ipcRenderer.send('engine-message', msg)
  }
  const kill_engine_instance = (): void => window.electron.ipcRenderer.send('engine-kill')

  return (
    <>
      <h1>Chess</h1>
      <button onClick={start_engine_instance}>Start engine</button>
      <button onClick={kill_engine_instance}>Kill engine</button>
      <button onClick={send_message_to_engine}>Send a default message</button>
    </>
  )
}

export default App
