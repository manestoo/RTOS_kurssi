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
	k_sleep(K_SECONDS(1));
	// Sanity check
	// printk("Started serial led example\n");

	// UART helpers
	char c=0;
	int cnt = 0;
	char buffer[20] = "\0";

	// superloop
	while (true) {

		if (uart_poll_in(uart_dev,&c) == 0) {
			if (c == '\n' || c == '\r') {
				// skip empty lines (e.g. the \n of a \r\n line ending)
				if (cnt == 0) {
					continue;
				}
				// printk(buffer);
				// here you call parser
				int ret = time_parse(buffer);
				// check parser return value, negative value is an error
				if (ret >= 0) {
					// send signal / message to mailbox
				} 
				// clear buffer and counter
				cnt = 0;
				memset(buffer,0,20);
			}
		}
	}
	return 0;
}

int time_parse(char *time) {
	int ret = -1;

	// replace this function with your own code from test project!!
	//tänne kopsataan timeparse.cpp:stä
	
	return ret;
}