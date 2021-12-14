import React from "react";
import { Route, Routes } from "react-router-dom";
import Home from "./components/Home/Home";

const About = () => <span>About</span>;

const Users = () => <span>Users</span>;

export default function AppRoutes() {
    return (
        <Routes>
            <Route path="/home" component={Home}></Route>
            <Route path="/about" component={About}></Route>
            <Route path="/users" component={Users}></Route>
        </Routes>
    );
}
