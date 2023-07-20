/**
 * @brief DisplayRequests component file
 * 
 * @file DisplayRequests.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from 'react'
import { AuthContext } from "../../context/AuthContext";
import { Col } from 'react-bootstrap'
import { ShowProfile } from './ShowProfile';
import axios from 'axios';


export const DisplayRequest = ({data}) => {

    //url for axios request
    const PROFILE_VERIFY_URL = "https://sigma-animal-shelter.herokuapp.com/api/user-verify/";

    //modal window for request
    const [show, setShow] = useState(false);

    //modal window, click => hide, click => show
    const handleClick = () => {
        setShow(current => !current)
    };

    //getting JWT token from AuthContext
    let {authTokens} = useContext(AuthContext);

    //axios verify profile request
    let verifyProfile = (e) => {
        e.preventDefault();
        let options = {
            method: 'POST',
            url: PROFILE_VERIFY_URL + data[0].id + "/",
            withCredentials: true,
            headers: {
                'Content-Type':'application/json',
                'Authorization':'Bearer ' + String(authTokens?.access)
            }
        };
        axios(options).then((response) => {
            if(response.data !== "User is not updated."){
                //getting updated requests
                data[1]();
            }
            else{
                alert("User was not verified!");
            }
        });
    };

  return (
    <div className='table-row d-flex justify-content-center' style={{height: "4rem"}}>
        <Col className='table-col justify-content-center d-flex align-items-center' sm={3}>
            {data[0].date_joined.split('T')[0]}
        </Col>
        <Col className='table-col justify-content-center d-flex align-items-center' sm={2}>
            ACOUNT VERIFICATION
        </Col>
        <Col className='table-col justify-content-center d-flex align-items-center' sm={3}>
            <button className='hyper-text' onClick={handleClick}>SHOW PROFILE</button>
        </Col>
        <Col className='d-flex justify-content-center align-items-center' sm={3}>
            <button className='hyper-text' onClick={verifyProfile}>VERIFY PROFILE</button>
            <ShowProfile data={[show, handleClick, data[0]]}/>
        </Col>
    </div>
  )
}
