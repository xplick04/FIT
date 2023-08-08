import React from 'react'
import { useState } from 'react'

import {
	Card,
	Col
} from 'react-bootstrap'

import AnimalCardDetail from './AnimalCardDetail'
import configData from '../config.json'

import styles from '../styles/Home.module.css'
import colors from '../styles/Colors.module.css'

const AnimalCard = ({animal}) => {

	const [show, setShow] = useState(false);

	const handleShow = () => setShow(true);
	const handleClose = () => setShow(false);

	return (
		<>
			<AnimalCardDetail
				show={show}
				handleClose={handleClose}
				animal={animal}/>
			<Col className='m-1'>
				<Card
					onClick={handleShow}
					className={`
						${styles.Card}
						${colors.Light}
						${colors.DarkHover}
						shadow`}>
					<Card.Img
						className={`${styles.Image}`}
						src={animal.photo}/>
					<Card.Body>
						<Card.Title>{animal.name + ', ' + animal.breed}
						</Card.Title>
					</Card.Body>
				</Card>
			</Col>
		</>
	)
}

export default AnimalCard
