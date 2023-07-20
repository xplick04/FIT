/**
 * @brief DisplayRecord component file
 * 
 * @file DisplayRecord.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useContext } from 'react'
import { Col } from 'react-bootstrap'
import axios from 'axios';
import { AuthContext } from '../../context/AuthContext';

export const DisplayRecord = ({data}) => {

  //url for axios request
  const DELETE_RECORD_URL = "https://sigma-animal-shelter.herokuapp.com/api/record-delete/";

  //getting JWT token from AuthContext
  let {authTokens} = useContext(AuthContext);

  //delete record axios request
  let deleteRecord = (e) => {
    e.preventDefault();
    let options = {
        method: 'DELETE',
        url: DELETE_RECORD_URL + data[0].id + '/',
        withCredentials: true,
        headers: {
            'Content-Type':'application/json',
            'Authorization':'Bearer ' + String(authTokens?.access)
        },
    };
    axios(options).then((response) => {
      if(response.data !== "Record is not deleted."){
        //get current records from db
        data[1]();
      }
      else{
        alert("Record was not deleted!");
      }
    });        
    };


  return (
    <div className='table-row d-flex justify-content-center' style={{height:"3em"}}>
      <Col className='d-flex justify-content-center align-items-center' sm={3}>
        {data[0].date}
      </Col>
      <Col className='d-flex justify-content-center align-items-center' sm={3}>
        {data[0].title}
      </Col>
      <Col className='d-flex justify-content-center align-items-center' sm={4}>
        {data[0].description}
      </Col>
      <Col className='d-flex justify-content-center align-items-center' sm={2}>
        <button className='hyper-text' onClick={deleteRecord}>DELETE</button>
      </Col>
    </div>
  )
}
