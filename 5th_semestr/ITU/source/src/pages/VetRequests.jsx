/**
 * @brief VetRequests page component file
 * 
 * @file VetRequests.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useEffect, useContext } from 'react'
import { Card, Col, Container} from 'react-bootstrap'
import axios from 'axios';
import { DisplayVetRequest } from '../components/inc/DisplayVetRequest';
import { AuthContext } from "../context/AuthContext";
import { useNavigate } from "react-router-dom";

export const VetRequests = () => {

    //url for axios request
    const REQUEST_LIST_URL = 'https://sigma-animal-shelter.herokuapp.com/api/request-list/';

    //creating requests list
    let [requestState, setRequestState] = useState({
        requests: []
        });

    //getting user informations and JWT token from Authcontext
    let {authTokens} = useContext(AuthContext);
    let {user} = useContext(AuthContext);
    
    //getting requests from db
    let getRequestList = () => {
        let options = {
            method: 'GET',
            url: REQUEST_LIST_URL,
            withCredentials: true,
            headers: {
            'Content-Type':'application/json',
            'Authorization':'Bearer ' + String(authTokens?.access)
            },
        };
        axios(options).then((response) => {
            setRequestState({
                ...requestState,
                requests: response.data
                });
        });
    };

    //for redirecting user
    const navigate = useNavigate();

    //get requests list after load and redirect user if he does not have role "Veterinarian"
    useEffect(() => {
        if(user.type !== "Veterinarian"){
            navigate("/");
          }
        getRequestList()
      }, []);
      
  return (
    <Container>
    <Card className='mt-4 mb-4 shadow-lg'>
        <Card.Body className='mt-4 mb-4' style={{padding: "1rem"}}>
                <div className='d-flex justify-content-center mb-1'>
                    <Col className='text-center bold-font' sm={2}>DATE:</Col> 
                    <Col className='text-center bold-font' sm={2}>TYPE:</Col> 
                    <Col className='text-center bold-font' sm={5}>DESCRIPTION:</Col>
                    <Col className='text-center bold-font' sm={3}>ACTION:</Col>
                </div>
                <div style={{border: "2px solid black"}}>
                { requestState.requests.length > 0 ?
                requestState.requests.map(request => {
                    return (
                        <>
                            <DisplayVetRequest data={[request, getRequestList]} />
                        </>
                    )
                }) : <div className='d-flex justify-content-center empty-request-font'>NO MEDICAL REQUESTS</div>
                }
                </div>
        </Card.Body>
    </Card>
   </Container>
  )
}
