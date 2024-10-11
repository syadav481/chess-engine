function App(): JSX.Element {
  const ipcHandle = (): void => window.electron.ipcRenderer.send('ping')

  return (
    <>
      <h1>hi</h1>
      <button onClick={ipcHandle}></button>
    </>
  )
}

export default App
