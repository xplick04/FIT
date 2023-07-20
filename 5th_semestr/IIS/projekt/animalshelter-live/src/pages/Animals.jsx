import React from 'react'
import { useState, useEffect, useContext } from 'react'
import axios from 'axios'

import {
	Button,
	Container,
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import Animal from '../components/Animal'
import CreateAnimal from '../components/CreateAnimal'

import styles from '../styles/Animals.module.css'
import container from '../styles/Container.module.css'
import myButton from '../styles/Button.module.css'

const Animals = () => {

	let {user} = useContext(AuthContext);

	const [animals, setAnimals] = useState([]);

	const getAnimals = async () => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'animal-list/',
			headers: {
				'Content-Type':'application/json',
			}
		};
		axios(options).then((response) => {
				setAnimals(response.data)
			});
	};

	useEffect(() => {
		getAnimals();
	}, []);

	const [show, setShow] = useState(false);
	const handleShow = () => setShow(true);
	const handleClose = () => setShow(false);

	return (
		<Container
			className={`${container.Container} p-5 shadow rounded`}>
			<h1 className={`${styles.Header}`}>Our Animals</h1>
			{(user?.type === 'Caretaker') &&
				<>
				<CreateAnimal
					show={show} handleClose={handleClose}
					getAnimals={getAnimals}/>
				<Button onClick={handleShow}
					className={`${myButton.myButtonInv} mt-5`}>
						Create new animal
				</Button>
				</>
			}
			{(animals?.length > 0 && Array.isArray(animals)) &&
				<>
				{animals.map((animal) => (
					<Animal getAnimals={getAnimals} animal={animal}
					key={animal.id + animal.name}/>
				))}
				</>
			}
		</Container>
	)
}

export default Animals
