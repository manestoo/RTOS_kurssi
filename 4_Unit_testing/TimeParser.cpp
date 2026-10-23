//pistetoivomus: 2p

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "TimeParser.h"

// time format: HHMMSS (6 characters)
int time_parse(char *time) {

	// how many seconds, default returns error
	int seconds;

	// TODO: Check that string is not null
	if (time == NULL){
		return TIME_VALUE_ERROR;
	} else if (strlen(time) != 6){
		return TIME_LEN_ERROR;
	}

	for (int i = 0; i < 6; i++){
		if (isdigit(time[i]) == false){
			return TIME_VALUE_TYPE_ERROR;
		}
	}

	// Parse values from time string
	// For example: 124033 -> 12hour 40min 33sec
	int values[3];
	values[2] = atoi(time+4); // seconds
	time[4] = 0;
	values[1] = atoi(time+2); // minutes
	time[2] = 0;
	values[0] = atoi(time); // hours
	// Now you have:
	// values[0] hour
	// values[1] minute
	// values[2] second

	// TODO: Add boundary check time values: below zero or above limit not allowed
	// limits are 59 for minutes, 23 for hours, etc

	//HUOM: jos syötössä olisi negatviinen arvo (eli miinus-merkki) isdigit()-funktio käsittelee tämän virheen jo
	// eli tässä alapuolella oikeastaan turhaan tarkistetaan voiko value olla <0
	if (values[0] > 23) {
		seconds = TIME_VALUE_ERROR;
	} else if (values[0] < 0) {
		seconds = TIME_VALUE_ERROR;
	} else if (values[1] > 59) {
		seconds = TIME_VALUE_ERROR;
	} else if (values[1] < 0) {
		seconds = TIME_VALUE_ERROR;
	} else if (values[2] > 59) {
		seconds = TIME_VALUE_ERROR;
	} else if (values[2] < 0) {
		seconds = TIME_VALUE_ERROR;
	} else {
		seconds = values[2] + values[1]*60 + values[0]*60*60; 
	}
	
	// TODO: Add more features to get more points
	// HUOM:  isdigit()-feature on rivillä 19 ja merkkijonon pituus -feature rivillä 15
	if (seconds == 0){
		return TIME_VALUE_ERROR; //0 ei saa toteuttaa ajastinkeskeytystä
	}
	
	
	return seconds;	//seconds antaa failure
}
