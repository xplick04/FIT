import React from 'react'
import { useState, useEffect, useContext } from 'react'

import {
	Container,
	Button,
	Row,
	Col,
	Form
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import UserData from '../components/UserData'
import Register from '../components/Register'
import Login from '../components/Login'

import styles from '../styles/UserDetail.module.css'
import myButton from '../styles/Button.module.css'
import container from '../styles/Container.module.css'
import axios from 'axios'

const UserDetail = () => {

	let {logoutUser, user, authTokens} = useContext(AuthContext);

	let first_name = 'User';
	if(user && user.first_name !== ''){
		first_name = user.first_name;
	}


	const [showReg, setShowReg] = useState(false);

	const handleShowReg = () => setShowReg(true);
	const handleCloseReg = () => setShowReg(false);

	const [showLog, setShowLog] = useState(false);

	const handleShowLog = () => setShowLog(true);
	const handleCloseLog = () => setShowLog(false);

	if(user?.verified === 'False'){
		alert('You are not a verified user. Wait a bit for our caretakers to verify you or contact us at animal.shelter@gmail.com.');
		logoutUser();
	}

	//get photo
	const [userPhoto, setUserPhoto] = useState();

	const getPhoto = async () => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'user-detail/' + user.user_id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then((response) => {
			setUserPhoto(response.data.photo);
		});
	};

	useEffect(() => {
		getPhoto();
	}, []);

	//upload photo
	const [photo, setPhoto] = useState();

	const uploadPhoto = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'POST',
			url: configData.API_URL + 'upload-photo/' + user.user_id + '/',
			headers: {
				'Content-Type':'multipart/form-data',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data: {
				photo: photo
			}
		};

		axios(options).then(() => {
			window.location.reload(false);
		});
	};

	const deleteUser = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'DELETE',
			url: configData.API_URL + 'user-delete/' + user.user_id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};

		axios(options).then(() => {
			logoutUser();
			window.location.reload(false);
		});
	};

	return (
		<Container className={`${container.Container} p-5 shadow rounded`}>
			{(user)
				?(
				<>
				<h1 className={`${styles.Center}`}>
					Hello, {first_name}
				</h1>
				<Row className={`${styles.Row} p-5 mt-5`}>
					<Col
						className='d-flex justify-content-center align-items-center'>
						<UserData/>
					</Col>
					<Col>
						<p><b>Type of user: </b>{user?.type}</p>
						{(userPhoto) &&
							<img
								src={userPhoto}
								className={`${styles.Img} img-fluid shadow rounded`}/>
						}
						<Form>
							<Form.Group>
								<Form.Label>Upload your photo here:</Form.Label>
								<Form.Control
									type='file'
									onChange={
										(e) => setPhoto(e.target.files[0])
									}/>
							</Form.Group>
							<Button
								type='submit'
								onClick={uploadPhoto}
								className={`${myButton.myButtonInv} mt-3`}>
									Upload photo
							</Button>
						</Form>
					</Col>
				</Row>
				</>
				):(
				<>
				<h1 className={`${styles.Center}`}>
					You are not logged in.
				</h1>
				<p className={`${styles.Center}`}>
					Login or register new user bellow please.
				</p>
				</>
				)
			}
			<Row>
				<div className={`${styles.Div}`}>
					<Register show={showReg} handleClose={handleCloseReg}/>
					<Button
						className={`${myButton.myButtonInv}`}
						onClick={handleShowReg}>
							Register New User
					</Button>
					<Login data={[showLog, handleCloseLog]}/>
					{(user)
						?(
							<>
								<Button
									className={`${myButton.myButtonInv}`}
									onClick={logoutUser}>
										Logout
								</Button>
								<Button
									className={`${myButton.myButtonInv}`}
									onClick={deleteUser}>
										Delete User Profile
								</Button>
							</>
						):(
							<Button
								className={`${myButton.myButtonInv}`}
								onClick={handleShowLog}>
									Login
							</Button>
						)
					}
				</div>
			</Row>
		</Container>
	)
}

export default UserDetail
