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

import Walk from '../components/Walk'

import styles from '../styles/Walks.module.css'
import container from '../styles/Container.module.css'

const Walks = () => {

	let {authTokens} = useContext(AuthContext);

	const [walks, setWalks] = useState([]);

	const getWalks = async () => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'mywalks/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then((response) => {
			setWalks(response.data);
		});
	};

	useEffect(() => {
		getWalks();
	}, []);

	return (
		<Container
			className={`${container.Container} p-5 shadow rounded`}>
			{(walks?.length > 0 && Array.isArray(walks)) &&
				<>
				{toChunks(walks).map((new_walks) => (
					<Row className={`${styles.Row} mt-4`}>
						{new_walks.map((walk) => (
							<Walk walk={walk} key={walk.id}/>
						))}
					</Row>
				))}
				</>
			}
		</Container>
	)
}

export default Walks
