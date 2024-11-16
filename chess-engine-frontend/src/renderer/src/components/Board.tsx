import { Box, Center, Paper, SimpleGrid } from '@mantine/core';
import React from 'react';

function Board() {
    // Create an array with 64 undefined values
    const boxes = Array(64).fill(null);

    return (
        <Paper shadow='xl' radius="md">
            <SimpleGrid cols={8} spacing="xs">
                {boxes.map((_, index) => (
                    <Box key={index} bg={index % 2 == 0 ? 'rgb(66, 66, 67)' : 'gray'}>
                        <Center>

                        </Center>
                    </Box>
                ))}
            </SimpleGrid>
        </Paper>
    );
}

export default Board;
