/**
 * @brief navbar component file
 * 
 * @file MyNav.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useContext } from "react";
import { Container, Navbar } from "react-bootstrap";
import "bootstrap/dist/css/bootstrap.min.css";
import { NavLink } from "react-router-dom";
import { AuthContext } from "../../context/AuthContext";
import { Login } from './Login';

export const MyNav = () => {

    //getting user from AuthContext
    let {user} = useContext(AuthContext);
  
    //modal window login user
    const [show, setShow] = useState(false);
  
    //modal window, click => hide, click => show
    const handleClick = () => setShow(current => !current);

  return (
    <>
     <Navbar sticky="top" bg="white" className="navbar justify-content-md-center" expand="sm">
        <Container className="justify-space-between flex-nowrap" style={{width:"50%"}}>
            <NavLink className={({isActive}) => isActive ? "nav-button-a" : "nav-button" } to="/">HOME</NavLink>
            <NavLink className={({isActive}) => isActive ? "nav-button-a" : "nav-button" } to="/OurAnimals">OUR ANIMALS</NavLink>
            {!user && <NavLink className="nav-button" onClick={handleClick}>LOGIN</NavLink>}
            {user?.type === "Administrator" && <NavLink className={({isActive}) => isActive ? "nav-button-a" : "nav-button" } to="/ManageUsers">MANAGE USERS</NavLink>}
            {user?.type === "Caretaker" && <NavLink className={({isActive}) => isActive ? "nav-button-a" : "nav-button" } to="/Requests">REQUESTS</NavLink>}
            {user?.type === "Veterinarian" && <NavLink className={({isActive}) => isActive ? "nav-button-a" : "nav-button" } to="/VetRequests">REQUESTS</NavLink>}
            {user?.type === "Registered" && <NavLink className={({isActive}) => isActive ? "nav-button-a" : "nav-button" } to="/MyWalks">MY WALKS</NavLink>}
            {user && <NavLink className={({isActive}) => isActive ? "nav-button-a" : "nav-button" } to="/MyProfile">MY PROFILE</NavLink>}
            <Login data ={[show, handleClick]} />
        </Container>
    </Navbar>
    </>
  )
};
