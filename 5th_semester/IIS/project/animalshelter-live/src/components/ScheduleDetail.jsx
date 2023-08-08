import React from 'react'
import { useState, useEffect, useContext } from 'react'
import axios from 'axios'

import {
	Card,
	Col,
	Row,
	Form,
	Button,
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import Reservation from './Reservation'

import myButton from '../styles/Button.module.css'

const ScheduleDetail = ({schedule}) => {

	let {user, authTokens} = useContext(AuthContext);

	const [reservations, setReservations] = useState([]);

	const [collision, setCollision] = useState(false);

	const getReservations = async () => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'reservation-list/' + schedule.id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then((response) => {
				setReservations(response.data)
			});
	};

	useEffect(() => {
		getReservations();
	}, []);

	const createReservation = (e) => {
		e.preventDefault();

		const options = {
			method: 'POST',
			url: configData.API_URL + 'reservation-create/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data:{
				r_from	:e.target.r_from.value,
				r_to	:e.target.r_to.value,
				schedule:schedule.id,
				user	:user.user_id
			}
		};
		axios(options).then((response) => {
			if(response.data === 'Collision.'){
				setCollision(true);
			}else{
				setCollision(false);
			}
			getReservations();
		});
	};

	return (
		<Card className='m-3'>
			<Card.Body>
				<Card.Title className='m-2'><b>Date: </b>{schedule.date}</Card.Title>
				<div className='d-flex'>
					<Card.Title className='m-2'>
						<b>From: </b>{schedule.s_from}
					</Card.Title>
					<Card.Title className='m-2'>
						<b>To: </b>{schedule.s_to}
					</Card.Title>
				</div>
				<Row>
					<Col>
						{(reservations?.length > 0
							&& Array.isArray(reservations)) &&
							<>
							{reservations.map((reservation) => (
								<Reservation
									getReservations={getReservations}
									reservation={reservation}
									key={reservation.id}/>
							))}
							</>
						}
					</Col>
					{(user.type === 'Registered') &&
						<Col>
							<Form onSubmit={createReservation}>
								<Form.Group>
									<Form.Label>From</Form.Label>
									<Form.Control
										type='time'
										name='r_from'/>
								</Form.Group>
								<Form.Group>
									<Form.Label>To</Form.Label>
									<Form.Control
										type='time'
										name='r_to'/>
								</Form.Group>
								<Button
									type='submit'
									className={`${myButton.myButton} mt-3`}>
										Submit
								</Button>
								{(collision) &&
									<p className='m-3'>Not a valid time for reservation.</p>
								}
							</Form>
						</Col>
					}
				</Row>
			</Card.Body>
		</Card>
	)
}

export default ScheduleDetail
