#include <Arduino.h>

// Lib AVR pour sleep_mode...
#include <avr/power.h>
#include <avr/sleep.h>

#include <DS3231.h>

#include "../lib/calsol.h"

uint8_t SHIFT_NIGHT = 45;
uint8_t SHIFT_DAY = 30;

// Module DS3231 pour l'heure
int gnd_rtc = 14;
int vcc_rtc = 15;
int vcc_interr = 53;
DS3231  rtc(16, 17);
int sqw_rtc = 18;

int motor_power = 31;
int pont_h_1 = 29;
int pont_h_2 = 27;

int fin_course_ouverture = 25;
int fin_course_fermeture = 49;


int sens_ouverture = 2; //etat indeterminé

struct Date_t {
	unsigned long annee;
	unsigned long mois;
	unsigned long jour;
};


void setdate() {
	Serial.println("Set Date format : 05/06/19 for example");
	char buffer[] = {' ',' ',' ',' ',' ',' ',' ', ' '};
	while (!Serial.available()); // Wait for characters
 		Serial.readBytesUntil('\n', buffer, 8);

		int incomingValue = atoi(buffer);
		Serial.println(incomingValue);

		rtc.setDate(atoi(buffer[0])*10+atoi(buffer[1]),
								atoi(buffer[3])*10+atoi(buffer[4]),
								atoi(buffer[6])*10+atoi(buffer[7]) );   // Set the date to January 1st, 2014

}

void read_time() {
	Time t;
	t = rtc.getTime();

	Serial.println();
	Serial.print("=> Date : ");
	Serial.println(rtc.getDateStr());
	Serial.print("=> Heure : ");
	Serial.println(rtc.getTimeStr());
	Serial.println();

}


/*
 * Variable globales pour interruptions
 */
void ouvrir() {
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
	//on ouvre
	if (val == true) {
		// On set le pont en H +12V 0V
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

	//on ferme
	if (val == false) {
		// On set le pont en H pour 0V -12V
		digitalWrite(pont_h_1, LOW);
		digitalWrite(pont_h_2, HIGH);
		delay(100);
		// On envoi la purée
		digitalWrite(motor_power, LOW);
		// On attend le pin qui va bien
		while (digitalRead(fin_course_fermeture) != LOW) {
			delayMicroseconds(10);
		}
	}

	// On coupe tout les relais
	digitalWrite(pont_h_1, HIGH);
	digitalWrite(pont_h_2, HIGH);
	digitalWrite(motor_power, HIGH);

}


void clearSerial() {
	Serial.begin(115200);
	Serial.write(27);       // ESC command
	Serial.print("[2J");    // clear screen command
	Serial.write(27);
	Serial.print("[H");     // cursor to home command
}

void setRTC() {
	rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
	rtc.setTime(20, 43, 0);     // Set the time to 12:00:00 (24hr format)
	rtc.setDate(13, 1, 2020);   // Set the date to January 1st, 2014
}


void selftest_func(void) {

	bool read;
	Serial.println("Self-Test Menu");
	Serial.println("--------------------");
	Serial.println("1. Read Time");
	Serial.println("2. Switch all relays 2 times each");
	Serial.println("3. Read fin_course_*");
	Serial.println("4. Detect position and switch position");
	Serial.println("5. Reset Arduino");
	Serial.println("6. setRTC()");
	Serial.println("o. open door");
	Serial.println("c. close door");
	Serial.println("+. open door for 100ms");
	Serial.println("-. close dor for 100ms");
	Serial.println("a. open/close forever");
	Serial.println("--------------------");
	Serial.println("Type the number and press enter");
	delay(500);
	Serial.flush(); //flush all previous received and transmitted data
	while(!Serial.available()) ;
	char ch;
	ch = Serial.read();
	switch(ch) {
		case '1':
		 	Serial.println("Read Time");
			read_time();
			selftest_func();
			break;

		case '2':
			Serial.println("Switch pont_h_1...");
			digitalWrite(pont_h_1, LOW);
			delay(500);
			digitalWrite(pont_h_2, HIGH);
			delay(500);

			Serial.println("Switch pont_h_2...");
			digitalWrite(pont_h_1, LOW);
			delay(500);
			digitalWrite(pont_h_2, HIGH);
			delay(500);

			Serial.println("Switch motor_power...");
			digitalWrite(motor_power, LOW);
			delay(500);
			digitalWrite(motor_power, HIGH);
			delay(500);

			selftest_func();
			break;

		case '3':
		/*
		 * On lit les interrupteurs de fin de course
		 * enclenché = 1
		 * non enclenché = 0
		 */
			read = digitalRead(fin_course_ouverture);
			if (read == 1) {
				Serial.println("=> fin_course_ouverture enclenché !\n");
				Serial.println(read);
			}
			else {
				Serial.println("=> fin_course_ouverture non enclenché.\n");
				Serial.println(read);
			}
			delay(50);
			read = digitalRead(fin_course_fermeture);
			if (read == 0) {
				Serial.println("=> fin_course_fermeture enclenché !\n");
				Serial.println(read);
			}
			else {
				Serial.println("=> fin_course_fermeture non enclenché.\n");
				Serial.println(read);
			}
			selftest_func();
			break;

		case '4':
			read = digitalRead(fin_course_ouverture);
			if (read == HIGH) {
				Serial.println("=> Porte ouverte, on ferme");
				ouvrirPorte(0);
			}
			else {
				Serial.println("=> Porte fermée, on ouvre");
				ouvrirPorte(1);
			}
			selftest_func();
			break;

		case '5':
				asm volatile ("  jmp 0");

		case '6':
				setRTC();
				//selftest_func();
				break;

		case 'o':
			Serial.println("=> On ouvre !");
			ouvrirPorte(1);
			selftest_func();
			break;

		case 'c':
			Serial.println("=> On ferme !");
			ouvrirPorte(0);
			selftest_func();		Serial.println("TROP HAUT !");
			break;

		case '+':
			if (digitalRead(fin_course_ouverture) == LOW ) {
				// On set le pont en H pour 0V -12V
				digitalWrite(pont_h_1, HIGH);
				digitalWrite(pont_h_2, LOW);
				delay(100);
				// On envoi la purée
				digitalWrite(motor_power, LOW);
				delay(100);
				digitalWrite(motor_power, HIGH);
				digitalWrite(pont_h_1, HIGH);
				digitalWrite(pont_h_2, HIGH);

				Serial.println("On monte !");
				selftest_func();
				break;
			}
			Serial.println("TROP HAUT !");
			selftest_func();
			break;

		case '-':
			// On check l'état des relais
				if (digitalRead(fin_course_fermeture) == HIGH	) {
					// On set le pont en H +12V 0V
					digitalWrite(pont_h_1, LOW);
					digitalWrite(pont_h_2, HIGH);
					delay(100);
					// On envoi la purée
					digitalWrite(motor_power, LOW);
					delay(100);
					digitalWrite(motor_power, HIGH);
					digitalWrite(pont_h_1, HIGH);
					digitalWrite(pont_h_2, HIGH);
					Serial.println("On baisse !");
					selftest_func();
					break;
		}
		Serial.println("TROP BAS !");
		selftest_func();
		break;

		case 'a':
			uint16_t i = 0;
			while(true) {
				ouvrirPorte(1);
				ouvrirPorte(0);
				Serial.println(i);
				i++;
			}
			selftest_func();

			default:
				selftest_func();

	}
}








void setup() {
	// On dit que c'est des sorties
	pinMode(motor_power, OUTPUT);
	pinMode(pont_h_1, OUTPUT);
	pinMode(pont_h_2, OUTPUT);
	pinMode(fin_course_ouverture, INPUT_PULLUP);
	pinMode(vcc_interr, OUTPUT);

	// On coupe le relay qui envoi la purée
	digitalWrite(motor_power, HIGH);
	digitalWrite(vcc_interr, HIGH);

	// On fait 2 cycles pour entendre que ca initialise.
	for(int i=0; i<2; i++) {
		digitalWrite(pont_h_1, LOW);
		digitalWrite(pont_h_2, HIGH);
		delay(100);
		digitalWrite(pont_h_1, HIGH);
		digitalWrite(pont_h_2, LOW);
		delay(100);
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
}



void loop() {

	bool read, read2;
	read = digitalRead(fin_course_ouverture);
	read2 = digitalRead(fin_course_fermeture);
	clearSerial();
	Serial.println("-=-=-=-=-=-=- INITIALIZATION -=-=-=-=-=-=-=-");
	Serial.println("Press 's' to enter self-test mode in 5 senconds");
	delay(5000);

	// Test pour une fonction self-test
	char ch = 0;
	ch = Serial.read();
	switch(ch){
		case 's':
			selftest_func();
	}

	Time t;
	t = rtc.getTime();

	int myval;
	myval = sizeof(DateSol_t) / sizeof(DateSol_t[0]);
	Serial.println();
	Serial.print("=> Taille DateSol_t : ");
	Serial.println( myval );

	Serial.println();
	Serial.print("=> Date : ");
	Serial.println(rtc.getDateStr());
	Serial.print("=> Heure : ");
	Serial.println(rtc.getTimeStr());
	Serial.println();

	/*
	 * On créé un timestamp en minute pour pouvoir se positionner dans le timeline
	 * 3 cas de figure :
	 * - Avant ouverture (CAS 1)
	 * - Apres ouverture et avant fermeture (CAS 2)
	 * - Apres fermeture (CAS 3) attention, taper dans j+1
	 */

	float timestamp_time, timestamp_cal_matin, timestamp_cal_soir, timestamp_minuit, timestamp_2359;

	for (int i=0 ; i <  sizeof(DateSol_t) / sizeof(DateSol_t[0]) ; i++) {
		if (t.year == DateSol_t[i][0] + 2000) {
			if (t.mon == DateSol_t[i][1]) {
				if (t.date == DateSol_t[i][2]) {

					timestamp_time = ((float)i * 24 * 60) + ((float)t.hour * 60) + (float)t.min;
					timestamp_cal_matin = ((float)i * 24 * 60) + ((float)DateSol_t[i][3] * 60) + (float)DateSol_t[i][4] - (float)SHIFT_DAY;
					timestamp_cal_soir = ((float)i * 24 * 60) + ((float)DateSol_t[i][5] * 60) + (float)DateSol_t[i][6] + (float)SHIFT_NIGHT;
					timestamp_minuit = ((float)i * 24 * 60);
					timestamp_2359 = ((float)i * 24 * 60) + (24 * 60) -1;

					Serial.print("timestamp_time :      ");
					Serial.println(timestamp_time);
					Serial.println();

					Serial.print("timestamp_minuit :    ");
					Serial.println(timestamp_minuit);
					Serial.print("timestamp_cal_matin : ");
					Serial.println(timestamp_cal_matin);
					Serial.print("timestamp_cal_soir :  ");
					Serial.println(timestamp_cal_soir);
					Serial.print("timestamp_2359 :      ");
					Serial.println(timestamp_2359);
					Serial.println();


					if ( (timestamp_time > timestamp_minuit) && (timestamp_time < timestamp_cal_matin) ) {
						// Cas1
						Serial.println("=> Day is comming... CAS1");

						Serial.print("=> On tape dans J normal car après minuit ");
						Serial.print(DateSol_t[i][2]);
						Serial.print("/");
						Serial.print(DateSol_t[i][1]);
						Serial.print("/");
						Serial.print(DateSol_t[i][0]);
						Serial.println();

						Serial.print("=> CalSol without SHIFT_DAY ");
						Serial.print( DateSol_t[i][3] );
						Serial.print(":");
						Serial.print( DateSol_t[i][4]);
						Serial.println();


						Serial.print("=> Setting Alarm1 registers @ ");

						//car shift, on soustrait le shift au timestamp_cal_matin
						if (DateSol_t[i][4]-SHIFT_DAY < 0) {
							Serial.print( DateSol_t[i][3] - 1 );
							Serial.print(" : ");
							Serial.print( (DateSol_t[i][4]-SHIFT_DAY)+60 );
							rtc.setAlarm1Time( DateSol_t[i][3]-1, (DateSol_t[i][4]-SHIFT_DAY)+60 );
						}
						else {
							Serial.print( DateSol_t[i][3] );
							Serial.print(" : ");
							Serial.print( DateSol_t[i][4] - SHIFT_DAY);
							rtc.setAlarm1Time(DateSol_t[i][3], DateSol_t[i][4]-SHIFT_DAY);
						}

						Serial.print(" with ");
						Serial.print(SHIFT_DAY);
						Serial.println("minutes less.");

						rtc.setControl();
						rtc.resetAlarm();

						if((read == LOW && read2 == LOW) || (read == LOW && read2 == HIGH)) {
							ouvrirPorte(0);
						}

						// Attachement d'une interruption sur front descendant de INT0
						attachInterrupt(INT5, ouvrir, FALLING);
					}


					// Cas2
					else if ( (timestamp_time > timestamp_cal_matin) && (timestamp_time < timestamp_cal_soir) ) {
						Serial.println("=> Night is comming, CAS2");
						Serial.print("=> Setting Alarm1 registers @ ");


						//car shift, on additionne le shift au timestamp_cal_soir
						if (DateSol_t[i][6]+SHIFT_NIGHT >= 60) {
							Serial.print( DateSol_t[i][5] + 1 );
							Serial.print(":");
							Serial.print( (DateSol_t[i][6]+SHIFT_NIGHT)-60 );
							rtc.setAlarm1Time(DateSol_t[i][5]+1, (DateSol_t[i][6]+SHIFT_NIGHT)-60);
						}
						else {
							Serial.print( DateSol_t[i][5] );
							Serial.print(":");
							Serial.print( DateSol_t[i][6] + SHIFT_NIGHT);
							rtc.setAlarm1Time(DateSol_t[i][5], DateSol_t[i][6]+SHIFT_NIGHT);
							//rtc.setAlarm1Time(t.hour, t.min+1);
						}
						Serial.print(" with ");
						Serial.print(SHIFT_NIGHT);
						Serial.println("minutes more.");

						rtc.setControl();
						rtc.resetAlarm();

						//Si porte ouverte ou indeterminée
						if ((read == HIGH && read2 == HIGH) || (read == LOW && read2 == HIGH)) {
							ouvrirPorte(1);
						}

						// Attachement d'une interruption sur front descendant de INT0
						attachInterrupt(INT5, fermer, FALLING);
					}

 					//Cas3
					else if ( (timestamp_time > timestamp_cal_soir) && (timestamp_time < timestamp_2359) ) {
						Serial.println("=> Day is comming tomorrow, CAS3");
						Serial.print("=> On tape dans J+1 ");
						Serial.print(DateSol_t[i+1][2]);
						Serial.print("/");
						Serial.print(DateSol_t[i+1][1]);
						Serial.print("/");
						Serial.print(DateSol_t[i+1][0]);
						Serial.println();

						Serial.print("=> CalSol without SHIFT_DAY ");
						Serial.print( DateSol_t[i+1][3] );
						Serial.print(":");
						Serial.print( DateSol_t[i+1][4]);
						Serial.println();

						Serial.print("=> Setting Alarm1 registers @ ");

						//car shift, on soustrait le shift au timestamp_cal_matin
						if (DateSol_t[i+1][4]-SHIFT_DAY < 0) {
							Serial.print( DateSol_t[i+1][3] - 1 );
							Serial.print(":");
							Serial.print( (DateSol_t[i+1][4]-SHIFT_DAY)+60 );
							rtc.setAlarm1Time( DateSol_t[i+1][3]-1, (DateSol_t[i+1][4]-SHIFT_DAY)+60 );
						}
						else {
							Serial.print( DateSol_t[i+1][3] );
							Serial.print(":");
							Serial.print( DateSol_t[i+1][4] - SHIFT_DAY);
							rtc.setAlarm1Time(DateSol_t[i+1][3], DateSol_t[i+1][4]-SHIFT_DAY);
							//rtc.setAlarm1Time(t.hour, t.min+1);
						}

						Serial.print(" with ");
						Serial.print(SHIFT_DAY);
						Serial.println("minutes less.");

						rtc.setControl();
						rtc.resetAlarm();

						if((read == HIGH && read2 == HIGH) || (read == LOW && read2 == HIGH)) {
							ouvrirPorte(0);
						}

						// Attachement d'une interruption sur front descendant de INT0
						attachInterrupt(INT5, ouvrir, FALLING);
					}

					else {
						Serial.print("=> Waiting 5 seconds and reset, wait for minute change");
						delay(2000);
						asm volatile ("  jmp 0");
					}
				}
			}
		}

	}

	/*
	 * Slepping mode et attente du réveil de l'intérruption.
	 */

	Serial.println("=> Going to sleep... see you... zZzzZzzzZzz");
	delay(100);

	// Configuration du type de sleep
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	// Activation du mode sleep
	sleep_mode();
	// CPU en mode sleep,
	// Attente de l'interruption INT5 qui réveillera le CPU

	// Désactivation du mode sleep avec delais pour avoir le temps de se réveiller.
	sleep_disable();
	delay(100);

	if (sens_ouverture == 1) {
		Serial.println("-=-=-=-=-=- CA OUVRE ! -=-=-=-=-=-=-");
		ouvrirPorte(1);
		delay(5000);
	}

	if (sens_ouverture == 0) {
	  Serial.println("-=-=-=-=-=- CA FERME ! -=-=-=-=-=-=-");
		ouvrirPorte(0);
		delay(5000);
	}
	sens_ouverture = 2;
}
