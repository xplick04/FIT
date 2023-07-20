import React from 'react'
import { useState, useEffect, useContext } from 'react'
import axios from 'axios'

import {
	Button,
	Modal,
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import ScheduleDetail from './ScheduleDetail'
import NewSchedule from './NewSchedule'

import myButton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const Schedule = ({show, handleClose, animal_id}) => {

	let {user, authTokens} = useContext(AuthContext);

	const [schedules, setSchedules] = useState([]);

	const [showNewSchedule, setShowNewSchedule] = useState(false);
	const handleShowNewSchedule = () => setShowNewSchedule(true);
	const handleCloseNewSchedule = () => setShowNewSchedule(false);

	const getSchedules = async () => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'schedule-list/' + animal_id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options)
			.then((response) => {
				setSchedules(response.data)
			});
	};

	useEffect(() => {
		getSchedules();
	}, []);

	return (
		<Modal
			className={`${colors.Dark}`}
			centered show={show} onHide={handleClose}>
			<Modal.Header>
				<Modal.Title>Schedule</Modal.Title>
			</Modal.Header>
			<Modal.Body>
				{(schedules?.length > 0 && Array.isArray(schedules)) &&
					<>
					{schedules.map((schedule) => (
						<ScheduleDetail
							schedule={schedule}
							key={schedule.id}/>
					))}
					</>
				}
			</Modal.Body>
			{(user?.type === 'Caretaker') &&
				<Modal.Footer>
					<NewSchedule
						show={showNewSchedule} 
						handleClose={handleCloseNewSchedule}
						getSchedules={getSchedules}
						animal_id={animal_id}/>
					<Button
						className={`${myButton.myButton}`}
						onClick={handleShowNewSchedule}>
							New Schedule
					</Button>
				</Modal.Footer>
			}
		</Modal>
	)
}

export default Schedule
