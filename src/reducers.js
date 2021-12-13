import { combineReducers } from "redux";
import { createRouterReducer } from "@lagunovsky/redux-react-router";

const createRootReducer = (history) =>
    combineReducers({
        router: createRouterReducer(history),
    });

export default createRootReducer;
