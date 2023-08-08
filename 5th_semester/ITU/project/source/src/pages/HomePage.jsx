/**
 * @brief Homepage page component file
 * 
 * @file HomePage.jsx
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import React from 'react';
import { Card, Row, Col } from 'react-bootstrap';


export const HomePage = () => {
  return (
    <div className='d-flex justify-content-center align-items-center mt-4 mb-4'>
      <Card className='homePage-content d-flex flex-column align-items-center' style={{width: "60%"}}>
      <Card.Header className='d-flex flex-column align-items-center'>
          <h1 className='text-center mt-2'>Sigma Animal Shelter</h1>
          <div className="mt-4" style={{width: "80%"}}>
          <p>
            An animal shelter or pound is a place where stray, lost, abandoned or surrendered animals –
             mostly dogs and cats – are housed. The word "pound" has its origins in the animal pounds of 
             agricultural communities, where stray livestock would be penned or impounded until they were 
             claimed by their owners.
          </p> 
          <p>
            While no-kill shelters exist, it is sometimes policy to euthanize animals that are not claimed 
            quickly enough by a previous or new owner. In Europe, of the 30 countries included in a survey, 
            all but six (Austria, the Czech Republic, Germany, Greece, Italy and Poland) permitted euthanizing
             non-adopted animals.
            </p>
          </div>
        </Card.Header>
        <Card.Body>

        <h2 className='text-center'>Where to find us</h2>
        <div className="mt-2">
        <Row className='centered'>
          <Col>
          <Card className='clear-card'>
            <Card.Body className='clear-card'>
            <Card.Title className='text-center'>Based in</Card.Title>
            <div className='d-flex justify-content-center'>
            <img alt="map" src="https://img.freepik.com/free-vector/grey-world-map_1053-431.jpg?w=1060&t=st=1668630744~exp=1668631344~hmac=4b139797e27ad7caef99afbc42706c052cb2050130cb3fb2ac8d645d462cef83"
              style={{width: "300px"}} />
            </div>
            </Card.Body>
            <Card.Footer className='text-center clear-card'>
              <p><b>Open</b> : 8:00 - 17:00, everyday</p>
            </Card.Footer>
          </Card>
          </Col>
          <Col>
          <Card className='clear-card text-center'>
              <Card.Body className='clear-card'>
                <Card.Title>Contact</Card.Title>
                <p><b>Phone</b> : 123 456 789</p>
                <p><b>Mail</b> : example@gmail.com</p>
              </Card.Body>
              <Card.Footer className='clear-card mt-5'>
                <Card.Title>Socials</Card.Title>
                <div className='d-flex justify-content-between'>
                  <div className='socials d-flex justify-content-center align-items-center'>
                    <a href="https://www.facebook.com">FB</a>
                  </div>
                  <div className='socials d-flex justify-content-center align-items-center'>
                    <a href="https://www.instagram.com">IG</a>
                  </div>
                  <div className='socials d-flex justify-content-center align-items-center'>
                    <a href="https://www.twitter.com">TW</a>
                  </div>
                </div>
              </Card.Footer>
          </Card>
          </Col>
        </Row>
        </div>

        </Card.Body>
        <Card.Footer className='d-flex flex-column' style={{width: "100%"}}>
          <h2 className='text-center'>Gallery</h2>
            <Row className='d-flex flex-wrap mb-3 justify-content-center mt-2'>
              <Col className='d-flex justify-content-center mt-2'>
                <img alt="hp1" src="https://images.pexels.com/photos/47547/squirrel-animal-cute-rodents-47547.jpeg?cs=srgb&dl=pexels-pixabay-47547.jpg&fm=jpg" 
                style={{width: "300px", height: "300px", borderRadius: "10px"}}/>
              </Col>
              <Col className='d-flex justify-content-center mt-2'>
                <img alt="hp2" src="https://i.guim.co.uk/img/media/75226b6594c05319bea872da5e8f83d389cd8eb6/0_78_4629_2777/master/4629.jpg?width=1200&height=1200&quality=85&auto=format&fit=crop&s=2c146c7c9597d872fd238999d8056ac6" 
                style={{width: "300px", height: "300px", borderRadius: "10px"}}/>
              </Col>
              <Col className='d-flex justify-content-center mt-2'>
                <img alt="hp3" src="https://allthatsinteresting.com/wordpress/wp-content/uploads/2018/12/squirrel-does-the-splits.jpg" 
                style={{width: "300px", height: "300px", borderRadius: "10px"}}/>
              </Col>
            </Row>
        </Card.Footer>

      </Card>
    </div>
  )
};
