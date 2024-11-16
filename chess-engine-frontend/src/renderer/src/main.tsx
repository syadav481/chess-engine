import React from 'react'
import ReactDOM from 'react-dom/client'
import App from './App'
import '@mantine/core/styles.css'

import { createTheme, MantineProvider } from '@mantine/core';

const theme = createTheme({
  fontFamily: 'Open Sans, sans-serif',
  primaryColor: 'blue',

  colors: {
    blue: [
      "#e5f8ff",
      "#d0ecff",
      "#9fd7fc",
      "#6cc0fb",
      "#46aefa",
      "#32a2fa",
      "#259cfb",
      "#1788e0",
      "#0078c9",
      "#0068b2"
    ]
  }
});

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
  <React.StrictMode>
    <MantineProvider theme={theme} defaultColorScheme='dark'>
      <App />
    </MantineProvider>
  </React.StrictMode>
)

