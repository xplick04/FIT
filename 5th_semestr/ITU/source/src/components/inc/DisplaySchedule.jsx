/**
 * @brief DisplaySchedule component file
 * 
 * @file DisplaySchedule.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React, { useState } from 'react'
import { Card } from 'react-bootstrap'
import { DisplayScheduleReservations } from './DisplayScheduleReservations';

export const DisplaySchedule = ({data}) => {

    //modal window schedule
    const [show, setShow] = useState(false);

    //modal window, click => hide, click => show
    const handleClick = () => {
        setShow(current => !current)
    };

  return (
    <Card style={{width: "10em", height: "9em"}} className="schedule m-3">
        <Card.Title className='text-center'>{data.date.slice(8,10) + "." +data.date.slice(5,7) + "." + data.date.slice(0,4)}</Card.Title>
        <Card.Title className='text-center'>{data.s_from.slice(0,5)}-{data.s_to.slice(0,5)}</Card.Title>
        <Card.Body className='d-flex justify-content-center align-items-center'>
            <button className='hyper-text mt-2' onClick={handleClick}>
                SHOW MORE
            </button>
            <DisplayScheduleReservations dataReservations={[show, handleClick, data]} />
        </Card.Body>
    </Card>
  )
}
