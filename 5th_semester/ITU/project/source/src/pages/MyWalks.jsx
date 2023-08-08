/**
 * @brief MyWalks page component file
 * 
 * @file MyWalks.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useEffect, useContext } from 'react'
import axios from 'axios';
import { AuthContext } from '../context/AuthContext';
import { useNavigate } from "react-router-dom";
import { DisplayWalk } from '../components/inc/DisplayWalk';
import { Container, Card, Col } from 'react-bootstrap';

export const MyWalks = () => {

  //url for axios request
  const GET_MY_WALKS_URL = "https://sigma-animal-shelter.herokuapp.com/api/mywalks/";

  //getting user informations and JWT token from Authcontext
  let {authTokens} = useContext(AuthContext);
  let {user} = useContext(AuthContext);

  //creating walks list
  let [walkState, setWalkState] = useState({
    walks: [],
    });

  //getting user walks from db
  let getWalks = () => {
    let options = {
        method: 'GET',
        url: GET_MY_WALKS_URL,
        withCredentials: true,
        headers: {
            'Content-Type':'application/json',
            'Authorization':'Bearer ' + String(authTokens?.access)
        },
    }
    axios(options).then((response) => {
      setWalkState({
        ...walkState,
        walks: response.data
      });
    });  
  };

  //for redirecting user
  const navigate = useNavigate();

  //get user walks list after load and redirect user if he does not have role "Registered"
  useEffect(() => {
    if(user.type !== "Registered"){
      navigate("/");
    }
    getWalks()
  }, []);

  return (
    <div className='mt-4 mb-4'>
      <Container className='mt-4 mb-4'>
        <Card className='mt-4 shadow-lg'>
        <div className=''>
          <Container className='p-0 mt-5 mb-5' style={{width:"80%"}}>
            <div className='d-flex'>
              <Col sm={3} className="text-center">Date:</Col>
              <Col sm={3} className="text-center">Time:</Col>
              <Col sm={3} className="text-center">Animal name:</Col>
              <Col sm={3} className="text-center">Action:</Col>
            </div>

            <div style={{ border:"2px solid black"}}>
            {walkState.walks.length > 0 ?
              walkState.walks.map(walk => {
                return (
                  <>
                    <DisplayWalk data={[walk, getWalks]} />
                  </>
                );
              }) : <div className='d-flex justify-content-center empty-request-font'>NO WALKS RESERVED</div>
            }
            </div>
          </Container>
        </div>
        <Card.Footer className='text-center mywalks-footer'>NOTE: You can add walks in OUR ANIMALS</Card.Footer>
        </Card>
      </Container>
    </div>
  )
}
