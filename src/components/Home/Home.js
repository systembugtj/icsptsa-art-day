import React from "react";

import { bindActionCreators } from "redux";
import { connect } from "react-redux";

const Home = (props) => (
    <div>
        <h1>Home</h1>
        <p>Welcome home!</p>
        <button onClick={() => props.changePage()}>
            Go to about page via redux
        </button>
    </div>
);

const mapDispatchToProps = (dispatch) =>
    bindActionCreators(
        {
            changePage: () => {},
        },
        dispatch
    );

export default connect(null, mapDispatchToProps)(Home);
