// Arduino Uno/2009:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 4 or the dedicated SDA pin
//          SCL pin   -> Arduino Digital 5 or the dedicated SCL pin


#include <Stepper.h>
#include <DS3231.h>
#include <string.h>

#include "../lib/calsol.h"

// Pour steppers
#define STEP_REV 400
#define REV 10

const int stepsPerRevolution = 400;
// 360 / 0.9deg

Stepper myStepper1(stepsPerRevolution, 6, 7, 8, 9);
Stepper myStepper2(stepsPerRevolution, 2, 3, 4, 5);

// Module DS3231 pour l'heure
DS3231  rtc(4, 5);

struct Date_t {
	unsigned long annee;
	unsigned long mois;
	unsigned long jour;
};


void stepperOff() {
	for(int i=2; i<=9; i++) {
		digitalWrite(i, LOW);  
	}
}


void setup() {
	Serial.begin(115200);
	rtc.begin();  
		rtc.setAlarm1Time(8, 20);
		rtc.setAlarm2Time(14, 45);
	// The following lines can be uncommented to set the date and time

	/*  rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
	  rtc.setTime(22, 14, 0);     // Set the time to 12:00:00 (24hr format)
	  rtc.setDate(4, 11, 2019);   // Set the date to January 1st, 2014 */

}

void loop() {
	/*myStepper2.setSpeed(100);
	for(int i=0; i<=STEP_REV*REV; i++) {
		myStepper1.step(1);
		myStepper2.step(-1);
	}
	for(int i=0; i<=STEP_REV*REV; i++) {
		myStepper1.step(-1);
		myStepper2.step(1);
	}
	stepperOff();
	delay(3000);*/

	// Send Day-of-Week
	Serial.print(rtc.getDOWStr());
	Serial.print(" ");

	// Send date
	Serial.print(rtc.getDateStr());
	Serial.print(" -- ");

	// Send time
	Serial.println(rtc.getTimeStr());

	
	Serial.print("Alarm 1 : ");
	Serial.println(rtc.getAlarm1Str());
	Serial.println("-----------------------------------------------");
	Serial.print("Alarm 2 : ");
	Serial.println(rtc.getAlarm2Str());	
	
	// Wait one second before repeating :)
	delay (1000);
}
