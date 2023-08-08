/**
 * @brief ShowProfile component file
 * 
 * @file ShowProfile.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React from 'react'
import { Form, Modal, Container} from "react-bootstrap";

export const ShowProfile = ({data}) => {
  return (
    <>
      <Modal  show={data[0]} onHide={data[1]} centered="true">
      <Modal.Header closeButton>
      </Modal.Header>
      <Modal.Body>
          <Container className="" style={{padding:"30px"}}>
            <Form style={{width:"400px"}}>
                    <Form.Group className="">
                    <Form.Label className="login-text mb-0">Username:</Form.Label>
                    <Form.Control value={data[2].username} disabled/>
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">First name:</Form.Label>
                    <Form.Control value={data[2].first_name} disabled/>
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">Last name:</Form.Label>
                    <Form.Control value={data[2].last_name} disabled/>
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">Date of birth:</Form.Label>
                    <Form.Control value={data[2].date_of_birth} disabled/>
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">Type:</Form.Label>
                    <Form.Control value={data[2].type} disabled>
                    </Form.Control>
                    </Form.Group>

                    <Form.Group className="mt-2">
                    <Form.Label className="login-text mb-0">Verified:</Form.Label>
                    <Form.Control placeholder={data[2].verified ? "Yes" : "No"} disabled/>
                    </Form.Group>
                  </Form>
          </Container>
      </Modal.Body>
      </Modal>
    </>
  )
}
