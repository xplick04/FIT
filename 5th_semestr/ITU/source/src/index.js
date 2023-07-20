/**
 * @brief App rendering file
 * 
 * @file Index.js
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React from 'react';
import ReactDOM from 'react-dom/client';
import './index.css';
import App from './App';
import { BrowserRouter } from 'react-router-dom';

const root = ReactDOM.createRoot(document.getElementById('root'));
//App render function + router for page routing
root.render(
  <BrowserRouter> 
    <App />
  </BrowserRouter>
);
