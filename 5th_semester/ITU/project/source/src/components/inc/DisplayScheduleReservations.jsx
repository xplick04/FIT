/**
 * @brief DisplayScheduleReservations component file
 * 
 * @file DisplayScheduleReservations.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useEffect, useContext } from 'react'
import axios from 'axios';
import { AuthContext } from "../../context/AuthContext"
import { Container, Modal, Col, Form } from 'react-bootstrap'
import { DisplayReservation } from './DisplayReservation';

export const DisplayScheduleReservations = ({dataReservations}) => {

    //urls for axios requests
    const GET_RESERVATIONS_URL = "https://sigma-animal-shelter.herokuapp.com/api/reservation-list/";
    const CREATE_RESERVATION_URL = "https://sigma-animal-shelter.herokuapp.com/api/reservation-create/";

    //getting user and JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);
    let {user} = useContext(AuthContext);

    //creating reservation list
    let [state, setState] = useState({
        reservations: []
      });

    //getting reservations from db
    let getReservations = () => {
      let options = {
          method: 'GET',
          url: GET_RESERVATIONS_URL + dataReservations[2].id + "/",
          withCredentials: true,
          headers: {
              'Content-Type':'application/json',
              'Authorization':'Bearer ' + String(authTokens?.access)
          },
      };
      axios(options).then((response) => {
        setState({
            ...state,
            reservations: response.data
          });
      });        
    };

    //after load get all reservations
    useEffect(() => {
        getReservations();
    }, []);

    //creating reservation
    let [reservation, setReservation] = useState({
        r_from: "",
        r_to: ""
    });

    //binding input values to reservation
    let updateInput = (e) => {
        setReservation({
            ...reservation,
                [e.target.name] : e.target.value
        })
    };

    //creating reservation axios request
    let createReservation = async (e) => {
        e.preventDefault();
        let options = {
            method: 'POST',
            url: CREATE_RESERVATION_URL,
            withCredentials: true,
            headers: {
                'Content-Type':'application/json',
                'Authorization':'Bearer ' + String(authTokens?.access)
            },
            data: {
                r_from: reservation.r_from,
                r_to: reservation.r_to,
                schedule: dataReservations[2].id,
                confirmed: false,
                returned: false,
                borrowed: false,
                user: user.user_id,
            },
        };
        //trying to execute POST request
        axios(options).then((response) => {
            if(response.data === "Collision."){
                alert("This time is unavailable!");
            }
            else{
                alert("Reservation created!");
                getReservations();
            }
        });        
      };

  return (
    <Modal show={dataReservations[0]} onHide={dataReservations[1]} centered="true">
        <Modal.Header closeButton>
        </Modal.Header>
        <Modal.Title className='text-center'>{dataReservations[2].date.slice(8,10) + "." + dataReservations[2].date.slice(5,7) + "."
         + dataReservations[2].date.slice(0,4) + ", " + dataReservations[2].s_from.slice(0,5) + "-" + dataReservations[2].s_to.slice(0,5) }</Modal.Title>
        <Modal.Body>
            {user.type === "Caretaker" &&
            <Container className="d-flex justify-content-center flex-wrap schedule-container">
                {(state.reservations.length > 0) ?
                    state.reservations.map(reservation => {
                        return (
                            <>
                                <DisplayReservation data={[reservation, getReservations]}/>
                            </>
                        )
                    }) : <div className='empty-request-font mt-2'>No reservations available</div>
                }
            </Container>
            }

            {user.type === "Registered" &&
            <>
            <Container className="d-flex justify-content-center flex-wrap schedule-container">
                <Col>
                <div className='empty-request-font mt-2'>TIMES UNAVAILABLE:</div>
                {(state.reservations.length > 0) ?
                    state.reservations.map(reservation => {
                        return (
                            <>
                                <DisplayReservation data={[reservation, getReservations]}/>
                            </>
                        )
                    }) : <div className='mt-2'>No reservations yet!</div>
                }
                </Col>
                <Col>
                <div className='empty-request-font mt-2'>CREATE RESERVATION:</div>
                <Form className="mt-2">
                <Form.Group>
                    <Form.Label className="login-text mb-0">*From:</Form.Label>
                    <Form.Control name="r_from" type="time" onChange={updateInput} value={reservation.r_from} required/>
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">*To:</Form.Label>
                    <Form.Control name="r_to" type="time" onChange={updateInput} value={reservation.r_to} required/>
                    </Form.Group>
                    <div className='mt-3 d-flex justify-content-center align-items-center'>
                        <button className="hyper-text" onClick={createReservation}>SUBMIT</button>
                    </div>
                </Form>
                </Col>
            </Container>
            </>
            }
            
        </Modal.Body>
        </Modal>
  )
}
