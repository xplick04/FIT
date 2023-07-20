import { createContext, useState, useEffect } from "react";
import axios from 'axios'
import jwt_decode from 'jwt-decode'

import configData from "../config.json"

const AuthContext = createContext();

export default AuthContext;

export const AuthProvider = ({children}) => {

	const [authTokens, setAuthTokens] = useState(() =>
		localStorage.getItem('authTokens')
			? JSON.parse(localStorage.getItem('authTokens'))
			: null
	);
	const [user, setUser] = useState(() =>
		localStorage.getItem('authTokens')
			? jwt_decode(localStorage.getItem('authTokens'))
			: null
	);
	const [loading, setLoading] = useState(true);

	const loginUser = async (e) => {
		e.preventDefault();

		const options = {
			method: 'POST',
			url: configData.API_URL + 'token/',
			headers: {
				'Content-Type':'application/json',
			},
			data:{
				'username':e.target.username.value,
				'password':e.target.password.value
			}
		};

		axios(options)
			.then((response) => {
				setAuthTokens(response.data);
				setUser(jwt_decode(response.data.access));
				localStorage.setItem(
					'authTokens', JSON.stringify(response.data)
				);

				window.location.reload(false);
			})
			.catch((error) => {
				if(error?.response.status === 401){
					alert('Wrong username or password.');
				}
			});
	}

	const logoutUser = () => {
		setAuthTokens(null);
		setUser(null);
		localStorage.removeItem('authTokens');
	}

	const updateToken = async () => {
		const options = {
			method: 'POST',
			url: configData.API_URL + 'token/refresh/',
			headers: {
				'Content-Type':'application/json',
			},
			data:{
				'refresh':authTokens?.refresh
			}
		};

		if(authTokens?.refresh){
			await axios(options).then((response) => {
				setAuthTokens(response.data);
				setUser(jwt_decode(response.data.access));
				localStorage.setItem(
					'authTokens', JSON.stringify(response.data)
					);
				});
		}else{
			logoutUser();
		}

		if(loading){
			setLoading(false);
		}
	}

	let contextData = {
		user:user,
		loginUser:loginUser,
		logoutUser:logoutUser,
		authTokens:authTokens
	};

	useEffect(() => {

		if(loading){
			updateToken();
		}

		let fourMinutes = 1000 * 60 * 4;
		let  interval = setInterval(() => {
			if(authTokens){
				updateToken();
			}
		}, fourMinutes);
		return () => clearInterval(interval);
	}, [authTokens, loading]);

	return (
		<AuthContext.Provider value={contextData}>
			{(loading)
				?(null)
				:children
			}
		</AuthContext.Provider>
	)
}