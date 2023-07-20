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

import styles from '../styles/Animals.module.css'
import myButton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const NewRecord = ({show, handleClose, animal_id, getRecords}) => {

	let {user, authTokens} = useContext(AuthContext);

	const createRecord = async (e) => {
		e.preventDefault();

		const date = new Date(Date.now());
		const today = date.toISOString().split('T')[0];

		let options = {
			method: 'POST',
			url: configData.API_URL + 'record-create/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data:{
				title		:e.target.title.value,
				description	:e.target.description.value,
				animal		:animal_id,
				user		:user.id,
				date		:today
			}
		};
		axios(options).then(() => {
			getRecords();
			handleClose();
		});
	};

	return (
		<Modal show={show} onHide={handleClose}
			className={`${styles.ModalNew} ${colors.Dark}`}>
			<Modal.Header>
				<Modal.Title>New Record</Modal.Title>
			</Modal.Header>
			<Modal.Body>
				<Form onSubmit={createRecord}>
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
							Create new record
					</Button>
				</Form>
			</Modal.Body>
		</Modal>
	)
}

export default NewRecord
