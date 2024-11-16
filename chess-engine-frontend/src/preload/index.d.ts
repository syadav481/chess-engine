import { ElectronAPI } from '@electron-toolkit/preload'

export interface IElectronAPI {
  load_engine_output: (callback: (data: JSON) => void) => Electron.IpcRenderer
}

declare global {
  interface Window {
    electron: ElectronAPI
    api: IElectronAPI
  }
}
