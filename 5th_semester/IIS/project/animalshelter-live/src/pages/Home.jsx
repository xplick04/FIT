import React from 'react'
import { useState, useEffect } from 'react'
import axios from 'axios'

import {
	Container,
	Row,
} from 'react-bootstrap'

import configData from '../config.json'

import AnimalCard from '../components/AnimalCard'

import styles from '../styles/Home.module.css'
import container from '../styles/Container.module.css'

const Home = () => {

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
			setAnimals(response.data);
		});
	};

	useEffect(() => {
		getAnimals();
	}, []);

	return (
		<Container
			className={`${container.Container} p-5 shadow rounded`}>
			<h1 className={`${styles.Header}`}>Animal Shelter</h1>
			<p className={`${styles.Text}`}>Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam erat volutpat. Integer lacinia. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Integer lacinia. Curabitur ligula sapien, pulvinar a vestibulum quis, facilisis vel sapien. Morbi leo mi, nonummy eget tristique non, rhoncus non leo. Aenean fermentum risus id tortor. Fusce dui leo, imperdiet in, aliquam sit amet, feugiat eu, orci. Et harum quidem rerum facilis est et expedita distinctio. Mauris elementum mauris vitae tortor. Et harum quidem rerum facilis est et expedita distinctio. Mauris tincidunt sem sed arcu. Etiam ligula pede, sagittis quis, interdum ultricies, scelerisque eu. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.</p>
			<p className={`${styles.Text}`}>Curabitur vitae diam non enim vestibulum interdum. Integer lacinia. Mauris suscipit, ligula sit amet pharetra semper, nibh ante cursus purus, vel sagittis velit mauris vel metus. Etiam commodo dui eget wisi. Vivamus luctus egestas leo. Quisque porta. Et harum quidem rerum facilis est et expedita distinctio. Sed ac dolor sit amet purus malesuada congue. Duis ante orci, molestie vitae vehicula venenatis, tincidunt ac pede. In dapibus augue non sapien. Nulla pulvinar eleifend sem. Aliquam erat volutpat. Nullam eget nisl.</p>
			<div>
				{(animals?.length > 0 && Array.isArray(animals)) &&
					<Row className={`${styles.Row}`}>
						{animals.slice(0, 4).map((animal) => (
							<AnimalCard animal={animal} key={animal.id + animal.name}/>
						))}
					</Row>
				}
			</div>
		</Container>
	)
}

export default Home
