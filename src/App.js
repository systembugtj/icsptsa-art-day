import logo from "./logo.svg";
import "./App.scss";

import { Provider } from "react-redux";
import { ReduxRouter } from "@lagunovsky/redux-react-router";
import AppRoutes from "./routes";
import configureStore, { history } from "./configureStore";
import PropTypes from "prop-types";
import { BrowserRouter, Outlet, Route, Routes, Link } from "react-router-dom";

const store = configureStore({});
function App() {
    return (
        <Provider store={store}>
            <BrowserRouter>
                <AppRoutes />
            </BrowserRouter>
        </Provider>
    );
}

App.propTypes = {
    history: PropTypes.object,
};

export default App;
