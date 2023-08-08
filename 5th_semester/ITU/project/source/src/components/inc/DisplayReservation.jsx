/**
 * @brief DisplayReservation component file
 * 
 * @file DisplayReservation.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from 'react'
import { Form, Card } from 'react-bootstrap'
import { AuthContext } from "../../context/AuthContext";
import axios from 'axios';

export const DisplayReservation = ({data}) => {

    //creating reservation
    let [state, setState] = useState(data[0]);

    //binding input values to type
    let updateInput = (e) => {
        setState({
            ...state,
                [e.target.name] : e.target.checked
        })
    };

    //urls for axios requests
    const UPDATE_RESERVATION_URL = "https://sigma-animal-shelter.herokuapp.com/api/reservation-update/";
    const DELETE_RESERVATION_URL = "https://sigma-animal-shelter.herokuapp.com/api/reservation-delete/";

    //getting user and JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);
    let {user} = useContext(AuthContext);
    
    //update reservation status axios request
    let updateReservation = (e) => {
      e.preventDefault();
      let options = {
          method: 'POST',
          url: UPDATE_RESERVATION_URL + data[0].id + "/",
          withCredentials: true,
          headers: {
              'Content-Type':'application/json',
              'Authorization':'Bearer ' + String(authTokens?.access)
          },
          data: {
            returned : state.returned,
            borrowed : state.borrowed,
            confirmed : state.confirmed,
        }
      };
      axios(options).then((response) => {
        if(response.data !== "Reservation is not updated."){
            //getting updated reservations from db
            data[1]();
          }
          else{
            alert("Reservation was not updated!");
          }
      });        
    };

    //delete reservation axios request
    let deleteReservation = (e) => {
        e.preventDefault();
        let options = {
            method: 'DELETE',
            url: DELETE_RESERVATION_URL + data[0].id + "/",
            withCredentials: true,
            headers: {
                'Content-Type':'application/json',
                'Authorization':'Bearer ' + String(authTokens?.access)
            },
        };
        axios(options).then(() => {
            //getting updated reservations from db
            data[1]();
        });        
      };
    
  return (
    <>
    {user.type === "Caretaker" && 
    <Card style={{width: "12em", height: "12em"}} className="m-3 reservation">
        <Card.Title className='text-center'>{data[0].r_from.slice(0,5)}-{data[0].r_to.slice(0,5)}</Card.Title>
        <Card.Body className='d-flex align-items-center flex-column'>
            <Form>
                <Form.Check name="confirmed" onChange={updateInput} checked={state.confirmed} label="Confirmed"/>
                {data[0].confirmed ?
                <Form.Check name="borrowed" onChange={updateInput} checked={state.borrowed} label="Borrowed"/> :
                <Form.Check name="borrowed" onChange={updateInput} checked={state.borrowed} label="Borrowed" disabled/>
                }
                {(data[0].confirmed && data[0].borrowed) ?
                <Form.Check name="returned" onChange={updateInput} checked={state.returned} label="Returned"/> :
                <Form.Check name="returned" onChange={updateInput} checked={state.returned} label="Returned" disabled/>
                }
                <div className='d-flex justify-content-between align-items-center mt-2'>
                    <button className='hyper-text' onClick={updateReservation}>UPDATE</button>
                    <button className='hyper-text' onClick={deleteReservation}>DELETE</button>
                </div>
            </Form>
        </Card.Body>
    </Card>
    }
    {(user.type === "Registered" && data[0].confirmed) &&
    <Card style={{width: "8em", height: "2em"}} className="m-3 reservation d-flex justify-content-center align-items-center">
        <div className='text-center empty-request-font'>{data[0].r_from.slice(0,5)}-{data[0].r_to.slice(0,5)}</div>
    </Card>
    }
    {(user.type === "Registered" && !data[0].confirmed) &&
    <Card style={{width: "8em", height: "2em"}} className="m-3 reservation-not-confirmed d-flex justify-content-center align-items-center">
        <div className='text-center empty-request-font'>{data[0].r_from.slice(0,5)}-{data[0].r_to.slice(0,5)}</div>
    </Card>
    }
    </>
  )
}
