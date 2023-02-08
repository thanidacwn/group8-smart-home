import { useState } from 'react'
import reactLogo from './assets/react.svg'
import './App.css'
import Home from "./pages/Home"
import 'bootstrap/dist/css/bootstrap.min.css';

function App() {
  const [count, setCount] = useState(0)

  return (
    <div className="App">
        <Home />
    </div>
  )
}

export default App
