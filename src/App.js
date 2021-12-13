import logo from "./logo.svg";
import "./App.scss";
import Container from "react-bootstrap/Container";
import NavBar from "./components/NavBar/NavBar";
import { MemoryRouter, Routes, Route } from "react-router-dom";

const Home = () => <span>Home</span>;

const About = () => <span>About</span>;

const Users = () => <span>Users</span>;

function App() {
    return (
        <MemoryRouter>
            <Container className="App p-3">
                <Routes>
                    <Route path="/about" component={About}></Route>
                    <Route path="/users" component={Users}></Route>
                    <Route path="/" component={Home}></Route>
                </Routes>
                <NavBar />
                <header className="App-header">
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
                </header>
            </Container>
        </MemoryRouter>
    );
}

export default App;
