import React from "react";
import PropTypes from "prop-types";
import styles from "./NavBar.module.scss";

import Button from "react-bootstrap/Button";
import ButtonToolbar from "react-bootstrap/ButtonToolbar";
import { LinkContainer } from "react-router-bootstrap";

const NavBar = () => (
    <div className={styles.NavBar} data-testid="NavBar">
        <ButtonToolbar className="custom-btn-toolbar">
            <LinkContainer to="/">
                <Button>Home</Button>
            </LinkContainer>
            <LinkContainer to="/about">
                <Button>About</Button>
            </LinkContainer>
            <LinkContainer to="/users">
                <Button>Users</Button>
            </LinkContainer>
        </ButtonToolbar>
    </div>
);

NavBar.propTypes = {};

NavBar.defaultProps = {};

export default NavBar;
