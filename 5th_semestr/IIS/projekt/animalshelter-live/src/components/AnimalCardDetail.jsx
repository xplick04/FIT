import React from 'react'

import {
	Modal,
} from 'react-bootstrap'

import colors from '../styles/Colors.module.css'

const AnimalCard = ({show, handleClose, animal}) => {
	return (
		<Modal
			className={`${colors.Dark}`}
			centered show={show} onHide={handleClose}>
			<Modal.Header>
				<Modal.Title>{animal.name}</Modal.Title>
			</Modal.Header>
			<Modal.Body>
				<p>
					<b>Breed: </b>
					{animal.breed}
				</p>
				<p>
					<b>Date of birth: </b>
					{animal.date_of_birth}
				</p>
				<p>
					<b>Description: </b><br/>
					{animal.description}
				</p>
				<p>
					<b>Found information: </b><br/>
					{animal.found_info}
				</p>
			</Modal.Body>
		</Modal>
	)
}

export default AnimalCard
