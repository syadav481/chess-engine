import { useEffect, useState } from 'react'
import { INIT_MESSAGE } from '../../dtos/client.dto'
import {
  Button,
  Flex,
  Stack,
  Textarea,
  TextInput
} from '@mantine/core'
import Board from './components/Board'

function App(): JSX.Element {
  const [engineOutput, setEngineOutput] = useState<JSON | null>(null)

  const start_engine_instance = (): void => window.electron.ipcRenderer.send('engine-start')
  const send_message_to_engine = (): void => {
    // place holder
    const msg: INIT_MESSAGE = {
      OPERATION: 'INIT',
      engine_color: 'white',
      FEN: 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1'
    }
    window.electron.ipcRenderer.send('engine-message', msg)
  }
  const kill_engine_instance = (): void => window.electron.ipcRenderer.send('engine-kill')

  useEffect(() => {
    window.api.load_engine_output((data: JSON) => {
      setEngineOutput(data)
    })

    return () => {
      window.api.load_engine_output(() => { })
    }
  }, [])

  return (
    <>
      <Board />
      <Stack>
        <Flex justify="center" align="center" gap="sm">
          <Button size="sm" onClick={start_engine_instance}>
            Start new game
          </Button>
          <Button size="sm" onClick={kill_engine_instance}>
            End game
          </Button>
        </Flex>
        <Stack justify="center" align="center" gap="sm">
          <TextInput />
          <Button size="sm" onClick={send_message_to_engine}>
            Apply FEN
          </Button>
          <Textarea>
            {JSON.stringify(engineOutput)}
          </Textarea>
        </Stack>
      </Stack>
    </>
  )
}

export default App
