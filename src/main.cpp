#include <Arduino.h>

// Lib AVR pour sleep_mode...
#include <avr/power.h>
#include <avr/sleep.h>

#include <DS3231.h>

#include "../lib/calsol.h"

uint8_t SHIFT_NIGHT=45;
uint8_t SHIFT_DAY=30;

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


void settime() {
	Serial.println("Set time format : 01:34 for example");
	char buffer[] = {' ',' ',' ',' ',' '};
	while (!Serial.available()); // Wait for characters
 		Serial.readBytesUntil('\n', buffer, 5);

		int incomingValue = atoi(buffer);
		Serial.println(incomingValue);

		rtc.setTime(atoi(buffer[0])*10+buffer[1],
								atoi(buffer[3])*10+atoi(buffer[4]),
								00);

}

/*
void setRTC() {
	rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
	rtc.setTime(25, 21, 00);     // Set the time to 12:00:00 (24hr format)
	rtc.setDate(25, 6, 2019);   // Set the date to January 1st, 2014
}
*/





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
	//on ferme
	if (val == false) {
		// On set le pont en H +12V 0V
		digitalWrite(pont_h_1, LOW);
		digitalWrite(pont_h_2, HIGH);
		delay(100);
		// On envoi la purée
		digitalWrite(motor_power, LOW);

		// delai?
		delay(2000);
	}

	//on ouvre
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



void selftest_func(void) {

	bool read;

	Serial.println("Self-Test Menu");
	Serial.println("--------------------");
	Serial.println("1. Read Time");
	Serial.println("2. Switch all relays 2 times");
	Serial.println("3. Read fin_course_ouverture");
	Serial.println("4. Detect position and switch position");
	Serial.println("5. Reset Arduino");
	Serial.println("6. Set Date");
	Serial.println("7. Set time");
	Serial.println("o. open door");
	Serial.println("c. close door");
	Serial.println("+. open door for 100ms");
	Serial.println("-. close dor for 100ms");
	Serial.println("a. open/close forever");
	Serial.println("--------------------");
	Serial.println("Type the number and press enter");

	Serial.flush(); //flush all previous received and transmitted data
	while(!Serial.available()) ;
	char ch;
	ch = Serial.read();
	switch(ch)
	{
	 case '1':
	 	Serial.println("Read Time");
		selftest_func();

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

	case '3':
		read = digitalRead(fin_course_ouverture);
		if (read == 1) {
			Serial.println("=> fin_course_ouverture enclenché !\n");
		}
		else {
			Serial.println("=> fin_course_ouverture non enclenché.\n");

		}
		selftest_func();

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


	case '5':
			asm volatile ("  jmp 0");

	case '6':
			setdate();

	case '7':
			settime();

	case 'o':
		ouvrirPorte(1);


	case 'c':
		ouvrirPorte(0);




	case '+':
		// On set le pont en H pour 0V -12V
		digitalWrite(pont_h_1, HIGH);
		digitalWrite(pont_h_2, LOW);
		delay(100);
		// On envoi la purée
		digitalWrite(motor_power, LOW);
		delay(200);
		digitalWrite(motor_power, HIGH);
		digitalWrite(pont_h_1, HIGH);
		digitalWrite(pont_h_2, HIGH);
		selftest_func();


	case '-':
		// On set le pont en H +12V 0V
		digitalWrite(pont_h_1, LOW);
		digitalWrite(pont_h_2, HIGH);
		delay(100);
		// On envoi la purée
		digitalWrite(motor_power, LOW);
		delay(250);
		digitalWrite(motor_power, HIGH);
		digitalWrite(pont_h_1, HIGH);
		digitalWrite(pont_h_2, HIGH);
		selftest_func();

		case 'a':
			uint16_t i = 0;
			while(true) {
				ouvrirPorte(1);
				ouvrirPorte(0);
				Serial.println(i);
				i++;
			}
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

void loop() {

	bool read;
	read = digitalRead(fin_course_ouverture);
	//	Serial.print(rtc.getDOWStr());
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
					timestamp_cal_matin = ((float)i * 24 * 60) + ((float)DateSol_t[i][3] * 60) + (float)DateSol_t[i][4] - SHIFT_DAY;
					timestamp_cal_soir = ((float)i * 24 * 60) + ((float)DateSol_t[i][5] * 60) + (float)DateSol_t[i][6] + SHIFT_NIGHT;
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

						//car shift, on soustrait le shift au timestamp_cal_matin
						if (DateSol_t[i][4]-SHIFT_DAY < 0) {
							Serial.print( DateSol_t[i][3] - 1 );
							Serial.print(" : ");
							Serial.println( (DateSol_t[i][4]-SHIFT_DAY)+60 );
							rtc.setAlarm1Time( DateSol_t[i][3]-1, (DateSol_t[i][4]-SHIFT_DAY)+60 );
						}
						else {
							Serial.print( DateSol_t[i][3] );
							Serial.print(" : ");
							Serial.println( DateSol_t[i][4] - SHIFT_DAY);
							rtc.setAlarm1Time(DateSol_t[i][3], DateSol_t[i][4]-SHIFT_DAY);
							//rtc.setAlarm1Time(t.hour, t.min+1);
						}

						rtc.setControl();
						rtc.resetAlarm();


						if(read == HIGH) {
							ouvrirPorte(0);
						}

						// Attachement d'une interruption sur front descendant de INT0
						attachInterrupt(INT5, ouvrir, FALLING);
					}


					// Cas2
					else if ( (timestamp_time > timestamp_cal_matin) && (timestamp_time < timestamp_cal_soir) ) {
						Serial.println("=> Night is comming, CAS2");
						Serial.print("=> Setting Alarm1 registers @ (45min SHIFT_NIGHT)");


						//car shift, on additionne le shift au timestamp_cal_soir
						if (DateSol_t[i][6]+SHIFT_NIGHT >= 60) {
							Serial.print( DateSol_t[i][5] + 1 );
							Serial.print(" : ");
							Serial.println( (DateSol_t[i][6]+SHIFT_NIGHT)-60 );
							rtc.setAlarm1Time(DateSol_t[i][5]+1, (DateSol_t[i][6]+SHIFT_NIGHT)-60);
						}
						else {
							Serial.print( DateSol_t[i][5] );
							Serial.print(" : ");
							Serial.println( DateSol_t[i][6] + SHIFT_NIGHT);
							rtc.setAlarm1Time(DateSol_t[i][5], DateSol_t[i][6]+SHIFT_NIGHT);
							//rtc.setAlarm1Time(t.hour, t.min+1);
						}

						rtc.setControl();
						rtc.resetAlarm();

						if(read == LOW) {
							ouvrirPorte(1);
						}

						// Attachement d'une interruption sur front descendant de INT0
						attachInterrupt(INT5, fermer, FALLING);
					}

 					//Cas3
					else if ( (timestamp_time > timestamp_cal_soir) && (timestamp_time < timestamp_2359) ) {
						Serial.println("=> Day is comming tomorrow, CAS3");
						Serial.print("=> Setting Alarm1 registers @ ");

						//car shift, on soustrait le shift au timestamp_cal_matin
						if (DateSol_t[i][4]-SHIFT_DAY < 0) {
							Serial.print( DateSol_t[i][3] - 1 );
							Serial.print(" : ");
							Serial.println( (DateSol_t[i][4]-SHIFT_DAY)+60 );
							rtc.setAlarm1Time( DateSol_t[i][3]-1, (DateSol_t[i][4]-SHIFT_DAY)+60 );
						}
						else {
							Serial.print( DateSol_t[i][3] );
							Serial.print(" : ");
							Serial.println( DateSol_t[i][4] - SHIFT_DAY);
							rtc.setAlarm1Time(DateSol_t[i][3], DateSol_t[i][4]-SHIFT_DAY);
							//rtc.setAlarm1Time(t.hour, t.min+1);
						}

						rtc.setControl();
						rtc.resetAlarm();

						Serial.println(read);

						if(read == 1) {
							ouvrirPorte(0);
							delay(100);
						}

						// Attachement d'une interruption sur front descendant de INT0
						attachInterrupt(INT5, ouvrir, FALLING);
					}

					else {
						Serial.print("=> Waiting ");
						asm volatile ("  jmp 0");
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
