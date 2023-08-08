/**
 * @brief OurAnimals page component file
 * 
 * @file OurAnimals.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import axios from 'axios';
import React, { useState, useEffect, useContext } from 'react'
import { Container } from 'react-bootstrap';
import { DisplayAnimal } from '../components/inc/DisplayAnimal';
import { AuthContext } from "../context/AuthContext";
import { AddAnimal } from '../components/inc/AddAnimal';

export const OurAnimals = () => {

  //url for axios request
  const ANIMAL_LIST_URL = 'https://sigma-animal-shelter.herokuapp.com/api/animal-list/';

  //getting user informations from AuthContext
  let {user} = useContext(AuthContext);

  //modal window for adding animal
  const [show, setShow] = useState(false);

  //modal window, click => hide, click => show
  const handleClick = () => {
      setShow(current => !current)
  };

  //creating animal list
  let [animalState, setAnimalState] = useState({
      animals: []
    });
  
  //getting all animals from db
  let getAnimalList = () => {
      axios.get(ANIMAL_LIST_URL).then((response) => {
      setAnimalState({
        ...animalState,
        animals: response.data
      });
    });
  };

  //get animal list after load
  useEffect(() => {
    getAnimalList();
  }, []);


  return (
    <div className='mt-4 mb-4'>
      {user?.type === "Caretaker" && 
      <div className='d-flex justify-content-center'>
        <button className='hyper-text' onClick={handleClick}>ADD</button>
        <AddAnimal dataAdd={[show, handleClick]} />
      </div>
      }
      <Container className='mt-4 mb-4'>{
          animalState.animals.map(animal => {
            return (
              <DisplayAnimal data={[animal, getAnimalList]} />
            )
          })
        }
      </Container>
    </div>
  )
};
