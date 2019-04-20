
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
	rtc.setTime(9, 54, 0);     // Set the time to 12:00:00 (24hr format)
	rtc.setDate(20, 4, 2019);   // Set the date to January 1st, 2014 
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
	 * Attention on met toujours + 1 minute par rapport à l'heure dans le registre
	 * Changé !
	 */
	for (int i=0 ; i <  sizeof(DateSol_t) / sizeof(DateSol_t[0]) ; i++) {
		if (t.year == DateSol_t[i][0]) {
			if (t.mon == DateSol_t[i][1]) {
				if (t.date == DateSol_t[i][2]) {
					if (t.hour > DateSol_t[i][3] && t.hour < DateSol_t[i][5] ) {
						/*
						 * On test Annee, mois, jour et minute
						 * Si boot entre ouverture et fermeture =>set alarm au soir => Fermeture
						 * Si boot après fermeture mais le même jour => set alarm a j+1 => ouverture
						 * Si boot avant ouverture du matin => set alarm au matin => Ouverture
						 * 
						 */

						// Cas de figure 1
						Serial.println("=> Night (brrrr) is comming... DEV");
						Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i][5] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][6] );

					        rtc.setAlarm1Time(t.hour, (t.min + 1));
					        rtc.setControl();
					        rtc.resetAlarm();
			
        	                                // Attachement d'une interruption sur front descendant de INT0
                	                        attachInterrupt(INT0, fermer, FALLING);
					}
					
					// Cas de figure 2
					else if ( t.hour == DateSol_t[i][5] && t.min > DateSol_t[i][6] ) {
						Serial.println("=> Night is engaged, next step tomorrow morning...");
                                                Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i+1][3] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i+1][4] );

                                                rtc.setAlarm1Time(DateSol_t[i+1][3], DateSol_t[i+1][4]);
                                                rtc.setControl();
                                                rtc.resetAlarm();

                                                // Attachement d'une interruption sur front descendant de INT0
                                                attachInterrupt(INT0, ouvrir, FALLING);
					}

 					// Cas de figure 3
					else if ( t.hour == DateSol_t[i][5] && t.min < DateSol_t[i][6] ) {
						Serial.println("=> Night (brrrr) is comming...");
						Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i][5] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][6] );

					        rtc.setAlarm1Time(DateSol_t[i][5], DateSol_t[i][6]);
					        rtc.setControl();
					        rtc.resetAlarm();
			
        	                                // Attachement d'une interruption sur front descendant de INT0
                	                        attachInterrupt(INT0, fermer, FALLING);

					}

					// Cas de figure 4
					else if ( t.hour == DateSol_t[i][3] && t.min < DateSol_t[i][4] ) {
						Serial.println("=> Day is comming...");
                                                Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i][3] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][4] );

                                                rtc.setAlarm1Time(DateSol_t[i][3], DateSol_t[i][4]);
                                                rtc.setControl();
                                                rtc.resetAlarm();
	
					        // Attachement d'une interruption sur front descendant de INT0
					        attachInterrupt(INT0, ouvrir, FALLING);
					}			

					else if ( t.hour == DateSol_t[i][3] && t.min > DateSol_t[i][4] ) {
						Serial.println("=> Night (brrrr) is comming...");
						Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i][5] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][6] );

					        rtc.setAlarm1Time(DateSol_t[i][5], DateSol_t[i][6]);
					        rtc.setControl();
					        rtc.resetAlarm();
			
        	                                // Attachement d'une interruption sur front descendant de INT0
                	                        attachInterrupt(INT0, fermer, FALLING);
					}

					else if ( t.hour < DateSol_t[i][3] ) {
						Serial.println("=> Day is comming...");
                                                Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i][3] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][4] );

                                                rtc.setAlarm1Time(DateSol_t[i][3], DateSol_t[i][4]);
                                                rtc.setControl();
                                                rtc.resetAlarm();
	
					        // Attachement d'une interruption sur front descendant de INT0
					        attachInterrupt(INT0, ouvrir, FALLING);

					}

					else if ( t.hour > DateSol_t[i][3] ) {
						Serial.println("=> Night (brrrr) is comming...");
						Serial.print("=> Setting Alarm1 registers @ ");
                                                Serial.print( DateSol_t[i][5] );
                                                Serial.print(" : ");
                                                Serial.println( DateSol_t[i][6] );

					        rtc.setAlarm1Time(DateSol_t[i][5], DateSol_t[i][6]);
					        rtc.setControl();
					        rtc.resetAlarm();
			
        	                                // Attachement d'une interruption sur front descendant de INT0
                	                        attachInterrupt(INT0, fermer, FALLING);
					}
	
				}
			}				
		}

	}

	/*
	 * Slepping mode et attente du réveil de l'intérruption.
	 * TODO
	 */

	if (sens_ouverture == 1) {
	        Serial.println("-=-=-=-=- CA OUVRE !!!!!! -=-=-=-=-=-=-");
		ouvrirPorte(1);
	}

	if (sens_ouverture == 0) {
	        Serial.println("-=-=-=-=- CA FERME !!!!!! -=-=-=-=-=-=-");
		ouvrirPorte(0);
	}

	sens_ouverture = 2;

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
