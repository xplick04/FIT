import React from 'react'
import { useState, useContext } from 'react'
import axios from 'axios'

import {
	Modal,
	Form,
	Button,
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import myButton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const CreateAnimal = ({show, handleClose, getAnimals}) => {

	let {authTokens} = useContext(AuthContext);

	const [animal, setAnimal] = useState({});

	const updateAnimal = (e) => {
		if(e.target.name === 'photo'){
			setAnimal({
				...animal,
				[e.target.name]:e.target.files[0]
			});
		}else{
			setAnimal({
				...animal,
				[e.target.name]:e.target.value
			});
		}
	};

	const createAnimal = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'POST',
			url: configData.API_URL + 'animal-create/',
			headers: {
				'Content-Type':'multipart/form-data',
				'Authorization':'Bearer ' + String(authTokens.access)
			},
			data: animal
		};

		axios(options).then(() => {
			handleClose();
			getAnimals();
			setAnimal({});
		});
	};

	return (
		<Modal
			className={`${colors.Dark}`}
			show={show} onHide={handleClose} centered>
			<Modal.Header>
				<Modal.Title>Create new animal</Modal.Title>
			</Modal.Header>
			<Modal.Body>
				<Form onSubmit={createAnimal}>
					<Form.Group>
						<Form.Label>*Name</Form.Label>
						<Form.Control
							type='text'
							name='name'
							onChange={updateAnimal}
							value={animal.name}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>*Breed</Form.Label>
						<Form.Control
							type='text'
							name='breed'
							onChange={updateAnimal}
							value={animal.breed}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>*Sex</Form.Label>
						<Form.Control
							type='text'
							name='sex'
							onChange={updateAnimal}
							value={animal.sex}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>*Date of birth</Form.Label>
						<Form.Control
							type='date'
							name='date_of_birth'
							onChange={updateAnimal}
							value={animal.date_of_birth}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>*Description</Form.Label>
						<Form.Control
							type='text'
							name='description'
							onChange={updateAnimal}
							value={animal.description}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>*Found information</Form.Label>
						<Form.Control
							type='text'
							name='found_info'
							onChange={updateAnimal}
							value={animal.found_info}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>Animal photo</Form.Label>
						<Form.Control
							type='file'
							name='photo'
							onChange={updateAnimal}/>
					</Form.Group>
					<Button
						type='submit'
						className={`${myButton.myButton} mt-3`}>
							Create Animal
					</Button>
				</Form>
			</Modal.Body>
		</Modal>
	)
}

export default CreateAnimal
