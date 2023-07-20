/**
 * @brief DisplayRecords component file
 * 
 * @file DisplayRecords.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useEffect, useContext } from 'react'
import axios from 'axios';
import { AuthContext } from "../../context/AuthContext"
import { Container, Col, Row } from 'react-bootstrap';
import { DisplayRecord } from './DisplayRecord';
import { AddRecord } from './AddRecord';

export const DisplayRecords = ({data}) => {

    //url for axios request
    const RECORDS_URL = "https://sigma-animal-shelter.herokuapp.com/api/record-list/";

    //creating records list
    let [recordState, setRecordState] = useState({
        records: []
        });

    //getting JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);

    //get all records from db
    let getRecords = () => {
        let options = {
            method: 'GET',
            url: RECORDS_URL + data.id + "/",
            withCredentials: true,
            headers: {
                'Content-Type':'application/json',
                'Authorization':'Bearer ' + String(authTokens?.access)
            }
        };
        axios(options).then((response) => {
            setRecordState({
                ...recordState,
                records: response.data
                });
        });
    };

    //modal window for record
    const [show, setShow] = useState(false);

    //click => hide, click => show
    const handleClick = () => {
        setShow(current => !current)
    };

    //after load get all records
    useEffect(() => {
        getRecords()
      }, []);

  return (
    <>
        <Container className=''>
            <Row>
            <Col className='text-center bold-font' sm={3}>DATE:</Col> 
            <Col className='text-center bold-font' sm={3}>TITLE:</Col> 
            <Col className='text-center bold-font' sm={4}>DESCRIPTION:</Col>
            <Col className='text-center bold-font' sm={2}>ACTION:</Col>
            </Row>
            <Row>
                <div style={{border: "2px solid black", padding: "0"}}>
                    {
                    recordState.records.map(record => {
                            return (
                                <>
                                    <DisplayRecord data={[record, getRecords]}/>
                                </>
                            )
                        })
                    }
                    <div className='d-flex justify-content-center'>
                        <button className='hyper-text' style={{height:"2.5em"}} onClick={handleClick}>ADD RECORD</button>
                        <AddRecord data={[show, handleClick, data, getRecords]}/>
                    </div> 
                </div>
            </Row>      
        </Container>
    </>
  )
}
