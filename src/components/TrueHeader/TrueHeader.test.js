import React from 'react';
import { render, screen } from '@testing-library/react';
import '@testing-library/jest-dom/extend-expect';
import TrueHeader from './TrueHeader';

describe('<TrueHeader />', () => {
  test('it should mount', () => {
    render(<TrueHeader />);
    
    const trueHeader = screen.getByTestId('TrueHeader');

    expect(trueHeader).toBeInTheDocument();
  });
});