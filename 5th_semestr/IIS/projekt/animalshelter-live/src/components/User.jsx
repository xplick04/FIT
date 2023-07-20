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
import input from '../styles/Input.module.css'

const User = ({user, getUsers}) => {

	let {authTokens} = useContext(AuthContext);

	const [type, setType] = useState(user.type);

	const [updated, setUpdated] = useState(false);

	const updateType = (e) => {
		setType(e.target.value);
	};

	const saveType = (e) => {
		e.preventDefault();

		const options = {
			method: 'POST',
			url: configData.API_URL + 'user-type/' + user.id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data:{
				type:type
			}
		};
		axios(options);

		setUpdated(true);
	};

	const deleteUser = (e) => {
		e.preventDefault();
		
		const options = {
			method: 'DELETE',
			url: configData.API_URL + 'user-delete/' + user.id + '/',
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
				{(user?.photo)
					?
						<Card.Img
							className={`${styles.Image} rounded-top`}
							src={user.photo}>
						</Card.Img>
					:
						<div style={{height: "300px", width: "100%"}}></div>
				}
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
							<Form.Label>Type of user</Form.Label>
							<Form.Select
								className={`${input.Select}`}
								value={type}
								name='type'
								onChange={updateType}>
									<option value="Registered">
										Registered
									</option>
									<option value="Caretaker">
										Caretaker
									</option>
									<option value="Veterinarian">
										Veterinarian
									</option>
									<option value="Administrator">
										Administrator
									</option>
							</Form.Select>
						</Form.Group>
						<Form.Group>
							<Form.Label>Date of birth</Form.Label>
							<Form.Control
								type='date' disabled
								name='date_of_birth'
								value={user.date_of_birth}/>
						</Form.Group>
						<div style={{height: "30px"}}>
							{(updated) &&
								<p>User has been updated.</p>
							}
						</div>
						<Button
							className={`${mybutton.myButton} mt-3`}
							onClick={saveType}
							type='submit'>Save changes</Button>
						<br></br>
						<Button
							className={`${mybutton.myButton} mt-3`}
							onClick={deleteUser}
							type='submit'>Delete user</Button>
					</Form>
				</Card.Body>
			</Card>
		</Col>
	)
}

export default User
