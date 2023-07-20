/**
 * @brief DisplayWalk component file
 * 
 * @file DisplayWalk.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useContext } from 'react'
import { Col } from 'react-bootstrap'
import axios from 'axios';
import { AuthContext } from '../../context/AuthContext';

export const DisplayWalk = ({data}) => {

  //url for axios request
  const DELETE_WALK_URL = 'https://sigma-animal-shelter.herokuapp.com/api/reservation-delete/';

  //getting JWT token from AuthContext
  let {authTokens} = useContext(AuthContext);

  //delete walk axios request
  let deleteWalk = (e) => {
    e.preventDefault();
    let options = {
      method: 'DELETE',
      url: DELETE_WALK_URL + data[0].id + "/",
      withCredentials: true,
      headers: {
          'Content-Type':'application/json',
          'Authorization':'Bearer ' + String(authTokens?.access)
      }
    };
    axios(options).then((response) => {
      if(response.data !== "Reservation is not deleted."){
        //getting updated walks from db
        data[1]();
      }
      else{
        alert("Reservation was not deleted!");
      }
    });
  };

  return (
    <div className='d-flex mywalks-walk' style={{height: "2em"}}>
        <Col sm={3} className="d-flex justify-content-center align-items-center">{data[0].schedule}</Col>
        <Col sm={3} className="d-flex justify-content-center align-items-center">{data[0].r_from.slice(0,5)}-{data[0].r_to.slice(0,5)}</Col>
        <Col sm={3} className="d-flex justify-content-center align-items-center">{data[0].animal}</Col>
        <Col sm={3} className="d-flex justify-content-center align-items-center">
            <button className='hyper-text' onClick={deleteWalk}>CANCEL</button>
        </Col>
    </div>
  )
}
