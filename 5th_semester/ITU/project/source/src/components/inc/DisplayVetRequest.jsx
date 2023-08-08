/**
 * @brief DisplayVetRequest component file
 * 
 * @file DisplayVetRequest.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from 'react'
import { AuthContext } from "../../context/AuthContext";
import { Col } from 'react-bootstrap'
import axios from 'axios';
import { AnimalRequest } from './AnimalRequest';


export const DisplayVetRequest = ({data}) => {

    //url for axios request
    const REQUEST_DELETE_URL = "https://sigma-animal-shelter.herokuapp.com/api/request-delete/";

    //modal window vetRequest
    const [show, setShow] = useState(false);

    //modal window, click => hide, click => show
    const handleClick = () => {
        setShow(current => !current)
    };

    //getting JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);

    //delete vetReqeust axios request
    let deleteRequest = (e) => {
        e.preventDefault();
        let options = {
            method: 'DELETE',
            url: REQUEST_DELETE_URL + data[0].id + "/",
            withCredentials: true,
            headers: {
                'Content-Type':'application/json',
                'Authorization':'Bearer ' + String(authTokens?.access)
            }
        };
        axios(options).then((response) => {
            if(response.data !== "Request is not deleted."){
                //getting refreshed vet requests
                data[1]();
            }
            else{
                alert("Request was not deleted!");
            }
        })
    };

  return (
    <div className='table-row d-flex justify-content-center' style={{height: "4rem"}}>
        <Col className='table-col justify-content-center d-flex align-items-center' sm={2}>
            {data[0].date}
        </Col>
        <Col className='table-col justify-content-center d-flex align-items-center' sm={2}>
            {data[0].title}
        </Col>
        <Col className='table-col justify-content-center d-flex align-items-center' sm={5}>
            {data[0].description}
        </Col>
        <Col className='d-flex justify-content-center align-items-center' sm={3}>
            <button className='hyper-text' onClick={handleClick}>SHOW ANIMAL</button>
            <AnimalRequest data={[show, handleClick, data[0]]}/>
            <button className='hyper-text' onClick={deleteRequest}>CLOSE</button>
        </Col>
    </div>
  )
}
