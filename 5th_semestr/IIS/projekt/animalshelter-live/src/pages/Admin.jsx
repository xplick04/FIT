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

import User from '../components/User'

import styles from '../styles/Admin.module.css'
import container from '../styles/Container.module.css'

const Admin = () => {

	let {authTokens} = useContext(AuthContext);

	const [users, setUsers] = useState([]);

	const getUsers = async () => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'user-list/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then((response) => {
			setUsers(response.data);
		});
	};

	useEffect(() => {
		getUsers();
	}, []);

	return (
		<Container
			className={`${container.Container} mt-5 mb-5 p-5 shadow rounded`}>
			{(users?.length > 0 && Array.isArray(users))
				?(
					<>
						{toChunks(users).map((new_users) => (
							<Row className={`${styles.Row} mt-4`}>
								{new_users.map((user) => (
									<User
										user={user}
										getUsers={getUsers}
										key={user.id + user.username}/>
								))}
							</Row>
						))}
					</>
				):(<></>)
			}
		</Container>
	)
}

export default Admin
