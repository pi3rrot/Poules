
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
DS3231  rtc(10, 11);

int interruptPin = 18;

int sens_ouverture = 2; //etat indeterminé

struct Date_t {
	unsigned long annee;
	unsigned long mois;
	unsigned long jour;
};


/*
 * Variable globales pour interruptions
 */
void ouvrir()
{
	sens_ouverture = 1;
}

void fermer() {
	sens_ouverture = 0;
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
	//setRTC();
}

void setRTC() {
	rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
	rtc.setTime(21, 22, 20);     // Set the time to 12:00:00 (24hr format)
	rtc.setDate(22, 4, 2019);   // Set the date to January 1st, 2014 
}

void loop() {
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
	 * On créé un timestamp en minute pour pouvoir se positionner dans le timeline
	 * 3 cas de figure :
	 * - Avant ouverture
	 * - Apres ouverture et avant fermeture
	 * - Apres fermeture
	 */

	float timestamp_time, timestamp_cal_matin, timestamp_cal_soir, timestamp_minuit, timestamp_2359;

	for (int i=0 ; i <  sizeof(DateSol_t) / sizeof(DateSol_t[0]) ; i++) {
		if (t.year == DateSol_t[i][0]) {
			if (t.mon == DateSol_t[i][1]) {
				if (t.date == DateSol_t[i][2]) {

					timestamp_time = ((float)i * 24 * 60) + ((float)t.hour * 60) + (float)t.min;
					timestamp_cal_matin = ((float)i * 24 * 60) + ((float)DateSol_t[i][3] * 60) + (float)DateSol_t[i][4];
					timestamp_cal_soir = ((float)i * 24 * 60) + ((float)DateSol_t[i][5] * 60) + (float)DateSol_t[i][6];
					timestamp_minuit = ((float)i * 24 * 60);
					timestamp_2359 = ((float)i * 24 * 60) + (24 * 60) -1;


					Serial.print("timestamp_time :");
					Serial.println(timestamp_time);
					Serial.println();

					Serial.print("timestamp_minuit :");
					Serial.println(timestamp_minuit);
					Serial.print("timestamp_cal_matin :");
					Serial.println(timestamp_cal_matin);
					Serial.print("timestamp_cal_soir :");
					Serial.println(timestamp_cal_soir);
					Serial.print("timestamp_2359 :");
					Serial.println(timestamp_2359);
					Serial.println();


					if ( (timestamp_time > timestamp_minuit) && (timestamp_time < timestamp_cal_matin) ) {
					// Cas1
						Serial.println("=> Day is comming... CAS1");
						Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i][3] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][4] );

					        rtc.setAlarm1Time(DateSol_t[i][3], DateSol_t[i][4]);
					        rtc.setControl();
					        rtc.resetAlarm();
			
        	                                // Attachement d'une interruption sur front descendant de INT0
                	                        attachInterrupt(INT5, ouvrir, FALLING);
					}
					
					// Cas2
					else if ( (timestamp_time > timestamp_cal_matin) && (timestamp_time < timestamp_cal_soir) ) {
						Serial.println("=> Night is comming, CAS2");
                                                Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i][5] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][6] );

                                                rtc.setAlarm1Time(DateSol_t[i][5], DateSol_t[i][6]);
                                                rtc.setControl();
                                                rtc.resetAlarm();

                                                // Attachement d'une interruption sur front descendant de INT0
                                                attachInterrupt(INT5, fermer, FALLING);
					}

 					//Cas3
					else if ( (timestamp_time > timestamp_cal_soir) && (timestamp_time < timestamp_2359) ) {
						Serial.println("=> Day is comming tomorrow, CAS3");
						Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i+1][3] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i+1][4] );

					        //rtc.setAlarm1Time(DateSol_t[i+1][3], DateSol_t[i+1][4]);
					        rtc.setAlarm1Time(t.hour, t.min+1);
					        rtc.setControl();
					        rtc.resetAlarm();
			
        	                                // Attachement d'une interruption sur front descendant de INT0
                	                        attachInterrupt(INT5, ouvrir, FALLING);
					}
					
					else {
						Serial.print("=> Waiting ");
						Serial.print(61 - t.sec);
						Serial.println(" seconds for minute change, and reset.");
						delay((61 - t.sec) * 1000);
						asm volatile ("  jmp 0");
					}
				}
			}				
		}

	}

	/*
	 * Slepping mode et attente du réveil de l'intérruption.
	 * TODO
	 */

	delay(1000);

	// Configuration du type de sleep
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	// Activation du mode sleep
	sleep_mode();
    
 
	// CPU en mode sleep,
	// Attente de l'interruption INT5 qui réveillera le CPU
     
	// Désactivation du mode sleep
	sleep_disable();
	     
	if (sens_ouverture == 1) {
	        Serial.println("-=-=-=-=- CA OUVRE !!!!!! -=-=-=-=-=-=-");
		ouvrirPorte(1);
	}

	if (sens_ouverture == 0) {
	        Serial.println("-=-=-=-=- CA FERME !!!!!! -=-=-=-=-=-=-");
		ouvrirPorte(0);
	}

	sens_ouverture = 2;

	/*
	Serial.print(" ");

	// Send date
	Serial.print(rtc.getDateStr());
	Serial.print(" -- ");

	// Send time
	Serial.println(rtc.getTimeStr());

	Serial.print("Alarm 1 : ");
	Serial.println(rtc.getAlarm1Str());
}
