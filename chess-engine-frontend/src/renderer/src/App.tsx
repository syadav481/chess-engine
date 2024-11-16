import { useEffect, useState } from 'react'
import { INIT_MESSAGE } from '../../dtos/client.dto'
import { Button, Stack, Title } from '@mantine/core';
import Board from './components/Board';


function App(): JSX.Element {
  const [engineOutput, setEngineOutput] = useState<JSON | null>(null);

  const start_engine_instance = (): void => window.electron.ipcRenderer.send('engine-start')
  const send_message_to_engine = (): void => {
    // place holder
    const msg: INIT_MESSAGE = { "OPERATION": "INIT", engine_color: "white", "FEN": "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" }
    window.electron.ipcRenderer.send('engine-message', msg)
  }
  const kill_engine_instance = (): void => window.electron.ipcRenderer.send('engine-kill')

  useEffect(() => {
    window.api.load_engine_output((data: JSON) => {
      setEngineOutput(data);
    })

    return () => {
      window.api.load_engine_output(() => { });
    }
  }, [])

  // TODO NOW: Figure out how to wait for this to load
  return (
    <Stack>
      <Title>Chess</Title>
      <Board />
      <Button onClick={start_engine_instance}>Start new game</Button>
      <Button onClick={kill_engine_instance}>End game</Button>
      <Button onClick={send_message_to_engine}>Send a default message</Button>
    </Stack>
  )
}

export default App
