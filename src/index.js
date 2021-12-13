import React from "react";
import ReactDOM from "react-dom";
import "./index.scss";
import App from "./App";
import { Provider } from "react-redux";
import configureStore, { history } from "./configureStore";
import reportWebVitals from "./reportWebVitals";
import { ReduxRouter } from "@lagunovsky/redux-react-router";
import AppRoutes from "./routes";

const store = configureStore({});

ReactDOM.render(
    <React.StrictMode>
        <Provider store={store}>
            <ReduxRouter
                history={history}
                store={store}
                children={<AppRoutes />}
            />
        </Provider>
    </React.StrictMode>,
    document.getElementById("root")
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
reportWebVitals();
