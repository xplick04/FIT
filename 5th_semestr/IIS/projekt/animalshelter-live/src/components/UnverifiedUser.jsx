import React from 'react'
import { useState, useContext } from 'react'
import axios from 'axios'

import {
	Form,
	Button,
	Card,
	Col,
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import styles from '../styles/Admin.module.css'
import mybutton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const UnverifiedUser = ({user, getUsers}) => {

	let {authTokens} = useContext(AuthContext);

	const verifyUser = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'POST',
			url: configData.API_URL + 'user-verify/' + user.id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then(() => {
			getUsers();
		});
	};

	return (
		<Col className={`${styles.Col}`}>
			<Card className={`${styles.Card} ${colors.Dark} shadow`}>
				{(user?.photo) &&
					<Card.Img
						className={`${styles.Image} rounded-top`}
						src={user.photo}>
					</Card.Img>
				}
				<div style={{height: "100%"}}></div>
				<Card.Body>
					<Form>
						<Form.Group>
							<Form.Label>Username</Form.Label>
							<Form.Control
								type='text' disabled
								name='username'
								value={user.username}/>
						</Form.Group>
						<Form.Group>
							<Form.Label>First name</Form.Label>
							<Form.Control
								type='text' disabled
								name='first_name'
								value={user.first_name}/>
						</Form.Group>
						<Form.Group>
							<Form.Label>Last name</Form.Label>
							<Form.Control
								type='text' disabled
								name='last_name'
								value={user.last_name}/>
						</Form.Group>
						<Form.Group>
							<Form.Label>Type</Form.Label>
							<Form.Control
								type='text' disabled
								name='type'
								value={user.type}/>
						</Form.Group>
						<Form.Group>
							<Form.Label>Date of birth</Form.Label>
							<Form.Control
								type='date' disabled
								name='date_of_birth'
								value={user.date_of_birth}/>
						</Form.Group>
						<Button
							className={`${mybutton.myButton} mt-3`}
							onClick={verifyUser} type='submit'>
								Verify user
							</Button>
					</Form>
				</Card.Body>
			</Card>
		</Col>
	)
}

export default UnverifiedUser
