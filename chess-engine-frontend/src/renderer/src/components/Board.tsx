import { AspectRatio } from '@mantine/core'
import { Chessboard } from 'react-chessboard'

function Board() {
  return (
    <AspectRatio ratio={1 / 1} maw={400} mx="auto" mt="1rem" mb="1rem">
      <Chessboard id="basic_board" />
    </AspectRatio>
  )
}

export default Board
