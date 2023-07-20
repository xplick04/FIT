import React from 'react'
import { useContext } from 'react'
import axios from 'axios'

import {
	Button,
} from 'react-bootstrap';

import AuthContext from '../context/AuthContext';
import configData from '../config.json'

import styles from '../styles/Animals.module.css'
import myButton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const Reservation = ({reservation, getReservations}) => {

	let {user, authTokens} = useContext(AuthContext);

	const deleteReservation = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'DELETE',
			url: configData.API_URL + 'reservation-delete/' + reservation.id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then(() => {
			getReservations();
		});
	};

	const markAsConfirmed = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'POST',
			url: configData.API_URL + 'reservation-update/' + reservation.id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data:{
				borrowed:reservation.borrowed,
				returned:reservation.returned,
				confirmed:true
			}
		};
		axios(options).then(() => {
			getReservations();
		});
	};

	const markAsBorrowed = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'POST',
			url: configData.API_URL + 'reservation-update/' + reservation.id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data:{
				borrowed:true,
				returned:reservation.returned,
				confirmed:reservation.confirmed
			}
		};
		axios(options).then(() => {
			getReservations();
		});
	};

	const markAsReturned = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'POST',
			url: configData.API_URL + 'reservation-update/' + reservation.id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data:{
				borrowed:reservation.borrowed,
				returned:true,
				confirmed:reservation.confirmed
			}
		};
		axios(options).then(() => {
			getReservations();
		});
	};

	return (
		<div
			className='d-flex'
			style={(reservation.confirmed === false)?{opacity: "70%"}:{}}>
			<div
				style={(user?.type === 'Registered')?{width: "100%"}:{}}
				className={`
					${styles.UpperDiv} ${colors.BgLight} ${colors.White} rounded p-0 d-flex justify-content-center align-items-center m-1`}>
				<p className='p-0 m-1'>
					{reservation.r_from} - {reservation.r_to}
				</p>
			</div>
			<div
				style={(user?.type === 'Registered')?{display: "none"}:{}}
				className={`${styles.LowerDiv} p-0`}>
				{(user?.type === 'Caretaker') &&
					<>
					<Button
						onClick={deleteReservation}
						className={`${myButton.myButton} ${styles.Button} m-1`}>
						Delete reservation
					</Button>
					{(reservation.confirmed === false) &&
						<Button
							onClick={markAsConfirmed}
							className={`${myButton.myButton} ${styles.Button} m-1`}>
							Mark as confirmed
						</Button>
					}
					{(reservation.borrowed === false && reservation.confirmed === true) &&
						<Button
							onClick={markAsBorrowed}
							className={`${myButton.myButton} ${styles.Button} m-1`}>
							Mark as borrowed
						</Button>
					}
					{(reservation.returned === false && reservation.borrowed === true && reservation.confirmed === true) &&
						<Button
							onClick={markAsReturned}
							className={`${myButton.myButton} ${styles.Button} m-1`}>
							Mark as returned
						</Button>
					}
					</>
				}
			</div>
		</div>
	)
}

export default Reservation
