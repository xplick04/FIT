import React from 'react'
import { useState, useEffect, useContext } from 'react'
import axios from 'axios'

import {
	Card,
	Col,
	Button,
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import colStyle from '../styles/Admin.module.css'
import styles from '../styles/Requests.module.css'
import myButton from '../styles/Button.module.css'

const Request = ({request, getRequests}) => {

	let {authTokens} = useContext(AuthContext);

	const deleteRequest = async (e) => {
		const options = {
			method: 'DELETE',
			url: configData.API_URL + 'request-delete/' + request.id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then(() => {
			getRequests();
		});
	};

	const [animalName, setAnimalName] = useState('');

	const getAnimal = async () => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'animal-detail/' + request.animal + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then((response) => {
			setAnimalName(response.data.name);
		});
	};

	useEffect(() => {
        getAnimal();
    }, []);

	return (
		<Col className={colStyle.Col}>
			<Card className={`${styles.Card} p-3 shadow`}>
				<Card.Title>Request id: {request.id}</Card.Title>
				<Card.Body>
					<p><b>Title: </b>{request.title}</p>
					<p><b>Description: </b>{request.description}</p>
					<p><b>Animal: </b>{animalName}</p>
				</Card.Body>
			</Card>
			<Button
				onClick={deleteRequest}
				className={`${myButton.myButtonInv} mt-3`}>
					Delete Request
			</Button>
		</Col>
	)
}

export default Request