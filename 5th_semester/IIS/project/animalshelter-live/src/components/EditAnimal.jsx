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

const EditAnimal = ({show, handleClose, animal, getAnimals}) => {

	let {authTokens} = useContext(AuthContext);

	const [animalEdit, setAnimalEdit] = useState(animal);
	const [updatedImage, setUpdatedImage] = useState(false);

	const updateAnimalEdit = (e) => {
		if(e.target.name === 'photo'){
			setUpdatedImage(true);
			setAnimalEdit({
				...animalEdit,
				[e.target.name]:e.target.files[0]
			});
		}else{
			setAnimalEdit({
				...animalEdit,
				[e.target.name]:e.target.value
			});
		}
	};

	const updateAnimal = async (e) => {
		e.preventDefault();
		
		let options = {};
		if(updatedImage){
			options = {
				method: 'POST',
				url: configData.API_URL + 'animal-detail/' + animal.id + '/',
				headers: {
					'Content-Type':'multipart/form-data',
					'Authorization':'Bearer ' + String(authTokens.access)
				},
				data: animalEdit
			};
		}else{
			options = {
				method: 'POST',
				url: configData.API_URL + 'animal-detail/' + animal.id + '/',
				headers: {
					'Content-Type':'application/json',
					'Authorization':'Bearer ' + String(authTokens.access)
				},
				data:{
					name			:animalEdit.name,
					breed			:animalEdit.breed,
					sex				:animalEdit.sex,
					date_of_birth	:animalEdit.date_of_birth,
					description		:animalEdit.description,
					found_info		:animalEdit.found_info,
				}
			};
		}

		axios(options).then(() => {
			handleClose();
			getAnimals();
		});
	};

	const deleteAnimal = async (e) => {
		e.preventDefault();
		
		const options = {
			method: 'DELETE',
			url: configData.API_URL + 'animal-delete/' + animal.id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then(() => {
			handleClose();
			getAnimals();
		});
	};

	return (
		<Modal
			className={`${colors.Dark}`}
			show={show} onHide={handleClose} centered>
			<Modal.Header>
				<Modal.Title>Edit {animal.name}</Modal.Title>
			</Modal.Header>
			<Modal.Body>
				<Form onSubmit={updateAnimal}>
					<Form.Group>
						<Form.Label>Name</Form.Label>
						<Form.Control
							type='text'
							name='name'
							onChange={updateAnimalEdit}
							value={animalEdit.name}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>Breed</Form.Label>
						<Form.Control
							type='text'
							name='breed'
							onChange={updateAnimalEdit}
							value={animalEdit.breed}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>Sex</Form.Label>
						<Form.Control
							type='text'
							name='sex'
							onChange={updateAnimalEdit}
							value={animalEdit.sex}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>Date of birth</Form.Label>
						<Form.Control
							type='date'
							name='date_of_birth'
							onChange={updateAnimalEdit}
							value={animalEdit.date_of_birth}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>Description</Form.Label>
						<Form.Control
							type='text'
							name='description'
							onChange={updateAnimalEdit}
							value={animalEdit.description}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>Found information</Form.Label>
						<Form.Control
							type='text'
							name='found_info'
							onChange={updateAnimalEdit}
							value={animalEdit.found_info}/>
					</Form.Group>
					<Form.Group>
						<Form.Label>Animal photo</Form.Label>
						<Form.Control
							type='file'
							name='photo'
							onChange={updateAnimalEdit}/>
					</Form.Group>
					<Button
						type='submit'
						className={`${myButton.myButton} m-3`}>
							Edit animal
					</Button>
					<Button
						onClick={deleteAnimal}
						className={`${myButton.myButton} m-3`}>
							Delete animal
					</Button>
				</Form>
			</Modal.Body>
		</Modal>
	)
}

export default EditAnimal
