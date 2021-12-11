import logo from "./logo.svg";
import "./App.scss";
import TrueHeader from "./components/TrueHeader/TrueHeader";

function App() {
    return (
        <div className="App">
            <TrueHeader className="App-header">
                <img src={logo} className="App-logo" alt="logo" />
                <p>
                    Edit <code>src/App.js</code> and save to reload.
                </p>
                <a
                    className="App-link"
                    href="https://reactjs.org"
                    target="_blank"
                    rel="noopener noreferrer"
                >
                    Learn React
                </a>
            </TrueHeader>
        </div>
    );
}

export default App;
