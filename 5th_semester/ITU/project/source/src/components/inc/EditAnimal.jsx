/**
 * @brief EditAnimal component file
 * 
 * @file EditAnimal.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from 'react'
import { Container, Modal, Form, Col } from 'react-bootstrap'
import { AuthContext } from "../../context/AuthContext";
import axios from 'axios';


export const EditAnimal = ({editData}) => {

  //urls for axios requests
  const EDIT_ANIMAL_URL = 'https://sigma-animal-shelter.herokuapp.com/api/animal-detail/'
  const DELETE_ANIMAL_URL = 'https://sigma-animal-shelter.herokuapp.com/api/animal-delete/'

  //creating animal state
  let [state, setState] = useState(editData[2]);

  //binding input values to animal
  let updateInput = (e) => {
      setState({
          ...state,
              [e.target.name] : e.target.value
      })
  };

  //creating animal photo
  let [photo, setPhoto] = useState(null);

  //binding input files to animal photo
  let updatePhoto = (e) => {setPhoto(e.target.files[0])};

  //getting JWT token from AuthContext
  let {authTokens} = useContext(AuthContext);
  
  //edit animal axios request
  let editAnimal = (e) => {
      e.preventDefault();
      let options = {
          method: 'POST',
          url: EDIT_ANIMAL_URL + editData[2].id + "/",
          withCredentials: true,
          headers: {
          'Content-Type':'multipart/form-data',
          'Authorization':'Bearer ' + String(authTokens?.access)
          },
          data: {
              name : state.name,
              breed : state.breed,
              date_of_birth : state.date_of_birth,
              description: state.description,
              found_info: state.found_info,
              sex: state.sex,
              photo: photo,
          }
      };
      axios(options).then((response) => {
        if(response.data !== "Animal is not updated."){
          alert("Animal was updated!");
          //getting updated animal list
          editData[3]();
        }
        else{
          alert("Animal not updated!");
        }
      });
  };


  let deleteAnimal = (e) => {
    e.preventDefault();
    let options = {
        method: 'DELETE',
        url: DELETE_ANIMAL_URL + editData[2].id + "/",
        withCredentials: true,
        headers: {
        'Content-Type':'application/json',
        'Authorization':'Bearer ' + String(authTokens?.access)
        },
    };
    axios(options).then((response) => {
      if(response.data !== "Animal is not deleted."){
        //closing modal window
        editData[1]();
        //getting updated animal list
        editData[3]();
        alert("Animal was deleted!");
      }
      else{
        alert("Animal was not deleted!");
      }
    });
  };
          
  return (
    <>
     <Modal  show={editData[0]} onHide={editData[1]} centered="true">
    <Modal.Header closeButton>
    </Modal.Header>
    <Modal.Body>
      <Container className="" style={{padding:"30px"}}>
        <Form style={{width:"400px"}}>
              <Form.Group className="">
              <Form.Label className="login-text mb-0">Name:</Form.Label>
              <Form.Control name="name" type="text" placeholder={editData[2].name} onChange={updateInput}/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Breed:</Form.Label>
              <Form.Control name="breed" type="text" placeholder={editData[2].breed} onChange={updateInput}/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Date of birth:</Form.Label>
              <Form.Control name="date_of_birth" type="date" value={state.date_of_birth} 
              max={new Date().toISOString().split("T")[0]} onChange={updateInput}/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Description:</Form.Label>
              <Form.Control name="description" type="text" placeholder={editData[2].description} onChange={updateInput}/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Found informations:</Form.Label>
              <Form.Control name="found_info" type="text" placeholder={editData[2].found_info} onChange={updateInput}/>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Sex:</Form.Label>
              <Form.Select name="sex" value={state.sex} onChange={updateInput}>
                <option value="Girl">Girl</option>
                <option value="Boy">Boy</option>
                <option value="Unknown">Unknown</option>
              </Form.Select>
              </Form.Group>

              <Form.Group className="mt-2">
              <Form.Label className="login-text mb-0">Photo:</Form.Label>
              <Form.Control name="photo" type="file" onChange={updatePhoto}/>
              </Form.Group>

              <div className="mt-4 d-flex">
                  <Col className='d-flex justify-content-center'>
                    <button type="submit" className="submit-button" onClick={editAnimal}>EDIT CHANGES</button>
                  </Col>
                  <Col className='d-flex justify-content-center'>
                    <button type="submit" className="submit-button" onClick={deleteAnimal}>DELETE ANIMAL</button>
                  </Col>
              </div>
            </Form>
      </Container>
    </Modal.Body>
    </Modal>
    </>
  )
}
