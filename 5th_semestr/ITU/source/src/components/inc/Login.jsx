/**
 * @brief Login component file
 * 
 * @file Login.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useContext } from "react";
import { Form, Modal, Col, Row, Container} from "react-bootstrap";
import { Link } from "react-router-dom";
import { AuthContext } from "../../context/AuthContext";

export const Login = (props) => {

    //for calling login function from AuthContext
    let {loginUser} = useContext(AuthContext);

    return (
        <>
            <Modal  show={props.data[0]} onHide={props.data[1]} centered="true">
            <Modal.Header closeButton>
            </Modal.Header>
            <Modal.Body>
                <Container className="d-flex justify-content-center" style={{padding:"30px"}}>
                    <Row className="mt-2">
                        <Col>
                            <Form onSubmit={loginUser}>
                                <Form.Group>
                                    <Form.Label className="login-text mb-0">Username:</Form.Label>
                                    <Form.Control name="username" type="text" placeholder="Enter username" required/>
                                </Form.Group>
                                <Form.Group className="mt-2">
                                    <Form.Label className="login-text mb-0">Password:</Form.Label>
                                    <Form.Control name="password" type="password" placeholder="Enter password" required />
                                </Form.Group>
                                <div className="mt-3 d-flex justify-content-between align-items-center">
                                    <div className="d-flex flex-column align-items-center">
                                        <p className="mb-0">No account?</p>
                                        <Link className="hyper-text" onClick={props.data[1]} to="/SignUp">Sign up</Link>
                                    </div>
                                    <button type="submit" className="submit-button">LOGIN</button>
                                </div>
                            </Form>
                        </Col>
                    </Row>
                </Container>
            </Modal.Body>
            </Modal>
        </>
    )
};
