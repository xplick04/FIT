import React from 'react'
import { useState, useEffect, useContext } from 'react'
import axios from 'axios'

import {
	Modal,
	Button,
} from 'react-bootstrap'

import AuthContext from '../context/AuthContext'
import configData from '../config.json'

import Record from './Record'
import NewRecord from './NewRecord'

import myButton from '../styles/Button.module.css'
import colors from '../styles/Colors.module.css'

const Records = ({show, handleClose, animal_id}) => {

	let {authTokens} = useContext(AuthContext);

	const [records, setRecords] = useState({});

	const [showNewRecord, setShowNewRecord] = useState(false);
	const handleShowNewRecord = () => setShowNewRecord(true);
	const handleCloseNewRecord = () => setShowNewRecord(false);

	const getRecords = async (e) => {
		const options = {
			method: 'GET',
			url: configData.API_URL + 'record-list/' + animal_id + '/',
			headers: {
				'Content-Type':'application/json',
				'Authorization':'Bearer ' + String(authTokens.access)
			}
		};
		axios(options).then((response) => {
			setRecords(response.data);
		});
	};

	useEffect(() => {
		getRecords();
	}, []);

	return (
		<Modal className={`${colors.Dark}`} show={show} onHide={handleClose} centered>
			<Modal.Header>
				<Modal.Title>Medical records</Modal.Title>
			</Modal.Header>
			<Modal.Body>
				{(records?.length > 0 && Array.isArray(records)) &&
					<>
					{records.map((record) => (
						<Record
							record={record}
							animal_id={animal_id}
							key={record.id}/>
					))}
					</>
				}
			</Modal.Body>
			<Modal.Footer>
				<NewRecord
					show={showNewRecord} 
					handleClose={handleCloseNewRecord}
					getRecords={getRecords}
					animal_id={animal_id}/>
				<Button
					className={`${myButton.myButton}`}
					onClick={handleShowNewRecord}>
						New Record
				</Button>
			</Modal.Footer>
		</Modal>
	)
}

export default Records
