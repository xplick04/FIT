import React from 'react'

import {
	Card,
	Col
} from 'react-bootstrap'

import colStyle from '../styles/Admin.module.css'
import styles from '../styles/Walks.module.css'
import colors from '../styles/Colors.module.css'

const Walk = ({walk}) => {
	return (
		<Col className={colStyle.Col}>
			<Card className={`${styles.Card} ${colors.Dark} p-3 shadow`}>
				<Card.Body>
					<Card.Title className={`${styles.Center}`}>
						Reservation
					</Card.Title>
					<Card.Text><b>From: </b>{walk.r_from}</Card.Text>
					<Card.Text><b>To: </b>{walk.r_to}</Card.Text>
					<Card.Text><b>Date: </b>{walk.schedule}</Card.Text>
					<Card.Text><b>Animal: </b>{walk.animal}</Card.Text>
					<Card.Text>id: {walk.id}</Card.Text>
					{(walk.confirmed === false) &&
						<Card.Text>
							Reservation has not been confirmed by caretaker.
						</Card.Text>
					}
				</Card.Body>
			</Card>
		</Col>
	)
}

export default Walk
