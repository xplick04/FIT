/**
 * @brief DisplayAnimal component file
 * 
 * @file DisplayAnimal.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from 'react'
import { Card, Container, Row, Col, Form } from 'react-bootstrap';
import { AuthContext } from "../../context/AuthContext";
import axios from 'axios';
import { DisplaySchedules } from './DisplaySchedules';
import { DisplayRecords } from './DisplayRecords';
import { EditAnimal } from './EditAnimal';

export const DisplayAnimal = ({data}) => {

    //url for axios request
    const REQUEST_CREATE_URL = 'https://sigma-animal-shelter.herokuapp.com/api/request-create/';

    //getting user and JWT token from AuthContext
    let {user} = useContext(AuthContext);
    let {authTokens} = useContext(AuthContext);

    //states for buttons
    const [openInfo, setOpenInfo] = useState(true);
    const [openRequest, setOpenRequest] = useState(false);
    const [openSchedule, setOpenSchedule] = useState(false);
    const [openRecord, setOpenRecord] = useState(false);
    const [openWalk, setOpenWalk] = useState(false);

    //modal window for adding new animal
    const [show, setShow] = useState(false);

    //modal window, click => hide, click => show
    const handleClick = () => {
        setShow(current => !current)
    };

    //function for button handling
    let handleClick2 = (openNumber) => {
      setOpenInfo(false);
      setOpenRequest(false);
      setOpenSchedule(false);
      setOpenRecord(false);
      setOpenWalk(false);
      if(openNumber === 1) setOpenInfo(true);
      else if(openNumber === 2) setOpenRequest(true);
      else if(openNumber === 3) setOpenSchedule(true);
      else if(openNumber === 4) setOpenRecord(true);
      else if(openNumber === 5) setOpenWalk(true);
    };
    
    //creating reqeust
    let [request, setRequest] = useState({
      title : '',
      description : ''
    });
  
    //binding input values to request
    let updateInput = (e) => {
      setRequest({
          ...request,
              [e.target.name] : e.target.value
      })
    };

    //axios request for creating request on particular animal
    let sendRequest = (e) => {
      const current = new Date();
      const dateToday = `${current.getFullYear()}-${current.getMonth()+1}-${current.getDate()}`;
      e.preventDefault();
      let options = {
        method: 'POST',
        url: REQUEST_CREATE_URL,
        withCredentials: true,
        headers: {
            'Content-Type':'application/json',
            'Authorization':'Bearer ' + String(authTokens?.access)
        },
        data: {
          date: dateToday,  //today
          title: request.title,
          description: request.description,
          animal: data[0].id
        }
      };
      axios(options).then((response) => {
        if(response.data !== "Request is not created."){
          alert("Request sent!");
        }
        else{
          alert("Request not sent!");
        }
      })
    };

    //for displaying animal age
    const todayYear = new Date().getFullYear();

  return (
    <Card key={data[0].id} className='mt-4 shadow-lg'>
        <Card.Title className='text-center mt-2'>{data[0].breed} {data[0].name}, {" "}  
        {
          //current year - year of birth
          parseInt(todayYear , 10) - parseInt(data[0].date_of_birth.split('-')[0] , 10 )
        } years</Card.Title>
        <Card.Body>
          <Row className="mt-2">
            <Col>     
              <button className={ openInfo ? 'nav-button-a':'nav-button'} onClick={() => handleClick2(1)}>INFORMATIONS</button>
            </Col>
            <Col> 
              {user?.type==="Caretaker" && <button className={ openRequest ? 'nav-button-a':'nav-button'} onClick={() => handleClick2(2)}>NEW REQUEST</button>}
              {user?.type==="Veterinarian" && <button className={ openRecord ? 'nav-button-a':'nav-button'} onClick={() => handleClick2(4)}>MEDICAL RECORDS</button>}
              {user?.type==="Registered" && <button className={ openWalk ? 'nav-button-a':'nav-button'} onClick={() => handleClick2(5)}>TAKE FOR A WALK</button>}
            </Col>
            <Col> 
              {user?.type==="Caretaker" && <button className={ openSchedule ? 'nav-button-a':'nav-button'} onClick={() => handleClick2(3)}>SCHEDULES</button>}
            </Col>
            <Col></Col>
          </Row>
          {openInfo &&
          <div className='mt-4 mb-4 d-flex flex-wrap'>
            <Col sm={8} style={{minWidth:"300px"}}>
                <Container>
                <Card.Title>Description</Card.Title>
                <Card.Text>{data[0].description}</Card.Text>
                <Card.Title>Found info</Card.Title>
                <Card.Text>{data[0].found_info}</Card.Text>
                <Card.Title>Sex</Card.Title>
                <Card.Text>{data[0].sex}</Card.Text>
                </Container>
                {user?.type==="Caretaker" &&
                  <div className='d-flex justify-content-center'>
                  <button className='hyper-text' onClick={handleClick}>EDIT</button>
                  <EditAnimal editData={[show, handleClick, data[0], data[1]]}/>
                  </div>
                }
            </Col>
            <Col className='d-flex justify-content-center' sm={4} style={{minWidth:"300px"}}>
                {data[0].photo ?
                <Card.Img style={{maxWidth: "250px", maxHeight: "250px", objectFit: 'contain'}} src={data[0].photo}/>
                : <Card.Img style={{maxWidth: "250px", maxHeight: "250px"}}/>
                }
            </Col>
          </div>
          }
          {openRequest && 
          <Row className='mt-4 mb-4'>
            <Col>
                <Container>
                  <Form onSubmit={sendRequest}>
                    <Form.Group>
                    <Form.Label className="login-text mb-0">*Title:</Form.Label>
                    <Form.Control name="title" type="text" onChange={updateInput} required/>
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">*Description:</Form.Label>
                    <Form.Control name="description" as="textarea" onChange={updateInput} rows={3} style={{resize: "none"}} required/>
                    </Form.Group>
                    <div className='d-flex justify-content-center'>
                      <button className='hyper-text mt-4'>SUBMIT</button>
                    </div>
                  </Form>
                </Container>
            </Col>
          </Row>
          }
          {openSchedule && 
          <Row className='mt-4 mb-4'>
            <Col>
                <Container>
                  <DisplaySchedules data={data[0]}/>
                </Container>
            </Col>
          </Row>
          }
          {openRecord && 
          <Row className='mt-4 mb-4'>
            <Col>
                <DisplayRecords data={data[0]}/>
            </Col>
          </Row>
          }
          {openWalk && 
          <Row className='mt-4 mb-4'>
            <Col>
                <Container>
                  <DisplaySchedules data={data[0]}/>
                </Container>
            </Col>
          </Row>
          }
        </Card.Body>
    </Card>
  )
};
