/**
 * @brief Global data context file
 * 
 * @file AuthContext.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState, useEffect, createContext } from 'react';
import axios from 'axios';
import jwt_decode from "jwt-decode";
import { useNavigate } from "react-router-dom";

//creating context for authorization
export const AuthContext = createContext();

export const AuthProvider = ({children}) => {

    //urls for axios requests
    const LOGIN_URL = "https://sigma-animal-shelter.herokuapp.com/api/token/";
    const REFRESH_URL = "https://sigma-animal-shelter.herokuapp.com/api/token/refresh/";

    //logged in user tokens, if token is in memory it loads from there
    let [authTokens, setAuthTokens] = useState(() =>
        localStorage.getItem("authTokens") ? 
        JSON.parse(localStorage.getItem("authTokens")) : null
        );

    //logged in user object
    let [user, setUser] = useState(() =>
        localStorage.getItem("authTokens") ? 
        jwt_decode(localStorage.getItem("authTokens")) : null
        );

    //for redirecting user
    const navigate = useNavigate();

    //login function
    let loginUser = (e) => {
        e.preventDefault();
        let options = {
            method: 'POST',
            url: LOGIN_URL,
            headers: {
                'Content-Type':'application/json',
            },
            data: {
                username: e.target.username.value,
                password: e.target.password.value
            }
        };
        //trying to execute POST request
        axios(options).then((response) => {
            setAuthTokens(response.data);
            setUser(jwt_decode(response.data.access));
            localStorage.setItem("authTokens", JSON.stringify(response.data));
            navigate("/");
            window.location.reload(false);
        })
        .catch ((error) => {
            if(error.response.status === 401){
                alert("Username or password is incorrect!");
            }
            else{
                alert("Something went wrong!");
            }
        });
    };

    //logout function, removes user data from local storage and redirect user to homepage
    let logoutUser = () => {
        setAuthTokens(null);
        setUser(null);
        localStorage.removeItem("authTokens");
        navigate("/");
        alert("User logged out succesfully!");
    };

    //loading states
    let [loading, setLoading] = useState(true);

    //reload access token
    useEffect(() => {
        //update token
        let updateToken = async () => {
            let options = {
                method: 'POST',
                url: REFRESH_URL,
                headers: {
                    'Content-Type':'application/json',
                },
                data: {
                    refresh: authTokens?.refresh,
                }
            };
            //trying to execute POST request
            if(authTokens?.refresh){
                await axios(options).then((response) => {
                    setAuthTokens(response.data);
                    setUser(jwt_decode(response.data.access));
                    localStorage.setItem("authTokens", JSON.stringify(response.data));
                })
            }
            else{
                setAuthTokens(null);
                setUser(null);
                localStorage.removeItem("authTokens");
            }
            if(loading){
                setLoading(false);
            }
        };
        if(loading)
        {
            updateToken();
        }
        let time = 1000 * 60 * 4;   //interval for refreshing token
        let interval = setInterval(() => {
            if(authTokens)
            {
                updateToken();
            }
        }, time);
        return () => clearInterval(interval)
    }, [authTokens, loading]);

    //data accesible from context
    let contextData = {
        user: user,
        authTokens: authTokens,
        loginUser: loginUser,
        logoutUser: logoutUser,
    };

    return(
        <AuthContext.Provider value={contextData}>
            {(loading) ? null : children}
        </AuthContext.Provider>
    );
};