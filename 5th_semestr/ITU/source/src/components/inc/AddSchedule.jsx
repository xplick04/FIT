/**
 * @brief AddSchedule component file
 * 
 * @file AddSchedule.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from 'react'
import { Form, Modal, Container} from "react-bootstrap";
import axios from 'axios';
import { AuthContext } from '../../context/AuthContext';

export const AddSchedule = ({data}) => {

    //url for axios request
    const ADD_SCHEDULE_URL = 'https://sigma-animal-shelter.herokuapp.com/api/schedule-create/';

    //getting JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);

    //creating schedule
    let [state, setState] = useState({
        date : '',
        s_from : '',
        s_to : '',
    });

    //binding input values to schedule
    let updateInput = (e) => {
        setState({
            ...state,
                [e.target.name] : e.target.value
        })
    };

    //add schedule axios request
    let addSchedule = (e) => {
        e.preventDefault();
        let options = {
            method: 'POST',
            url: ADD_SCHEDULE_URL,
            headers: {
                'Content-Type':'application/json',
                'Authorization':'Bearer ' + String(authTokens?.access)
            },
            data: {
                date : state.date,
                s_from : state.s_from,
                s_to : state.s_to,
                animal: data[2].id,
            }
        };
        axios(options).then((response) => {
            if(response.data !== "Collision."){
                //getting updated schedule list from db
                data[3]();
                alert("Schedule created!");
            }
            else{
                alert("Schedule was not created!");
            }
        });        
        };

    return (
        <Modal show={data[0]} onHide={data[1]} centered="true">
            <Modal.Header closeButton>
        </Modal.Header>
        <Modal.Body>
            <Container className="" style={{padding:"30px"}}>
                <Form onSubmit={addSchedule}>
                    <Form.Group className="">
                    <Form.Label className="login-text mb-0">*Date:</Form.Label>
                    <Form.Control name="date" onChange={updateInput} type="date" 
                    min={new Date().toISOString().split("T")[0]} required/>
                    </Form.Group>

                    <Form.Group className="">
                    <Form.Label className="login-text mb-0">*Time from:</Form.Label>
                    <Form.Control name="s_from" onChange={updateInput} type="time" required/>
                    </Form.Group>

                    <Form.Group className="">
                    <Form.Label className="login-text mb-0">*Time to:</Form.Label>
                    <Form.Control name="s_to" onChange={updateInput} type="time" required/>
                    </Form.Group>

                    <div className="mt-4 d-flex justify-content-center" style={{width:"100%"}}>
                        <button type="submit" className="submit-button">ADD SCHEDULE</button>
                    </div>
                </Form>
            </Container>
        </Modal.Body>
        </Modal>
    )
}
