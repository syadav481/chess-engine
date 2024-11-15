import { app, shell, BrowserWindow, ipcMain } from 'electron'
import { join } from 'path'
import { electronApp, optimizer, is } from '@electron-toolkit/utils'
import icon from '../../resources/icon.png?asset'
import { ChildProcess, spawn } from 'child_process'

let mainWindow: BrowserWindow | null = null
let engineProcess: ChildProcess | null = null

function createWindow(): void {
  // Create the browser window.
  mainWindow = new BrowserWindow({
    width: 900,
    height: 670,
    show: false,
    autoHideMenuBar: true,
    ...(process.platform === 'linux' ? { icon } : {}),
    webPreferences: {
      preload: join(__dirname, '../preload/index.js'),
      sandbox: false
    }
  })

  mainWindow.on('ready-to-show', () => {
    mainWindow?.show()
  })

  mainWindow.webContents.setWindowOpenHandler((details) => {
    shell.openExternal(details.url)
    return { action: 'deny' }
  })

  // HMR for renderer base on electron-vite cli.
  // Load the remote URL for development or the local html file for production.
  if (is.dev && process.env['ELECTRON_RENDERER_URL']) {
    mainWindow.loadURL(process.env['ELECTRON_RENDERER_URL'])
  } else {
    mainWindow.loadFile(join(__dirname, '../renderer/index.html'))
  }
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  // Set app user model id for windows
  electronApp.setAppUserModelId('com.electron')

  // Default open or close DevTools by F12 in development
  // and ignore CommandOrControl + R in production.
  // see https://github.com/alex8088/electron-toolkit/tree/master/packages/utils
  app.on('browser-window-created', (_, window) => {
    optimizer.watchWindowShortcuts(window)
  })

  ipcMain.on('engine-start', () => {
    // I think electron will autokill this process if I just close the window
    engineProcess = spawn('../chess-engine-backend/build/engine_backend')
    console.log('spawned engine at pid: ', engineProcess?.pid)

    engineProcess?.stdout?.on('data', (data: Buffer) => {
      console.log('from engine: ', data.toString())
    })

    engineProcess?.on('close', (code) => {
      console.log(`Engine process exited with code ${code}`)
      engineProcess = null
    })
  })

  ipcMain.on('engine-message', () => {
    if (!engineProcess) {
      console.log('CLIENT: tried to send a message but engine was not running')
    } else if (!engineProcess.stdin) {
      console.log('CLIENT: engine process exists but stdin is invalid')
    } else {
      const delim = '#'
      const end_delim = '!'
      const msg = `rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR${delim}w${delim}KQkq${delim}-${delim}0${delim}1${end_delim}`
      engineProcess.stdin.write(msg)
      console.log('CLIENT: Sent: ', msg)
    }
  })

  createWindow()

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

// In this file you can include the rest of your app"s specific main process
// code. You can also put them in separate files and require them here.
