/**
 * @brief SignUp page component file
 * 
 * @file SignUp.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useEffect, useContext } from 'react'
import { Form, Card, Container, Row } from 'react-bootstrap'
import axios from "axios"
import { AuthContext } from '../context/AuthContext';
import { useNavigate } from "react-router-dom";



export const SignUp = () => {

  //url for axios request
  const REGISTER_URL = 'https://sigma-animal-shelter.herokuapp.com/api/register/';

  //getting user informations from Authcontext
  let {user} = useContext(AuthContext);

  //creating user
  let [state, setState] = useState({
    User: {
      username : '',
      password : '',
      password2: '',
      first_name : '',
      last_name : '',
      date_of_birth : ''
    }
  });

  //binding input values to user from above
  let updateInput = (e) => {
      setState({
          ...state,
          User: {
              ...state.User,
              [e.target.name] : e.target.value
          }
      })
  };

  //register axios request
  let RegisterUser = (e) => {
    e.preventDefault();
    if(state.User.password !== state.User.password2){
      alert("Passwords does not match!");
    }
    else{
      let options = {
        method: 'POST',
        url: REGISTER_URL,
        headers: {
          'Content-Type':'application/json',
        },
        data: {
            username: state.User.username,
            password: state.User.password,
            first_name: state.User.first_name,
            last_name: state.User.last_name,
            date_of_birth: state.User.date_of_birth
        }
      };
      //trying to execute POST request
      axios(options).then((response) => {
        if(response.data !== "User is not registered."){
          window.location.reload(false);
          alert("User register successfully!");
        }
        else{
          alert("User was not registered!");
        }
      });   
    }     
  };

  //for redirecting user
  const navigate = useNavigate();

  //redirect user if already logged in
  useEffect(() => {
      if(user){
        navigate("/");
      }
  }, []);


  return (
    <Container>
      <Card className='mt-4'>
        <Row className="mt-2 d-flex justify-content-center">
              <Form style={{width:"400px"}} onSubmit={RegisterUser}>
                  <Form.Group >
                  <Form.Label className="login-text mb-0">*Username:</Form.Label>
                  <Form.Control name="username" type="text" onChange={updateInput} placeholder="Enter username" required/>
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">*Password:</Form.Label>
                  <Form.Control name="password" type="password" onChange={updateInput} placeholder="Enter password" required />
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">*Password:</Form.Label>
                  <Form.Control name="password2" type="password" onChange={updateInput} placeholder="Re-enter password" required />
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">First name:</Form.Label>
                  <Form.Control name="first_name" type="text" onChange={updateInput} placeholder="Enter first name" />
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">Last name:</Form.Label>
                  <Form.Control name="last_name" type="text" onChange={updateInput} placeholder="Enter last name" />
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">*Date of birth:</Form.Label>
                  <Form.Control name="date_of_birth" type="date" onChange={updateInput} 
                  max={new Date().toISOString().split("T")[0]} required/>
                  </Form.Group>
                  <div className="mt-3 mb-3 d-flex justify-content-center" style={{width:"100%"}}>
                  <button type="submit" className="submit-button">REGISTER NOW</button>
                  </div>
                </Form>
        </Row>
      </Card>
    </Container>
  )
};
