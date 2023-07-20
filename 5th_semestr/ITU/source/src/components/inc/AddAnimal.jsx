/**
 * @brief AddAnimal component file
 * 
 * @file AddAnimal.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext, useEffect } from 'react'
import { AuthContext } from '../../context/AuthContext';
import { Form, Modal, Container} from "react-bootstrap";
import axios from 'axios';

export const AddAnimal = ({dataAdd}) => {

    //url for axios request
    const ADD_ANIMAL_URL = 'https://sigma-animal-shelter.herokuapp.com/api/animal-create/';

    //getting JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);

    //creating animal
    let [state, setState] = useState({
        name : '',
        breed : '',
        date_of_birth : '',
        description: '',
        found_info: '',
        sex: '',
        });
    
    //binding input values to animal
    let updateInput = (e) => {
        setState({
            ...state,
                [e.target.name] : e.target.value
        })
    };

    //animal add axios request
    let addAnimal = (e) => {
        e.preventDefault();
        let options = {
            method: 'POST',
            url: ADD_ANIMAL_URL,
            headers: {
                'Content-Type':'application/json',
                'Authorization':'Bearer ' + String(authTokens?.access)
            },
            data: {
                name : state.name,
                breed : state.breed,
                date_of_birth : state.date_of_birth,
                description: state.description,
                found_info: state.found_info,
                sex: state.sex,
            }
        };
        axios(options).then((response) => {
            if(response.data !== "Animal is not created."){
                window.location.reload(false);
            }
            else{
                alert("User was not added!");
            }
        });        
    };

    //after load
    useEffect(() => {
        state.sex = 'Boy'
    }, []);
        
  return (
    <>
    <Modal show={dataAdd[0]} onHide={dataAdd[1]} centered="true">
        <Modal.Header closeButton>
        </Modal.Header>
        <Modal.Body>
        <Container className="" style={{padding:"30px"}}>
           <Form onSubmit={addAnimal}>
                  <Form.Group className="">
                  <Form.Label className="login-text mb-0">*Name:</Form.Label>
                  <Form.Control name="name" onChange={updateInput} type="text" required/>
                  </Form.Group>

                  <Form.Group className="">
                  <Form.Label className="login-text mb-0">*Breed:</Form.Label>
                  <Form.Control name="breed" onChange={updateInput} type="text" required/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">*Date of Birth:</Form.Label>
                  <Form.Control name="date_of_birth" onChange={updateInput} type="date" 
                  max={new Date().toISOString().split("T")[0]} required/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">*Description:</Form.Label>
                  <Form.Control name="description" onChange={updateInput} type="text" required/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">*Found informations:</Form.Label>
                  <Form.Control name="found_info" onChange={updateInput} type="text" required/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">*Sex:</Form.Label>
                  <Form.Select onChange={updateInput} value={state.sex} required>
                    <option value="Girl">Girl</option>
                    <option value="Boy">Boy</option>
                    <option value="Unknown">Unknown</option>
                  </Form.Select>
                  </Form.Group>
                  <div className="mt-4 d-flex justify-content-center" style={{width:"100%"}}>
                    <button type="submit" className="submit-button">ADD ANIMAL</button>
                  </div>
                </Form>
        </Container>
        </Modal.Body>
    </Modal>
    </>
  )
}
