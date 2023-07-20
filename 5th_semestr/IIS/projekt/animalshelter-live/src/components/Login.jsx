import React from 'react'
import { useContext } from 'react'

import {
	Form,
	Modal,
	Button } from 'react-bootstrap'

import AuthContext from '../context/AuthContext'

import myButton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const Login = (props) => {

	let {loginUser, user} = useContext(AuthContext);

	return (
		<Modal
			className={`${colors.Dark}`}
			centered show={props.data[0]} onHide={props.data[1]}>
			<Modal.Header>
				<Modal.Title>Login</Modal.Title>
			</Modal.Header>
			<Modal.Body>
				<Form onSubmit={loginUser}>
					<Form.Group>
						<Form.Label>Username</Form.Label>
						<Form.Control
							type='text'
							name='username'/>
					</Form.Group>
					<Form.Group>
						<Form.Label>Password</Form.Label>
						<Form.Control 
							type='password'
							name='password'/>
					</Form.Group>
					<Button
						type='submit'
						className={`${myButton.myButton} mt-3`}>
							Submit
					</Button>
					{user &&
						<p>{user.username} has been logged in</p>
					}
				</Form>
			</Modal.Body>
		</Modal>
	)
}

export default Login
