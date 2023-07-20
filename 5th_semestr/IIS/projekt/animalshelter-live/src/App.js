import React from 'react'
import {Routes, Route} from 'react-router-dom'

import Navigbar from './components/Navbar'
import Home from './pages/Home'
import Animals from './pages/Animals'
import Admin from './pages/Admin'
import Walks from './pages/Walks'
import Requests from './pages/Requests'
import UnverifiedUsers from './pages/UnverifiedUsers'
import Foot from './components/Foot'

import { AuthProvider } from './context/AuthContext'
import  UserDetail from './pages/UserDetail'
const App = () => {
  return (
	<>
		<AuthProvider>
			<Navigbar></Navigbar>
			<Routes>
				<Route path='/' element={<Home/>}/>
				<Route path='/animals' element={<Animals/>}/>
				<Route path='/administrator' element={<Admin/>}/>
				<Route path='/user' element={<UserDetail/>}/>
				<Route path='/walks' element={<Walks/>}/>
				<Route path='/requests' element={<Requests/>}/>
				<Route path='/users' element={<UnverifiedUsers/>}/>
			</Routes>
			<Foot></Foot>
		</AuthProvider>
	</>
  )
}

export default App

