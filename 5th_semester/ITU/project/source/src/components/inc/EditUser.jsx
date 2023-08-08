/**
 * @brief EditUser component file
 * 
 * @file EditUser.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from 'react'
import { Form, Modal, Container} from "react-bootstrap";
import axios from 'axios';
import { AuthContext } from '../../context/AuthContext';

export const EditUser = ({data}) => {

  //url for axios request
  const EDIT_PROFILE_URL = 'https://sigma-animal-shelter.herokuapp.com/api/user-type/';

  //getting JWT token from AuthContext
  let {authTokens} = useContext(AuthContext);

  //creating user
  let [state, setState] = useState(data[2].type);

  //binding input values to user
  let updateInput = (e) => {
      setState(e.target.value)
  };

  //edit changes axios request
  let editChanges = (e) => {
    e.preventDefault();
    let options = {
      method: 'POST',
      url: EDIT_PROFILE_URL + data[2].id + "/",
      withCredentials: true,
      headers: {
        'Content-Type':'application/json',
        'Authorization':'Bearer ' + String(authTokens?.access)
      },
      data: {
          type: state
      }
  };
  axios(options).then((response) => {
    if(response.data !== "User is not updated."){
      alert("User updated!");
      //getting updated user from db
      data[3]();
    }
    else{
      alert("User not updated!");
    }
  });
  }

  return (
    <>
    <Modal  show={data[0]} onHide={data[1]} centered="true">
    <Modal.Header closeButton>
    </Modal.Header>
    <Modal.Body>
        <Container className="" style={{padding:"30px"}}>
           <Form style={{width:"400px"}} onSubmit={editChanges}>
                  <Form.Group className="">
                  <Form.Label className="login-text mb-0">Username:</Form.Label>
                  <Form.Control name="username" type="text" placeholder={data[2].username} disabled/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">First name:</Form.Label>
                  <Form.Control name="first_name" type="text" placeholder={data[2].first_name} disabled/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">Last name:</Form.Label>
                  <Form.Control name="last_name" type="text" placeholder={data[2].last_name} disabled/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">Date of birth:</Form.Label>
                  <Form.Control name="date_of_birth" type="date" value={data[2].date_of_birth} 
                  max={new Date().toISOString().split("T")[0]} disabled/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">*Type:</Form.Label>
                  <Form.Select value={state} onChange={updateInput}>
                    <option value="Administrator">Administrator</option>
                    <option value="Caretaker">Caretaker</option>
                    <option value="Veterinarian">Veterinarian</option>
                    <option value="Registered">Registered</option>
                  </Form.Select>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">Verified:</Form.Label>
                  <Form.Control name="type" type="text" placeholder={data[2].verified ? "Yes" : "No"} disabled/>
                  </Form.Group>

                  <div className="mt-4 d-flex justify-content-center" style={{width:"100%"}}>
                    <button type="submit" className="submit-button">EDIT CHANGES</button>
                  </div>
                </Form>
        </Container>
    </Modal.Body>
    </Modal>
    </>
  )
}
