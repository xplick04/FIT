import React from 'react';
import {createRoot} from 'react-dom/client';
import {BrowserRouter} from 'react-router-dom';

import '../node_modules/react-bootstrap/dist/react-bootstrap'
import '../node_modules/bootstrap/dist/css/bootstrap.css'

import App from './App';

const container = document.getElementById('root');
const root = createRoot(container);
root.render(
	<BrowserRouter>
		<App />
	</BrowserRouter>
);