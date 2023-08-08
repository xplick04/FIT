/**
 * @brief MyProfile page component file
 * 
 * @file MyProfile.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useEffect, useContext } from 'react'
import axios from 'axios';
import { AuthContext } from '../context/AuthContext';
import { Container, Card, Row, Form, Col } from 'react-bootstrap';
import { useNavigate } from "react-router-dom";

export const MyProfile = () => {

  //urls for axios requests
  const MY_PROFILE_URL = 'https://sigma-animal-shelter.herokuapp.com/api/user-detail/';
  const MY_PROFILE_PHOTO_URL = 'https://sigma-animal-shelter.herokuapp.com/api/upload-photo/';

  //getting user informations and JWT token from AuthContext
  let {user} = useContext(AuthContext);
  let {authTokens} = useContext(AuthContext);

  //logout from authContext
  let {logoutUser} = useContext(AuthContext);

  //creating user state
  let [userProfile, setUserProfile] = useState({
        username : '',
        first_name : '',
        last_name : '',
        date_of_birth : '',
        type: '',
        photo: '',
  });
  
  //binding input values to user state
  let UpdateInput = (e) => {
      setUserProfile({
          ...userProfile,
              [e.target.name] : e.target.value
      })
  };

  //edit user axios request
  let editUser = (e) => {
    e.preventDefault();
    let options = {
      method: 'POST',
      url: MY_PROFILE_URL + String(user.user_id)+ '/',
      withCredentials: true,
      headers: {
          'Content-Type':'application/json',
          'Authorization':'Bearer ' + String(authTokens?.access)
      },
      data: {
        first_name: userProfile.first_name,
        last_name: userProfile.last_name,
        date_of_birth: userProfile.date_of_birth,
        photo: userProfile.photo
      }
    };
    axios(options).then((response) => {
      if(response.data !== "User is not updated."){
        window.location.reload(false);
      }
      else{
        alert("User not updated!");
      }
    });
  };
  
  //creating userPhoto state
  let [userPhoto, setUserPhoto] = useState(null);

  //binding input file to userPhoto state
  let updatePhoto = (e) => {setUserPhoto(e.target.files[0])};

  //edit user photo axios request
  let editPhoto = (e) => {
    e.preventDefault();
    let options = {
      method: 'POST',
      url: MY_PROFILE_PHOTO_URL + String(user.user_id)+ '/',
      withCredentials: true,
      headers: {
          'Content-Type':'multipart/form-data',
          'Authorization':'Bearer ' + String(authTokens?.access)
      },
      data: {
        photo: userPhoto,
      }
    };
    axios(options).then((response) => {
      if(response.data !== "User is not updated."){
        window.location.reload(false);
      }
      else{
        alert("User not updated!");
      }
    })
  };

  //getting user from db
  let getUserData = () => {
      let options = {
          method: 'GET',
          url: MY_PROFILE_URL + String(user.user_id)+ '/',
          withCredentials: true,
          headers: {
              'Content-Type':'application/json',
              'Authorization':'Bearer ' + String(authTokens?.access)
          }
      };
      //trying to execute POST request
      axios(options).then((response) => {
          setUserProfile(response.data);
      })
    };

  //for redirecting user
  const navigate = useNavigate();

  //get user data after load and redirect user if he does not logged in
  useEffect(() => {
      if(!user){
        navigate("/");
      }
      getUserData();
  }, []);

  return (
    <Container className="d-flex justify-content-center">
        <Card className=' mt-4 pt-4 pb-4 shadow-lg' style={{ width: '50rem'}}>
            <Row className="mt-2 d-flex justify-content-center">
              <Col className='d-flex justify-content-center flex-column flex-self-end'>
              <Container className='d-flex justify-content-center'>
                <Form style={{width:"300px"}}>
                    <Form.Group className="">
                    <Form.Label className="login-text mb-0">Username:</Form.Label>
                    <Form.Control name="username" type="text" placeholder={userProfile.username} disabled/>
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">First name:</Form.Label>
                    <Form.Control name="first_name" type="text" onChange={UpdateInput} placeholder={userProfile.first_name} />
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">Last name:</Form.Label>
                    <Form.Control name="last_name" type="text" onChange={UpdateInput} placeholder={userProfile.last_name} />
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">Date of birth:</Form.Label>
                    <Form.Control name="date_of_birth" type="date" onChange={UpdateInput} defaultValue={userProfile.date_of_birth} />
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">Type:</Form.Label>
                    <Form.Control name="type" type="text" placeholder={userProfile.type} disabled/>
                    </Form.Group>
                    <div className="mt-4 d-flex justify-content-evenly" style={{width:"100%"}}>
                      <button type="submit" className="submit-button" onClick={editUser}>EDIT USER</button>
                    </div>
                  </Form>
                </Container>
                </Col>
                <Col className="d-flex flex-column align-items-center align-self-end">
                <Container className="d-flex flex-column align-items-center align-self-end">
                  {userProfile.photo ?
                  <img alt="profilePic" style={{maxWidth:"300px", maxHeight:"250px"}} src={userProfile.photo} />
                  : <div style={{maxWidth:"300px", maxHeight:"250px"}}></div>
                  }
                  <Form style={{maxWidth:"300px"}} className="mt-2">
                    <Form.Group>
                    <Form.Label className="login-text mb-0">Photo:</Form.Label>
                    <Form.Control name="userPhoto" onChange={updatePhoto} type="file"/>
                    </Form.Group>
                    <div className="mt-4 d-flex justify-content-evenly" style={{width:"100%"}}>
                      <button type="submit" className="submit-button" onClick={editPhoto}>EDIT PHOTO</button>
                    </div>
                  </Form>
                </Container>
                </Col>
                <div className="mt-4 d-flex justify-content-evenly" style={{width:"100%"}}>
                    <button type="submit" className="submit-button" onClick={logoutUser}>LOGOUT</button>
                </div>
            </Row>
        </Card>
    </Container>
  )
}
