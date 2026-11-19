#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

// UART initialization
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

/*eli uart taskissa ennenku merkkijono lähetetään dispatcheriin, testataan merkkijono. testissä täytyy palautua 0 tai isompi (virheelliset palautukset ovat -1,-2,-3)*/

// Parser
// Add error codes, copy these from TimeParser.h
// Error codes
#define TIME_LEN_ERROR      	-1
#define TIME_ARRAY_ERROR    	-2
#define TIME_VALUE_ERROR    	-3
#define TIME_VALUE_TYPE_ERROR   -4
int time_parse(char *time);

int main(void)
{
	// UART initialization
	if (!device_is_ready(uart_dev)) {
		printk("UART initialization failed!\r\n");
		return 0;
	} 

	// Wait for everything to initialize and threads to start
	k_msleep(100);
	// Sanity check
	printk("Started serial led example\n");

	char rc=0;
	char uart_msg[20];
	int uart_msg_cnt=0;
	memset(uart_msg,0,20);			
	while (true) {
		// Ask UART if data available
		while (uart_poll_in(uart_dev,&rc) == 0) {
			// Add characters into buffer until X is received 
			if (rc != 'X') {
				uart_msg[uart_msg_cnt] = rc;
				uart_msg_cnt++;
			} else {
				// Send response to UART
				// X has been removed, so we need to add it to the end
				// printk("-1X"); example return error

				// Example check with time_parse
				//time_parse(uart_msg);
				int timer_delay = time_parse(uart_msg); // 000120 -> 80
				printk("%dX", timer_delay); //palautetaan robotille siis parsetettu lukema ja perään X

				// Clear UART message buffer
				uart_msg_cnt = 0;
				memset(uart_msg,0,20);			
			}
		}
		k_msleep(10);
	}
	return 0;
}

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