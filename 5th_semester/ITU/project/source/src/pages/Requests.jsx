/**
 * @brief Requests page component file
 * 
 * @file Requests.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useEffect, useContext } from 'react'
import { Card, Col, Container } from 'react-bootstrap'
import axios from 'axios';
import { DisplayRequest } from '../components/inc/DisplayRequest';
import { AuthContext } from "../context/AuthContext";
import { useNavigate } from "react-router-dom";

export const Requests = () => {

    //url for axios request
    const REQUEST_LIST_URL = 'https://sigma-animal-shelter.herokuapp.com/api/user-unverified/';

    //creating requsts list
    let [requestState, setRequestState] = useState({
        requests: []
        });

    //getting user informations and JWT token from Authcontext
    let {authTokens} = useContext(AuthContext);
    let {user} = useContext(AuthContext);
    
    //getting all requests from db
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

    //get requests list after load and redirect user if he does not have role "Caretaker"
    useEffect(() => {
        if(user.type !== "Caretaker"){
            navigate("/");
            }
        getRequestList()
    }, []);
    
    return (
    <Container>
    <Card className='mt-4 mb-4 shadow-lg'>
        <Card.Body className='mt-4 mb-4' style={{padding: "1rem"}}>
            <div className='d-flex justify-content-center mb-1'>
                <Col className='text-center bold-font' sm={3}>DATE:</Col> 
                <Col className='text-center bold-font' sm={2}>TYPE:</Col> 
                <Col className='text-center bold-font' sm={3}>DESCRIPTION:</Col>
                <Col className='text-center bold-font' sm={3}>ACTION:</Col>
            </div>
            <div style={{border: "2px solid black"}}>
            { requestState.requests.length > 0 ? 
            requestState.requests.map(request => {
                return (
                    <>
                        <DisplayRequest data={[request, getRequestList]} />
                    </>
                )
            }) : <div className='d-flex justify-content-center empty-request-font'>NO PROFILE TO VERIFY</div>
            }
            </div>
        </Card.Body>
        <Card.Footer className='text-center mywalks-footer'>NOTE: You can edit reservations in OUR ANIMALS</Card.Footer>
    </Card>
    </Container>
    )
}
