import React from 'react'
import ReactDOM from 'react-dom/client'
import App from './App'
import '@mantine/core/styles.css'

import { createTheme, MantineProvider } from '@mantine/core'

const theme = createTheme({
  fontFamily: 'Open Sans, sans-serif',
  primaryColor: 'blue'
})

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
  <React.StrictMode>
    <MantineProvider theme={theme} defaultColorScheme="dark">
      <App />
    </MantineProvider>
  </React.StrictMode>
)
