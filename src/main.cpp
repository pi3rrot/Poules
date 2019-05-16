#include <Arduino.h>

// Lib AVR pour sleep_mode...
#include <avr/power.h>
#include <avr/sleep.h>

#include <DS3231.h>

#include "../lib/calsol.h"

// Module DS3231 pour l'heure
int gnd_rtc = 14;
int vcc_rtc = 15;
DS3231  rtc(16, 17);
int sqw_rtc = 18;

int motor_power = 31;
int pont_h_1 = 22;
int pont_h_2 = 23;

int fin_course_ouverture = 53;

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






/*
 * Fonction d'accès à la porte
 * val = 0, fermer la porte
 * val = 1, ouvrir la porte
 */
void ouvrirPorte(bool val) {
	if (val == false) {
	        // On set le pont en H +12V 0V
					digitalWrite(pont_h_1, LOW);
					digitalWrite(pont_h_2, HIGH);
					delay(100);
					// On envoi la purée
					digitalWrite(motor_power, LOW);

					// delai?
					delay(3000);
	}

	if (val == true) {
	        // On set le pont en H pour 0V -12V
					digitalWrite(pont_h_1, HIGH);
					digitalWrite(pont_h_2, LOW);
					delay(100);
					// On envoi la purée
					digitalWrite(motor_power, LOW);
				// On attend le pin qui va bien
				while (digitalRead(fin_course_ouverture) != HIGH) {
					delayMicroseconds(10);
				}
	}

	digitalWrite(motor_power, HIGH);

}


void clearSerial() {
        Serial.begin(115200);
        Serial.write(27);       // ESC command
        Serial.print("[2J");    // clear screen command
        Serial.write(27);
        Serial.print("[H");     // cursor to home command
}

void setup() {
	// On dit que c'est des sorties
	pinMode(motor_power, OUTPUT);
	pinMode(pont_h_1, OUTPUT);
	pinMode(pont_h_2, OUTPUT);
	pinMode(fin_course_ouverture, OUTPUT);



	// On coupe le relay qui envoi la purée
	digitalWrite(motor_power, HIGH);

	// On fait 2 cycles pour entendre que ca initialise.
	for(int i=0; i<2; i++) {
		digitalWrite(pont_h_1, LOW);
		digitalWrite(pont_h_2, HIGH);
		delay(200);
		digitalWrite(pont_h_1, HIGH);
		digitalWrite(pont_h_2, LOW);
		delay(200);
	}

	/*
	 * On prépare le terrain pour le PIN qui nécéssite une resistance de PullUp
	 * L'interruption est déclarée dans le loop()
	 */
	pinMode(sqw_rtc, INPUT_PULLUP);
	pinMode(vcc_rtc, OUTPUT);
	pinMode(gnd_rtc, OUTPUT);
	digitalWrite(vcc_rtc, HIGH);
	digitalWrite(gnd_rtc, LOW);

	rtc.begin();

	/*
	 * The following lines can be uncommented to set the date and time
	 */
	//setRTC();
}

void setRTC() {
	rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
	rtc.setTime(21, 21, 00);     // Set the time to 12:00:00 (24hr format)
	rtc.setDate(15, 5, 2019);   // Set the date to January 1st, 2014
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
		if (t.year == DateSol_t[i][0] + 2000) {
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

					        //rtc.setAlarm1Time(DateSol_t[i][3], DateSol_t[i][4]);
									rtc.setAlarm1Time(t.hour, t.min+1);

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

                                                //rtc.setAlarm1Time(DateSol_t[i][5], DateSol_t[i][6]);
																								rtc.setAlarm1Time(t.hour, t.min+1);


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
		delay(500);
		Serial.println("-=-=-=-=- CA OUVRE ! -=-=-=-=-=-=-");
		ouvrirPorte(1);
		delay(5000);
	}

	if (sens_ouverture == 0) {
		delay(500);
	  Serial.println("-=-=-=-=- CA FERME ! -=-=-=-=-=-=-");
		//ouvrirPorte(0);
		delay(5000);
	}

	sens_ouverture = 2;

}
