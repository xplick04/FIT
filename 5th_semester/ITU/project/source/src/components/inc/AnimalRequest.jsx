/**
 * @brief AnimalRequest component file
 * 
 * @file AnimalRequest.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext, useEffect } from 'react'
import { Form, Modal, Container } from "react-bootstrap";
import { AuthContext } from '../../context/AuthContext';
import axios from 'axios';

export const AnimalRequest = ({data}) => {

    //url for axios request
    const ANIMAL_DETAIL_URL = "https://sigma-animal-shelter.herokuapp.com/api/animal-detail/";

    //getting JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);

    //creating animal
    let [state, setState] = useState({
        name : '',
        breed : '',
        description : '',
        found_info : '',
        date_of_birth : '',
        sex: '',
    });

    //getting particular animal data from db
    let getAnimalData = () => {
        let options = {
            method: 'GET',
            url: ANIMAL_DETAIL_URL + data[2]?.animal + "/",
            withCredentials: true,
            headers: {
                'Content-Type':'application/json',
                'Authorization':'Bearer ' + String(authTokens?.access)
            }
        };
        axios(options).then((response) => {
            setState(response.data);
        });
    };

    //after load
    useEffect(() => {
        getAnimalData();
    }, []);

  return (
    <Modal show={data[0]} onHide={data[1]} centered="true">
    <Modal.Header closeButton>
    </Modal.Header>
    <Modal.Body>
    <Container className="" style={{padding:"30px"}}>
        <Form>
              <Form.Group className="">
              <Form.Label className="login-text mb-0">Name:</Form.Label>
              <Form.Control type="text" value={state.name} disabled/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Breed:</Form.Label>
              <Form.Control type="text" value={state.breed} disabled/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Date of birth:</Form.Label>
              <Form.Control type="text" value={state.date_of_birth} 
              max={new Date().toISOString().split("T")[0]} disabled/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Description:</Form.Label>
              <Form.Control type="text" value={state.description} disabled/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Found informations:</Form.Label>
              <Form.Control type="text" value={state.found_info} disabled/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Sex:</Form.Label>
              <Form.Control type="text" value={state.sex} disabled/>
              </Form.Group>
            </Form>
      </Container>
    </Modal.Body>
    </Modal>
  )
}
