import React from 'react'
import { useState } from 'react'
import axios from 'axios'

import {
	Form,
	Modal,
	Button
} from 'react-bootstrap'

import configData from '../config.json'

import myButton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const Register = ({show, handleClose}) => {

	const register = async (e) => {
		e.preventDefault();

		const options = {
			method: 'POST',
			url: configData.API_URL + 'register/',
			headers: {
				'Content-Type':'application/json',
			},
			data:{
				username:e.target.username.value,
				password:e.target.password.value,
				first_name:e.target.first_name.value,
				last_name:e.target.last_name.value
			}
		};
		axios(options).then((response) =>{
			if(response.data === 'User is not registered.'){
				alert('This username is not available.');
			}else{
				handleClose();
			}
		});
	};

  return (
	<Modal
		className={`${colors.Dark}`}
		centered show={show} onHide={handleClose}>
		<Modal.Header>
			<Modal.Title>Registration</Modal.Title>
		</Modal.Header>
		<Modal.Body>
			<Form onSubmit={register}>
				<Form.Group>
					<Form.Label>*Username</Form.Label>
					<Form.Control
						type='text'
						name='username'/>
				</Form.Group>
				<Form.Group>
					<Form.Label>*Password</Form.Label>
					<Form.Control 
						type='password'
						name='password'/>
				</Form.Group>
				<Form.Group>
					<Form.Label>First name</Form.Label>
					<Form.Control 
						type='text'
						name='first_name'/>
				</Form.Group>
				<Form.Group>
					<Form.Label>Last name</Form.Label>
					<Form.Control 
						type='text'
						name='last_name'/>
				</Form.Group>
				<Button
					type='submit'
					className={`${myButton.myButton} mt-3`}>
						Register User
				</Button>
			</Form>
		</Modal.Body>
	</Modal>
  )
}

export default Register
