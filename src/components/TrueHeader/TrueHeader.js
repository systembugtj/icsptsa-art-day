import React from 'react';
import PropTypes from 'prop-types';
import styles from './TrueHeader.module.scss';

const TrueHeader = () => (
  <div className={styles.TrueHeader} data-testid="TrueHeader">
    TrueHeader Component
  </div>
);

TrueHeader.propTypes = {};

TrueHeader.defaultProps = {};

export default TrueHeader;
