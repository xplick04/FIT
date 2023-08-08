import React from 'react'
import { useState, useEffect, useContext } from 'react'
import axios from 'axios'

import {
	Container,
	Row,
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'
import toChunks from '../utils/toChunks'

import Request from '../components/Request'

import styles from '../styles/Admin.module.css'
import container from '../styles/Container.module.css'

const Requests = () => {

	let {authTokens} = useContext(AuthContext);

	const [requests, setRequests] = useState([]);

	const getRequests = async () => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'request-list/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then((response) => {
			setRequests(response.data);
		});
	};

	useEffect(() => {
		getRequests();
	}, []);

	return (
		<Container
			className={`${container.Container} p-5 shadow rounded`}>
			{(requests?.length > 0 && Array.isArray(requests)) &&
				<>
				{toChunks(requests).map((new_request) => (
					<Row className={`${styles.Row} mt-4`}>
						{new_request.map((request) => (
							<Request
								getRequests={getRequests}
								request={request}
								key={request.id}/>
						))}
					</Row>
				))}
				</>
			}
		</Container>
	)
}

export default Requests
