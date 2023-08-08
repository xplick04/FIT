/**
 * @brief ManageUsers page component file
 * 
 * @file ManageUsers.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useEffect, useContext } from 'react'
import axios from 'axios';
import { Container } from 'react-bootstrap'
import { useNavigate } from "react-router-dom";
import { AuthContext } from '../context/AuthContext';
import { DisplayUser } from '../components/inc/DisplayUser';
import { AddUser } from '../components/inc/AddUser';


export const ManageUsers = () => {

  //url for axios request
  const USER_LIST_URL = 'https://sigma-animal-shelter.herokuapp.com/api/user-list/';
  
  //creating user list
  let [userState, setUserState] = useState({
      users: []
    });
  
  //getting user informations and JWT token from AuthContext
  let {authTokens} = useContext(AuthContext);
  let {user} = useContext(AuthContext);
  
  //modal window for adding new user
  const [show, setShow] = useState(false);

  //modal window, click => hide, click => show
  const handleClick = () => {
      setShow(current => !current)
  };

  //getting all users from db
  let getUsersList = () => {
      let options = {
          method: 'GET',
          url: USER_LIST_URL,
          withCredentials: true,
          headers: {
              'Content-Type':'application/json',
              'Authorization':'Bearer ' + String(authTokens?.access)
          }
      };
      axios(options).then((response) => {
          setUserState({
              ...userState,
              users: response.data
            });
      });
  };
  
  //for redirecting user
  const navigate = useNavigate();

  //get user list after load and redirect user if he does not have role "Administrator"
  useEffect(() => {
    if(user.type !== "Administrator"){
      navigate("/");
    }
    getUsersList()
  }, []);

  return (
    <>
      <div className='d-flex justify-content-center mt-3'>
        <button className="text-center hyper-text" onClick={handleClick}>ADD USER</button>
        <AddUser data={[show, handleClick]}/>
      </div>
      <Container className='d-flex flex-wrap justify-content-center mt-2 mb-4'>{
            //sending individual user data to DisplayUser component
            userState.users.map(user => {
              return (
                <>
                  <DisplayUser data={[user, getUsersList]} />
                </>
              );
            })
          }
      </Container>
    </>
  )
};
