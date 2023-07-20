import React from 'react'
import { useContext } from 'react'
import axios from 'axios'

import {
	Modal,
	Form,
	Button,
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import myButton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const NewRequest = ({show, handleClose, animal_id}) => {

	let {authTokens} = useContext(AuthContext);

	const createRequest = async (e) => {
		e.preventDefault();

		const date = new Date(Date.now());
		const today = date.toISOString().split('T')[0];

		let options = {
			method: 'POST',
			url: configData.API_URL + 'request-create/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data:{
				date		:today,
				title		:e.target.title.value,
				description	:e.target.description.value,
				animal		:animal_id
			}
		};
		axios(options);

		handleClose();
	};

	return (
		<Modal
			className={`${colors.Dark}`}
			centered show={show} onHide={handleClose}>
			<Modal.Header>
				<Modal.Title>New Request</Modal.Title>
			</Modal.Header>
			<Modal.Body>
				<Form onSubmit={createRequest}>
					<Form.Group>
						<Form.Label>*Title</Form.Label>
						<Form.Control
							type='text'
							name='title'/>
					</Form.Group>
					<Form.Group>
						<Form.Label>*Description</Form.Label>
						<Form.Control
							type='text'
							name='description'/>
					</Form.Group>
					<Button
						type='submit'
						className={`${myButton.myButton} mt-3`}>
							Submit
					</Button>
				</Form>
			</Modal.Body>
		</Modal>
	)
}

export default NewRequest
