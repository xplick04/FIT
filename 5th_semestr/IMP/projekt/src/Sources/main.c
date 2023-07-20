/**
 * @brief IMP project - ARM-FITkit3: Hodiny s budíkem na bázi modulu Real Time Clock (RTC)
 *
 * @file main.c
 * @author Maxim Plièka (xplick04, 231813)
 * @date 12.12.2022
 */

#include "MK60D10.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


#define BUF_SIZ 20
#define LED1 0x0004
#define LED2 0x0008
#define LED3 0x0010
#define LED4 0x0020
#define LEDS 0x003C
#define BUZZER 0x0010
#define MINUTE 60
#define HOUR 3600

enum state {
	USEDEFAULT,
	SETSOUND,
	SETLIGHT,
	SETREPEATS,
	SETTIMEBETWEEN,
	SETTIMES,
	ALARM,
	ENDALARM
};

typedef struct s_times{
	short int hours;
	short int minutes;
} s_time;

char buffer[BUF_SIZ];
short int lightMode, soundMode, numOfRepeats, timeBetweenRepeats;
s_time currentTime, alarmTime;

void delay(uint64_t bound) {
	for (uint64_t i=0; i < bound; i++) { __NOP(); }
}

void MCUInit() {
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );	// set digital clock frequency to 48MHz
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);	// set clock divider to 1
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;	// turn off watchdog operations
}

void pinInit() {
	//turn on clock for
	SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;	//UART5
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;  //PORTA, PORTB a PORTE
	SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;	//RTC

	//bzzz
    PORTA->PCR[4] = PORT_PCR_MUX(0x01);	//BUZZER

    //MCU LEDs
    PORTB->PCR[2] = PORT_PCR_MUX(0x01);	// D9
	PORTB->PCR[3] = PORT_PCR_MUX(0x01);	// D10
	PORTB->PCR[4] = PORT_PCR_MUX(0x01);	// D11
	PORTB->PCR[5] = PORT_PCR_MUX(0x01);	// D12

	//communication
    PORTE->PCR[8] = PORT_PCR_MUX(0x03); // UART5_TX
    PORTE->PCR[9] = PORT_PCR_MUX(0x03); // UART5_RX

    //setting outputs
    PTA->PDDR = GPIO_PDDR_PDD(BUZZER); //set beep output
    PTB->PDDR = GPIO_PDDR_PDD(LEDS); //set LED output
    PTB->PDOR|= GPIO_PDDR_PDD(LEDS); //turn off LEDs

}

void UARTInit() {
	UART5->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);	// turn off reciever and transmiter while changing settings

	UART5->BDH = 0x00;	// interrupts disabled
	UART5->BDL = 0x1A;	// baud rate 115 200 Bd, 1 stop bit
	UART5->C4 = 0x0F;	// oversampling ratio 16, match address mode disabled

	UART5->C1 = 0x00;	// 8 data bites, no parity
	UART5->C3 = 0x00;
	UART5->MA1 = 0x00;	// no match address (mode disabled in C4)
	UART5->MA2 = 0x00;	// no match address (mode disabled in C4)

	UART5->S2 |= 0xC0;

	UART5->C2 |= ( UART_C2_TE_MASK | UART_C2_RE_MASK );	// turn on reciever and transmiter
}

void beep(){
	for(int i = 0; i < 500; i++){
		PTA->PDOR = GPIO_PDOR_PDO(BUZZER);
		delay(500);
		PTA->PDOR = GPIO_PDOR_PDO(0x0000);
		delay(500);
	}
}

void beep2(){
	for(int i = 0; i < 700; i++){
		PTA->PDOR = GPIO_PDOR_PDO(BUZZER);
		delay(700);
		PTA->PDOR = GPIO_PDOR_PDO(0x0000);
		delay(700);
	}
}

void sendCh(char c) {
    while(!(UART5->S1 & UART_S1_TDRE_MASK) && !(UART5->S1 & UART_S1_TC_MASK));
    UART5->D = c;
}

void sendStr(char* s) {
    unsigned int i = 0;
    while(s[i] != '\0') {
        sendCh(s[i++]);
    }
}

char receiveCh() {
    while(!(UART5->S1 & UART_S1_RDRF_MASK));
    return UART5->D;
}

char recieveStr() {
	memset(buffer, '\0', sizeof(buffer));	//empty buffer before new string

	for(int i = 0; i < (BUF_SIZ - 1); i++) {
		buffer[i] = receiveCh();
		sendCh(buffer[i]);	//send char back to user
		if (buffer[i] == '\r') {
			buffer[i] = '\0';
			break;
		}
	}
	sendStr("\r\n");
}

void sound(int mode){
	if(mode == 0 | mode == 1){
		delay(500000);
		beep();
	}

	if(mode == 0 | mode == 2){
		delay(500000);
		beep2();
	}

	if(mode == 0 | mode == 3){
		delay(500000);
		beep2();
		delay(100000);
		beep();
	}
}

void light(int mode){
	if (mode == 0 || mode == 1){
		delay(500000);
		PTB->PDOR &= ~GPIO_PDDR_PDD(LED1);
		delay(300000);
		PTB->PDOR &= ~GPIO_PDDR_PDD(LED2);
		delay(300000);
		PTB->PDOR &= ~GPIO_PDDR_PDD(LED3);
		delay(300000);
		PTB->PDOR &= ~GPIO_PDDR_PDD(LED4);
		delay(300000);
		PTB->PDOR |= GPIO_PDDR_PDD(LED1);
		delay(300000);
		PTB->PDOR |= GPIO_PDDR_PDD(LED2);
		delay(300000);
		PTB->PDOR |= GPIO_PDDR_PDD(LED3);
		delay(300000);
		PTB->PDOR |= GPIO_PDDR_PDD(LED4);
	}

	if (mode == 0 || mode == 2){
		delay(500000);
		PTB->PDOR &= ~GPIO_PDDR_PDD(LED1);
		PTB->PDOR &= ~GPIO_PDDR_PDD(LED4);
		delay(300000);
		PTB->PDOR |= GPIO_PDDR_PDD(LED1);
		PTB->PDOR |= GPIO_PDDR_PDD(LED4);
		PTB->PDOR &= ~GPIO_PDDR_PDD(LED2);
		PTB->PDOR &= ~GPIO_PDDR_PDD(LED3);
		delay(300000);
		PTB->PDOR |= GPIO_PDDR_PDD(LED2);
		PTB->PDOR |= GPIO_PDDR_PDD(LED3);

	}

	if (mode == 0 || mode == 3){
		delay(1000000);
		PTB->PDOR &= ~GPIO_PDDR_PDD(LEDS);
		delay(300000);
		PTB->PDOR |= GPIO_PDDR_PDD(LEDS);
	}
}

void RTC_IRQHandler() {
	sendStr("ALARM ");
	//print time when alarm starts
	memset(buffer, '\0', sizeof(buffer));
	sprintf(buffer, "%02d:%02d", alarmTime.hours, alarmTime.minutes);
	sendStr(buffer);
	sendStr("\r\n");

	for(int i = 0; i < 10; i++){
		sound(lightMode);
		light(soundMode);
	}

	if(numOfRepeats <= 0){
		RTC_TAR = 0;
	}
	else{
		alarmTime.minutes += timeBetweenRepeats;
		RTC_TAR += timeBetweenRepeats * MINUTE;
		numOfRepeats--;
	}
	NVIC_ClearPendingIRQ(RTC_IRQn);	//clear interrupt flag
}

void RTCInit(){
	//reset all RTC registers
	RTC_CR |= RTC_CR_SWR_MASK;
	RTC_CR &= ~RTC_CR_SWR_MASK;

	//RTC_SR[4] turn off clock for writing into TSR register
	RTC_SR &= RTC_SR_TCE_MASK;

	//TSR register, time register
	RTC_TSR = RTC_TSR_TSR(0x0000);

	//TAR register, alarm register
	RTC_TAR = RTC_TAR_TAR_MASK;

	//RTC_CR[8] 32.768 kHz oscillator is enabled + wait for starting
	RTC_CR |= RTC_CR_OSCE_MASK;
	delay(0x600000);

	//RTC_IER[2] Time Alarm Interrupt Enable
	RTC_IER |= RTC_IER_TAIE_MASK;

	//turn on clock
	RTC_SR |= RTC_SR_TCE_MASK;

	//enable interrupts from RTC
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);
}

bool isNumber(){
	for(int i = 0; buffer[i] != '\0'; i++){
		if(!isdigit(buffer[i])){
			sendStr("Invalid input!\r\n");
			return false;
		}
	}
	return true;
}

bool setTime(s_time *time){
	if( isdigit(buffer[0]) &&
		isdigit(buffer[1]) &&
		buffer[2] == ':'   &&
		isdigit(buffer[3]) &&
		isdigit(buffer[4]) &&
		buffer[5] == '\0'
		){
			char c_hours[3];
			char c_minutes[3];
			strncpy(c_hours, buffer, 2);
			strncpy(c_minutes, buffer + 3, 2);
			time->hours = atoi(c_hours);
			time->minutes = atoi(c_minutes);
			return true;
	}
	else{
		sendStr("Invalid time format!\r\n");
		return false;
	}
}

void setAlarm(s_time *currentTime, s_time *alarmTime){
	int currentSeconds = currentTime->hours * HOUR + currentTime->minutes * MINUTE;
	int alarmSeconds = alarmTime->hours * HOUR + alarmTime->minutes * MINUTE;
	int result = alarmSeconds - currentSeconds;

	if(result <= 0) result += 24 * HOUR;	//alarm for tomorrow
	RTC_TAR = RTC_TSR + result;	//RTC_TSR for setting time after restarting clock
}

int main() {
	enum state currentState = USEDEFAULT;

	//inits
	MCUInit();
	pinInit();
	UARTInit();
	RTCInit();

	delay(1000);

    while (1) {
    	switch (currentState){

			case (USEDEFAULT):
				sendStr("New alarm started. \r\n");
				sendStr("Do you want to use default light and sound for your alarm? [Y/N] \r\n");
				recieveStr();
				if( !strcmp(buffer, "Y") ){
					lightMode = 1;
					soundMode = 1;
					sendStr("Default light and sound settings selected!\r\n");
					currentState = SETREPEATS;
				}
				else if( !strcmp(buffer, "N") ){
					currentState = SETSOUND;
				}
				else{
					sendStr("Wrong input.\r\n");
				}
				break;

			case (SETSOUND):
				sendStr("Sound examples.\r\n");
				sound(0);
				delay(1000);
				sendStr("Select sound mode. [1/2/3]\r\n");
				recieveStr();
				if(isNumber()){
					soundMode = atoi(buffer);
					if(0 < lightMode < 4){
						sendStr("Sound mode selected!\r\n");
						currentState = SETLIGHT;
					}
					else{
						sendStr("Wrong sound mode!\r\n");
					}
				}
				break;

			case (SETLIGHT):
				sendStr("Light modes examples.\r\n");
				light(0);
				delay(1000);
				sendStr("Select light mode. [1/2/3]\r\n");
				recieveStr();
				if(isNumber()){
					lightMode = atoi(buffer);
					if(0 < lightMode < 4){
						sendStr("Light mode selected!\r\n");
						currentState = SETREPEATS;
					}
					else{
						sendStr("Wrong light mode!\r\n");
					}
				}
				break;

			case (SETREPEATS):
				sendStr("Select number of repeats for your alarm. [0-10]\r\n");
				recieveStr();
				if(isNumber()){
					numOfRepeats = atoi(buffer);
					if(0 <= numOfRepeats <= 10){
						sendStr("Number of repeats set!\r\n");
						if(numOfRepeats == 0) currentState = SETTIMES;
						else currentState = SETTIMEBETWEEN;
					}
					else{
						sendStr("Wrong number of repeats!\r\n");
					}
				}
				break;

			case (SETTIMEBETWEEN):
				sendStr("Select time in minutes between your alarms. [1-30]\r\n");
				recieveStr();
				if(isNumber()){
					timeBetweenRepeats = atoi(buffer);
					if(1 <= timeBetweenRepeats <= 30){
						sendStr("Time between alarms set!\r\n");
						currentState = SETTIMES;
					}
					else{
						sendStr("Wrong time between alarms!\r\n");
					}
				}
				break;

			case (SETTIMES):
				sendStr("Enter current time in HH:MM format.\r\n");
				recieveStr();
				if(!setTime(&currentTime)){
					break;
				}

				sendStr("Enter alarm time in HH:MM format.\r\n");
				recieveStr();
				if(!setTime(&alarmTime)){
					break;
				}

				setAlarm(&currentTime, &alarmTime);
				sendStr("Alarm has started!\r\n");
				currentState = ALARM;
				break;

			case (ALARM):
				sendStr("Action? [STOP/NEW]\r\n");
				recieveStr();
				if(!strcmp(buffer, "NEW")){
					RTC_TAR = 0;
					currentState = USEDEFAULT;
				}
				else if(!strcmp(buffer, "STOP")){
					RTC_TAR = 0;
					currentState = ENDALARM;
				}
				break;

			case (ENDALARM):
				//never leave main
				break;

			default:
				break;
    	}
    }
    return 0;
}
