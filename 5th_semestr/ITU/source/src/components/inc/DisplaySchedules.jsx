/**
 * @brief DisplaySchedules component file
 * 
 * @file DisplaySchedules.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext, useEffect } from 'react'
import { Container, Card } from "react-bootstrap";
import { AuthContext } from '../../context/AuthContext';
import axios from 'axios';
import { DisplaySchedule } from './DisplaySchedule';
import { AddSchedule } from './AddSchedule';

export const DisplaySchedules = ({data}) => {

  //url for axios request
  let SCHEDULE_DETAIL_URL = "https://sigma-animal-shelter.herokuapp.com/api/schedule-list/";

  //getting user and JWT token from AuthContext
  let {user} = useContext(AuthContext);
  let {authTokens} = useContext(AuthContext);

  //creatin schedule list
  let [state, setState] = useState({
    schedules : []
  });

  //getting schedules from db
  let getSchedules = () => {
    let options = {
        method: 'GET',
        url: SCHEDULE_DETAIL_URL + data?.id + "/",
        withCredentials: true,
        headers: {
            'Content-Type':'application/json',
            'Authorization':'Bearer ' + String(authTokens?.access)
        }
    };
    axios(options).then((response) => {
      setState({
        ...state,
        schedules: response.data
      });
    });
};

  //modal window schedule
  const [show, setShow] = useState(false);

  //modal window, click => hide, click => show
  const handleClick = () => {
      setShow(current => !current)
  };

  //after load
  useEffect(() => {
    getSchedules();
  }, []);

  return (
    <Container className="d-flex justify-content-center flex-wrap schedule-container" style={{height: "14em", overflowY: "auto"}}>
      {user.type === "Caretaker" &&
      <Card className="schedule m-3" style={{width: "10em", height: "9em"}}>
      <Card.Body className='d-flex justify-content-center align-items-center'>
          <button className='hyper-text mt-2' onClick={handleClick}>
              NEW SCHEDULE
          </button>
          <AddSchedule data={[show, handleClick, data, getSchedules]}/>
      </Card.Body>
      </Card>
      }

      {state.schedules.length > 0 ? 
        state.schedules.map(schedule => {
          return (
            <>
              <DisplaySchedule data={schedule} />
            </>
          );
        }) : <div className='d-flex justify-content-center align-items-center empty-request-font'>There are no schedules for this animal!</div>
        }
    </Container>
  )
}
