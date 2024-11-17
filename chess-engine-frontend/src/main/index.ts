import { app, shell, BrowserWindow, ipcMain, IpcMainEvent } from 'electron'
import { join } from 'path'
import { electronApp, optimizer, is } from '@electron-toolkit/utils'
import icon from '../../resources/icon.png?asset'
import { ChildProcess, spawn } from 'child_process'
import { CLIENT_MESSAGE } from '../dtos/client.dto'

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
    engineProcess = spawn('../build/engine_backend')

    engineProcess?.stderr?.on('data', (buf: Buffer) => {
      console.error('ENGINE-STDERR:\n', buf.toString())
      console.error('==========================================\n')
    })

    engineProcess?.stdout?.on('data', (buf: Buffer) => {
      // TODO: Change this to use a DTO once some of the cpp side stuff is done
      const engine_output: JSON = JSON.parse(buf.toString())
      console.log('ENGINE-STDOUT: as json', engine_output)
      console.log('ENGINE-STDOUT: as a string', buf.toString())

      mainWindow?.webContents.send('load_engine_output', engine_output)
    })

    engineProcess?.on('close', (code) => {
      console.error(`CLIENT: Engine process exited with code ${code}`)
      engineProcess = null
    })

    console.error('CLIENT: Spawned engine at pid: ', engineProcess?.pid)
  })

  ipcMain.on('engine-message', (_: IpcMainEvent, msg: CLIENT_MESSAGE) => {
    if (!engineProcess) {
      console.error('CLIENT: tried to send a message but engine was not running')
    } else if (!engineProcess.stdin) {
      console.error('CLIENT: engine process exists but stdin is invalid')
    } else {
      // const msg = { "OPERATION": "INIT", "FEN": "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" }
      engineProcess.stdin.write(JSON.stringify(msg))
      console.error('CLIENT: Sent: ', msg)
    }
  })

  ipcMain.on('engine-kill', () => {
    if (!engineProcess) {
      console.error('CLIENT: tried to kill engine but engine was not running')
    } else {
      engineProcess.kill()
      engineProcess = null
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
