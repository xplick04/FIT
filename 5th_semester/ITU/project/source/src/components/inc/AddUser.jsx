/**
 * @brief AddUser component file
 * 
 * @file AddUser.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext, useEffect } from 'react'
import { Form, Modal, Container} from "react-bootstrap";
import axios from 'axios';
import { AuthContext } from '../../context/AuthContext';


export const AddUser = ({data}) => {

    //url for axios request
    const ADD_PROFILE_URL = 'https://sigma-animal-shelter.herokuapp.com/api/register/';

    //getting JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);

    //creating user
    let [state, setState] = useState({
        username : '',
        password : '',
        password2: '',
        first_name : '',
        last_name : '',
        date_of_birth : '',
        type: '',
    });

    //binding input values to user
    let updateInput = (e) => {
        setState({
            ...state,
                [e.target.name] : e.target.value
        })
    };

    //register user axios request(same as register except admin can set type)
    let addUser = (e) => {
        e.preventDefault();
        if(state.password !== state.password2){
            alert("Passwords does not match!");
        }
        else{
            let options = {
                method: 'POST',
                url: ADD_PROFILE_URL,
                headers: {
                    'Content-Type':'application/json',
                    'Authorization':'Bearer ' + String(authTokens?.access)
                },
                data: {
                    username: state.username,
                    password: state.password,
                    first_name: state.first_name,
                    last_name: state.last_name,
                    date_of_birth: state.date_of_birth,
                    type: state.type
                }
            };
            //trying to execute POST request
            axios(options).then((response) => {
                if(response.data !== "User is not registered."){
                    window.location.reload(false);
                }
                else{
                    alert("User was not added!");
                }
            }); 
        }       
    };

    //after load
    useEffect(() => {
        state.type = 'Registered'
    }, []);

  return (
    <>
    <Modal show={data[0]} onHide={data[1]} centered="true">
    <Modal.Header closeButton>
    </Modal.Header>
    <Modal.Body>
        <Container className="" style={{padding:"30px"}}>
           <Form onSubmit={addUser}>
                  <Form.Group className="">
                  <Form.Label className="login-text mb-0">*Username:</Form.Label>
                  <Form.Control name="username" onChange={updateInput} type="text" placeholder="Enter username" required/>
                  </Form.Group>

                  <Form.Group className="">
                  <Form.Label className="login-text mb-0">*Password:</Form.Label>
                  <Form.Control name="password" onChange={updateInput} type="password" placeholder="Enter password" required/>
                  </Form.Group>

                  <Form.Group className="">
                  <Form.Label className="login-text mb-0">*Password:</Form.Label>
                  <Form.Control name="password2" onChange={updateInput} type="password" placeholder="Re-enter password" required />
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">First name:</Form.Label>
                  <Form.Control name="first_name" onChange={updateInput} type="text" placeholder="Enter first name"/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">Last name:</Form.Label>
                  <Form.Control name="last_name" onChange={updateInput} type="text" placeholder="Enter last name"/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">Date of birth:</Form.Label>
                  <Form.Control name="date_of_birth" onChange={updateInput} type="date" 
                  max={new Date().toISOString().split("T")[0]} required/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">Type:</Form.Label>
                  <Form.Select onChange={updateInput} name="type" value={state.type}>
                    <option value="Administrator">Administrator</option>
                    <option value="Caretaker">Caretaker</option>
                    <option value="Veterinarian">Veterinarian</option>
                    <option value="Registered">Registered</option>
                  </Form.Select>
                  </Form.Group>
                  <div className="mt-4 d-flex justify-content-center" style={{width:"100%"}}>
                    <button type="submit" className="submit-button" >ADD USER</button>
                  </div>
                </Form>
        </Container>
    </Modal.Body>
    </Modal>
    </>
  )
}
