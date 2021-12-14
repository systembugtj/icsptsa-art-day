import logo from "./logo.svg";
import "./App.scss";
import Container from "react-bootstrap/Container";
import NavBar from "./components/NavBar/NavBar";
import { Provider } from "react-redux";
import { ReduxRouter } from "@lagunovsky/redux-react-router";
import AppRoutes from "./routes";
import configureStore, { history } from "./configureStore";
import PropTypes from "prop-types";
import { BrowserRouter, Outlet, Route, Routes, Link } from "react-router-dom";

import Home from "./components/Home/Home";

const About = () => <span>About</span>;

const Users = () => <span>Users</span>;

function Layout() {
    return (
        <Container classname="p3">
            {/* A "layout route" is a good place to put markup you want to
          share across all the pages on your site, like navigation. */}
            <NavBar />

            {/* An <Outlet> renders whatever child route is currently active,
          so you can think about this <Outlet> as a placeholder for
          the child routes we defined above. */}
            <Outlet />
        </Container>
    );
}

const store = configureStore({});
function App() {
    return (
        <Provider store={store}>
            <BrowserRouter>
                <Routes>
                    <Route path="/" element={<Layout />}>
                        <Route path="/home" element={<Home />}></Route>
                        <Route path="/about" element={<About />}></Route>
                        <Route path="/users" element={<Users />}></Route>
                    </Route>
                </Routes>
            </BrowserRouter>
        </Provider>
    );
}

App.propTypes = {
    history: PropTypes.object,
};

export default App;
