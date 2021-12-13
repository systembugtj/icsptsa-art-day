import logo from "./logo.svg";
import "./App.scss";
import Container from "react-bootstrap/Container";
import NavBar from "./components/NavBar/NavBar";

import PropTypes from "prop-types";

import { Outlet } from "react-router-dom";

function App({ history }) {
    return (
        <div>
            <Container className="p-3">
                <NavBar />
            </Container>
            <Outlet />
        </div>
    );
}

App.propTypes = {
    history: PropTypes.object,
};

export default App;
