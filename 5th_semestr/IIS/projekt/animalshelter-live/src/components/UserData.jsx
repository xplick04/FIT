import React from 'react'
import { useState, useEffect, useContext } from 'react'
import axios from 'axios'

import {
	Form,
	Button
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import styles from '../styles/UserDetail.module.css'
import myButton from '../styles/Button.module.css'

const UserData = () => {

	let {user, authTokens} = useContext(AuthContext);

	const [currentUser, setCurrentUser] = useState({});

	const [submit, setSubmit] = useState(false);

	const updateUser = (e) => {
		setCurrentUser({
			...currentUser,
			[e.target.name]:e.target.value
		});
	};

	const getUserData = async () => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'user-detail/' + user.user_id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};

		axios(options).then((response) => {
			setCurrentUser(response.data);
		});
	};

	useEffect(() => {
		getUserData();
	}, []);

	const sendUser = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'POST',
			url: configData.API_URL + 'user-detail/' + user.user_id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data: currentUser
		};

		axios(options).then((response) => {
			setSubmit(true);
		});
	};

	return (
		<Form className={`${styles.Form} p-5`} onSubmit={sendUser}>
			<Form.Group>
				<Form.Label>
					Username<br></br>
					<i style={{fontSize: "12px"}}>can not be changed</i>
				</Form.Label>
				<Form.Control
					type='text' disabled
					name='username'
					value={user.username}/>
			</Form.Group>
			<Form.Group>
				<Form.Label>First name</Form.Label>
				<Form.Control
					type='text'
					name='first_name'
					onChange={updateUser}
					value={currentUser.first_name}/>
			</Form.Group>
			<Form.Group>
				<Form.Label>Last name</Form.Label>
				<Form.Control
					type='text'
					name='last_name'
					onChange={updateUser}
					value={currentUser.last_name}/>
			</Form.Group>
			<Form.Group>
				<Form.Label>Date of birth</Form.Label>
				<Form.Control
					type='date'
					name='date_of_birth'
					onChange={updateUser}
					value={currentUser.date_of_birth}/>
				<></>
			</Form.Group>
			<Button
				type='submit'
				className={`${myButton.myButtonInv} mt-3`}>
					Submit
			</Button>
			{(submit) &&
				<p className='mt-3'>User has been updated.</p>
			}
		</Form>
	)
}

export default UserData
