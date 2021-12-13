import { createBrowserHistory } from "history";
import { applyMiddleware, compose, createStore } from "redux";
import { createRouterMiddleware } from "@lagunovsky/redux-react-router";
import createRootReducer from "./reducers";

export const history = createBrowserHistory();
const routerMiddleware = createRouterMiddleware(history);

export default function configureStore(preloadedState) {
    const composeEnhancer =
        window.__REDUX_DEVTOOLS_EXTENSION_COMPOSE__ || compose;
    const store = createStore(
        createRootReducer(history),
        preloadedState,
        composeEnhancer(applyMiddleware(routerMiddleware))
    );

    // Hot reloading
    if (module.hot) {
        // Enable Webpack hot module replacement for reducers
        module.hot.accept("./reducers", () => {
            store.replaceReducer(createRootReducer(history));
        });
    }

    return store;
}
