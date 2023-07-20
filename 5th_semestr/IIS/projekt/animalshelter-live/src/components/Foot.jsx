import React from 'react'

import {
	Container,
	Row,
	Col,
	Image
} from 'react-bootstrap'

import styles from '../styles/Footer.module.css'
import colors from '../styles/Colors.module.css'

const Foot = () => {
	return (
		<Container className={`${colors.White} ${colors.BgLight} rounded mt-3 mb-3 shadow`}>
			<Row className={`${styles.Row}`}>
				<Col className='p-3'>
					<h3>Contact information</h3>
					<p>
						<b>Address: </b>
						Božetěchova 1/2, 612 00 Brno-Královo Pole
					</p>
					<p>
						<b>Phone number: </b> 
						775 603 558
					</p>
					<p>
						<b>Email address: </b> 
						animal.shelter@gmail.com
					</p>
				</Col>
				<Col className='p-0'>
					<Image
						className={`${styles.Map} rounded`}
						src='https://www.mensjournal.com/wp-content/uploads/screen-shot-2017-03-29-at-13810-pm-a887f892-960b-4efe-8de0-fa2501d9134f.png?w=900&h=506&crop=1&quality=86&strip=all'></Image>
				</Col>
			</Row>
		</Container>
	)
}

export default Foot
