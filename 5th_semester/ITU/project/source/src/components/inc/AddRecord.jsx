/**
 * @brief AddRecord component file
 * 
 * @file AddRecord.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from 'react'
import { Form, Modal, Container} from "react-bootstrap";
import axios from 'axios';
import { AuthContext } from '../../context/AuthContext';


export const AddRecord = ({data}) => {

    //url for axios request
    const ADD_RECORD_URL = 'https://sigma-animal-shelter.herokuapp.com/api/record-create/';

    //getting JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);

    //creating record
    let [state, setState] = useState({
        title : '',
        description : '',
    });

    //binding input values to type
    let updateInput = (e) => {
        setState({
            ...state,
                [e.target.name] : e.target.value
        })
    };

    //add record axios request
    let addRecord = (e) => {
        let today = new Date();
        const dateToday = today.getFullYear() + '-' + (today.getMonth() + 1) + '-' + today.getDate();
        e.preventDefault();
        let options = {
            method: 'POST',
            url: ADD_RECORD_URL,
            headers: {
                'Content-Type':'application/json',
                'Authorization':'Bearer ' + String(authTokens?.access)
            },
            data: {
                date : dateToday,  //TODAY
                title : state.title,
                description : state.description,
                animal: data[2].id,
            }
        };
        //trying to execute POST request
        axios(options).then((response) => {
            if(response.data !== "Record is not created."){
                alert("Record Created!");
                //getting updated record list from db
                data[3]();
            }
            else{
                alert("Record was not created!");
            }
        });        
        };

  return (
    <Modal show={data[0]} onHide={data[1]} centered="true">
    <Modal.Header closeButton>
    </Modal.Header>
    <Modal.Body>
        <Container className="" style={{padding:"30px"}}>
           <Form onSubmit={addRecord}>
                  <Form.Group className="">
                  <Form.Label className="login-text mb-0">Title:</Form.Label>
                  <Form.Control name="title" onChange={updateInput} type="text" />
                  </Form.Group>

                  <Form.Group className="mt-2">
                  <Form.Label className="login-text mb-0">Description:</Form.Label>
                  <Form.Control name="description" as="textarea" onChange={updateInput} rows={5} style={{resize: "none"}} />
                  </Form.Group>

                  <div className="mt-4 d-flex justify-content-center" style={{width:"100%"}}>
                    <button type="submit" className="submit-button">ADD RECORD</button>
                  </div>
                </Form>
        </Container>
    </Modal.Body>
    </Modal>
  )
}
