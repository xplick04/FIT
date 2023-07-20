import React from 'react'
import { useState, useContext } from 'react'
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

const NewSchedule = ({show, handleClose, animal_id, getSchedules}) => {

	let {authTokens} = useContext(AuthContext);

	const [collision, setCollision] = useState(false);

	const createSchedule = async (e) => {
		e.preventDefault();

		let options = {
			method: 'POST',
			url: configData.API_URL + 'schedule-create/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data:{
				date	:e.target.date.value,
				s_from	:e.target.s_from.value,
				s_to	:e.target.s_to.value,
				animal	:animal_id
			}
		};
		axios(options).then((response) => {
			if(response.data === 'Collision.'){
				setCollision(true);
			}else{
				getSchedules();
				handleClose();
			}
		});
	};

	return (
		<Modal show={show} onHide={handleClose}
			className={`${styles.ModalNew} ${colors.Dark}`}>
			<Modal.Header>
				<Modal.Title>New Schedule</Modal.Title>
			</Modal.Header>
			<Modal.Body>
				<Form onSubmit={createSchedule}>
					<Form.Group>
						<Form.Label>Date</Form.Label>
						<Form.Control
							type='date'
							name='date'/>
					</Form.Group>
					<Form.Group>
						<Form.Label>From</Form.Label>
						<Form.Control
							type='time'
							name='s_from'/>
					</Form.Group>
					<Form.Group>
						<Form.Label>To</Form.Label>
						<Form.Control
							type='time'
							name='s_to'/>
					</Form.Group>
					<Button
						type='submit'
						className={`${myButton.myButton} mt-3`}>
							Create new schedule
					</Button>
					{(collision) &&
						<p>Not a valis time for schedule.</p>
					}
				</Form>
			</Modal.Body>
		</Modal>
	)
}

export default NewSchedule
