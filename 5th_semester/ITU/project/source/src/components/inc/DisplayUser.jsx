/**
 * @brief DisplayUser component file
 * 
 * @file DisplayUser.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from 'react'
import { Card } from 'react-bootstrap'
import { EditUser } from './EditUser';
import axios from 'axios';
import { AuthContext } from '../../context/AuthContext';

export const DisplayUser = ({data}) => {

    //modal window EditUser
    const [show, setShow] = useState(false);
  
    //click => hide, click => show
    const handleClick = () => {
        setShow(current => !current)
    };

    //getting user and JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);
    let {user} = useContext(AuthContext);

    //url for axios request
    const DELETE_PROFILE_URL = 'https://sigma-animal-shelter.herokuapp.com/api/user-delete/';


    //delete user axios request
    let deleteUser = (e) => {
        if(user.id !== data[0].id){
            e.preventDefault();
            let options = {
                method: 'DELETE',
                url: DELETE_PROFILE_URL + data[0].id + '/',
                withCredentials: true,
                headers: {
                    'Content-Type':'application/json',
                    'Authorization':'Bearer ' + String(authTokens?.access)
                }
            };
            axios(options).then((response) => {
                if(response.data !== "User is not deleted."){
                    //getting refreshed users
                    data[1]();
                  }
                  else{
                    alert("User was not deleted!");
                  }
            })
        }
        else{
            alert("You can not delete yourself here do it in MY ACCOUNT instead!");
        }
        };

  return (
    <Card key={data[0].id} style={{ width: '400px', minWidth: "400px" }} className="m-3 shadow-lg">
        <Card.Img src={data[0].photo} style={{height:"400px", objectFit: 'contain'}} variant="top"></Card.Img>
        <Card.Body className="d-flex justify-conent-center flex-column">
            <Card.Title className='text-center'>Type: {data[0].type}</Card.Title>
            <Card.Title className='text-center'>Firstname: {data[0].first_name}</Card.Title>
            <Card.Title className='text-center'>LastName: {data[0].last_name}</Card.Title>
            <div className='d-flex justify-content-evenly mt-2'>
                <button className='hyper-text' onClick={handleClick}>EDIT</button>
                <EditUser data={[show, handleClick, data[0], data[1]]} />
                <button className='hyper-text' onClick={deleteUser}>DELETE</button>
            </div>
        </Card.Body>
    </Card>
  )
};