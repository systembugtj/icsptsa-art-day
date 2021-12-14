import React from "react";
import { Route, Routes } from "react-router-dom";
import Home from "./components/Home/Home";
import Layout from "./components/Layout/Layout";

const About = () => <span>About</span>;

const Users = () => <span>Users</span>;

export default function AppRoutes() {
    return (
        <Routes>
            <Route path="/" element={<Layout />}>
                <Route path="/home" element={<Home />}></Route>
                <Route path="/about" element={<About />}></Route>
                <Route path="/users" element={<Users />}></Route>
            </Route>
        </Routes>
    );
}
