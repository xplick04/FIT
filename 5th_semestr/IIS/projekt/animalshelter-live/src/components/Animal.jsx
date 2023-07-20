import React from 'react'
import { useState, useContext } from 'react'

import {
	Card,
	Col,
	Row,
	Button
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import Schedule from './Schedule'
import EditAnimal from './EditAnimal'
import NewRequest from './NewRequest'
import Records from './Records'

import styles from '../styles/Animals.module.css'
import myButton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const Animal = ({animal, getAnimals}) => {

	let {user} = useContext(AuthContext);

	// controls for modals
	const [showRequest, setShowRequest] = useState(false);
	const handleShowRequest = () => setShowRequest(true);
	const handleCloseRequest = () => setShowRequest(false);

	const [showEdit, setShowEdit] = useState(false);
	const handleShowEdit = () => setShowEdit(true);
	const handleCloseEdit = () => setShowEdit(false);

	const [showSchedule, setShowSchedule] = useState(false);
	const handleShowSchedule = () => setShowSchedule(true);
	const handleCloseSchedule = () => setShowSchedule(false);

	const [showRecord, setShowRecord] = useState(false);
	const handleShowRecord = () => setShowRecord(true);
	const handleCloseRecord = () => setShowRecord(false);

	return (
		<Card
			className={`${colors.Dark} mt-5 p-3 shadow`}>
			<Row>
				<Col>
					<Card.Body>
						<Card.Title><b>{animal.name}</b></Card.Title>
						<Card.Title>{animal.breed}</Card.Title>
						<Card.Title><b>Sex: </b>{animal.sex}</Card.Title>
						<Card.Title>
							<b>Date of birth: </b>{animal.date_of_birth}
						</Card.Title>
						<Card.Text>{animal.description}</Card.Text>
						<Card.Text>{animal.found_info}</Card.Text>
						<div className={`${styles.Div}`}>
							{(user?.type === 'Caretaker' ||
								user?.type === 'Registered') &&
								<>
								<Schedule
									show={showSchedule}
									handleClose={handleCloseSchedule}
									animal_id={animal.id}/>
								<Button
									className={`${myButton.myButton} m-1`}
									onClick={handleShowSchedule}>
										Show me schedule
								</Button>
								</>
							}
							{(user?.type === 'Caretaker') &&
								<>
								<EditAnimal
									show={showEdit}
									handleClose={handleCloseEdit}
									getAnimals={getAnimals}
									animal={animal}/>
								<Button
									onClick={handleShowEdit}
									className={`${myButton.myButton} m-1`}
									>
										Edit animal
								</Button>
								<NewRequest
									show={showRequest}
									handleClose={handleCloseRequest}
									animal_id={animal.id}/>
								<Button
									className={`${myButton.myButton} m-1`}
									onClick={handleShowRequest}>
										Add request
								</Button>
								</>
							}
							{(user?.type === 'Veterinarian') &&
								<>
								<Records
									show={showRecord}
									handleClose={handleCloseRecord}
									animal_id={animal.id}/>
								<Button
									className={`${myButton.myButton} m-1`}
									onClick={handleShowRecord}>
										Show me medical records
								</Button>
								</>
							}
						</div>
					</Card.Body>
				</Col>
				<Col
					className='d-flex justify-content-center align-items-center'>
					{(animal.photo) &&
						<Card.Img
							className={`${styles.Img}`}
							src={animal.photo}/>
					}
				</Col>
			</Row>
		</Card>
	)
}

export default Animal
