
// Lib AVR pour sleep_mode...
#include <avr/power.h>
#include <avr/sleep.h>

#include <Stepper.h>
#include <DS3231.h>

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

int interruptPin = 2;

bool sens_ouverture;

struct Date_t {
	unsigned long annee;
	unsigned long mois;
	unsigned long jour;
};


void ouvrir()
{
	delay(1000);
	Serial.println("-=-=-=-=- CA OUVRE !!!!!! -=-=-=-=-=-=-");
}

void fermer() {
        delay(1000);
        Serial.println("-=-=-=-=- CA FERME !!!!!! -=-=-=-=-=-=-");
}






/********************************
 * Fonction d'accès à la porte 
 * val = 0, fermer la porte
 * val = 1, ouvrir la porte
 */
void ouvrirPorte(bool val) {
        myStepper1.setSpeed(100);
        myStepper2.setSpeed(100);

        if (val == false) {
                for(int i=0; i<=STEP_REV*REV; i++) {
                        myStepper1.step(1);
                        myStepper2.step(-1);
                }
        }

        if (val == true) {
                for(int i=0; i<=STEP_REV*REV; i++) {
                        myStepper1.step(-1);
                        myStepper2.step(1);
                }
        }

        // Stepper OFF
        for(int i=2; i<=9; i++) {
                digitalWrite(i, LOW);
        }


}


void clearSerial() {
        Serial.begin(115200);
        Serial.write(27);       // ESC command
        Serial.print("[2J");    // clear screen command
        Serial.write(27);
        Serial.print("[H");     // cursor to home command
}

void setup() {
	
	/*
	 * On prépare le terrain pour le PIN qui nécéssite une resistance de PullUp
	 * L'interruption est déclarée dans le loop()
	 */
	pinMode(interruptPin, INPUT_PULLUP);

	rtc.begin();  
	
	/*
	 * The following lines can be uncommented to set the date and time
	 */

	/*
	  rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
	  rtc.setTime(20, 18, 0);     // Set the time to 12:00:00 (24hr format)
	  rtc.setDate(17, 4, 2019);   // Set the date to January 1st, 2014 
	*/

}



void loop() {
	// Send Day-of-Week
	//	Serial.print(rtc.getDOWStr());
	clearSerial();
        Serial.println("-=-=-=-=-=-=- INITIALIZATION -=-=-=-=-=-=-=-");


	Time t;
	t = rtc.getTime();

        int myval;
        myval = sizeof(DateSol_t) / sizeof(DateSol_t[0]);
	Serial.println();
	Serial.print("=> Taille DateSol_t : ");
        Serial.println( myval  );

	Serial.println();
        Serial.print("=> Date : ");
	Serial.println(rtc.getDateStr());
	Serial.print("=> Heure : ");
	Serial.println(rtc.getTimeStr());
	Serial.println();



	/*
	 * Attention on met toujours + 1 minute par rapport à l'heure dans le registre
	 * a changer pour prod !
	 */
	for (int i=0 ; i <  sizeof(DateSol_t) / sizeof(DateSol_t[0]) ; i++) {
		if (t.year == DateSol_t[i][0]) {
			if (t.mon == DateSol_t[i][1]) {
				if (t.date == DateSol_t[i][2]) {
					if (t.hour > DateSol_t[i][3] && t.hour < DateSol_t[i][5] ){
						Serial.println("=> Night (brrrr) is comming...");
						Serial.print("=> Setting Alarm1 register @ ");
                                                Serial.print( DateSol_t[i][5] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][6], 2 );

					        rtc.setAlarm1Time(t.hour, (t.min+1));
					        rtc.setControl();
					        rtc.resetAlarm();
				
                                                // Attachement d'une interruption sur front descendant de INT0
                                                attachInterrupt(INT0, fermer, FALLING);
					}
					if (t.hour < DateSol_t[i][3]) {
						Serial.println("=> Day is comming...");
                                                Serial.print("=> Setting Alarm1 register @ ");
                                                Serial.print( DateSol_t[i][3] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][4], 2 );

                                                rtc.setAlarm1Time(t.hour, (t.min+1));
                                                rtc.setControl();
                                                rtc.resetAlarm();
	
					        // Attachement d'une interruption sur front descendant de INT0
					        attachInterrupt(INT0, ouvrir, FALLING);
					}				
				}
			}				
		}

	}

	/*
	 * Slepping mode et attente du réveil de l'intérruption.
	 */

	// Configuration du type de sleep
//	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//	sleep_enable();
	// Activation du mode sleep
//	sleep_mode();
    
//attachInterrupt(INT0, fermer, LOW);

 
	// CPU en mode sleep,
	// Attente de l'interruption INT0 qui réveillera le CPU
     
	// Désactivation du mode sleep
//	sleep_disable();
     





	/*
	Serial.print(" ");

	// Send date
	Serial.print(rtc.getDateStr());
	Serial.print(" -- ");

	// Send time
	Serial.println(rtc.getTimeStr());

	
	Serial.print("Alarm 1 : ");
	Serial.println(rtc.getAlarm1Str());
	
	
	// Wait one second before repeating :) */
	delay (5000);
	clearSerial();
}
