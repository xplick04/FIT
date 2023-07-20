import React from 'react'
import { useContext } from 'react'

import {
	Navbar,
	Nav,
	Container
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'

import styles from '../styles/NavItem.module.css'
import colors from '../styles/Colors.module.css'

const Navigbar = () => {

	let {user} = useContext(AuthContext);

	return (
		<Navbar style={{backgroundColor: "white"}} expand="lg" className='p-0 fixed-top'>
			<Container>
				<Navbar.Brand className={`${colors.Dark}`}>Animal Shelter</Navbar.Brand>
				<Navbar.Toggle aria-controls='basic-navbar-nav'></Navbar.Toggle>
				<Navbar.Collapse id='basic-navbar-nav'>
					<Nav className='justify-content-end' style={{ width: "100%" }}>
						<Nav.Link
							className={`${styles.NavItem} ${colors.Dark}`}
							href='/'>Home</Nav.Link>
						<Nav.Link
							className={`${styles.NavItem} ${colors.Dark}`}
							href='/animals'>Animals</Nav.Link>
						{(user?.type === 'Administrator') &&
							<Nav.Link
							className={`${styles.NavItem} ${colors.Dark}`}
								href='/administrator'>Admin</Nav.Link>
						}
						{(user?.type === 'Registered') &&
							<Nav.Link
								href='/walks'
								className={`${styles.NavItem} ${colors.Dark}`}
								>Walks</Nav.Link>
						}
						{(user?.type === 'Veterinarian') &&
							<Nav.Link href='/requests'
							className={`${styles.NavItem} ${colors.Dark}`}
								>Requests</Nav.Link>
						}
						{(user?.type === 'Caretaker') &&
							<Nav.Link href='/users'
							className={`${styles.NavItem} ${colors.Dark}`}
								>Unverified Users</Nav.Link>
						}
						<Nav.Link href='/user'
							className={`${styles.NavItem} ${colors.Dark}`}
							>MyProfile</Nav.Link>
					</Nav>
				</Navbar.Collapse>
			</Container>
    	</Navbar>
	)
}

export default Navigbar
