import React from 'react'

import {
	Card,
} from 'react-bootstrap'

const Record = ({record}) => {
	return (
		<Card className='m-3'>
			<Card.Body>
				<Card.Title>{record.title}</Card.Title>
				<Card.Text>{record.description}</Card.Text>
			</Card.Body>
		</Card>
	)
}

export default Record