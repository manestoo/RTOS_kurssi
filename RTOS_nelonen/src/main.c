#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/timing/timing.h>
#include <stdlib.h>

#define STACKSIZE 500
#define PRIORITY 5

// UART initialization
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

// Led pin configurations
static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec blue = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

void red_led_task(void *, void *, void*);
void yellow_led_task(void *, void *, void*);
void green_led_task(void *, void *, void*);
void dispatcher_task(void*, void*, void*);
void uart_task(void*, void*, void*);
void debug_task(void*, void*, void*);
int red_time;
int yellow_time;
int green_time;
K_THREAD_DEFINE(red_thread,STACKSIZE,red_led_task,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_DEFINE(yellow_thread,STACKSIZE,yellow_led_task,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_DEFINE(green_thread,STACKSIZE,green_led_task,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_DEFINE(dis_thread,STACKSIZE,dispatcher_task,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_DEFINE(uart_thread,STACKSIZE,uart_task,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_DEFINE(debug_thread,STACKSIZE,debug_task,NULL,NULL,NULL,PRIORITY,0,0);

// Condition Variables
K_MUTEX_DEFINE(red_mutex);
K_CONDVAR_DEFINE(red_signal);
K_MUTEX_DEFINE(green_mutex);
K_CONDVAR_DEFINE(green_signal);
K_MUTEX_DEFINE(yellow_mutex);
K_CONDVAR_DEFINE(yellow_signal);
K_MUTEX_DEFINE(release_mutex);
K_CONDVAR_DEFINE(release_signal);

// Create dispatcher FIFO buffer
K_FIFO_DEFINE(dispatcher_fifo);

// Create data FIFO buffer
K_FIFO_DEFINE(data_fifo);

// FIFO dispatcher data type
struct data_t {
	/*************************
	// Add fifo_reserved below
	*************************/
	void *fifo_reserved;
	char msg[20];
    uint64_t time;
	uint64_t totalitaarinen;
};

//init UART
int init_uart(void) {
	// UART initialization
	if (!device_is_ready(uart_dev)) {
		return 1;
	} 
	return 0;
}

// Initialize leds
int init_led() {

	// Led pin initialization
	int ret = gpio_pin_configure_dt(&red, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Error: RED Led configure failed\n");		
		return ret;
	}
	// set led off
	gpio_pin_set_dt(&red,0);

	int ret_green = gpio_pin_configure_dt(&green, GPIO_OUTPUT_ACTIVE);
	if (ret_green < 0) {
		printk("Error: GREEN Led configure failed\n");		
		return ret_green;
	}
	// set led off
	gpio_pin_set_dt(&green,0);

	int ret_2 = gpio_pin_configure_dt(&blue, GPIO_OUTPUT_ACTIVE);
	if (ret_2 < 0) {
		printk("Error: BLUE Led configure failed\n");		
		return ret_2;
	}
	// set led off
	gpio_pin_set_dt(&blue,0);

	printk("Led initialized ok\n");
	
	return 0;
}

void red_led_task(void *, void *, void*) {
	
	printk("Red led thread started\n");
	while (true) {
        timing_start();
		timing_t red_start_time = timing_counter_get();

		// Wait for signal
		k_condvar_wait(&red_signal, &red_mutex, K_FOREVER);

		// 1. set led on 
		gpio_pin_set_dt(&red,1);
		printk("Red on\n");
		// 2. sleep for 2 seconds
		k_sleep(K_MSEC(red_time));
		// 3. set led off
		gpio_pin_set_dt(&red,0);
		printk("Red off\n");

        struct data_t *buf = k_malloc(sizeof(struct data_t));
		if (buf == NULL) {
			return;
		}

        timing_t red_stop_time =timing_counter_get();
        timing_stop();
    	uint64_t timing_ns = timing_cycles_to_ns(timing_cycles_get(&red_start_time, &red_stop_time));
		printk("Red task: %lld\n", timing_ns);

        buf->time = timing_ns;
		k_fifo_put(&data_fifo, buf);
		printk("Red added to fifo: %lld\n",buf->time);

		k_condvar_broadcast(&release_signal);
	}
}

void yellow_led_task(void *, void *, void*) {
	
	printk("Yellow led thread started\n");
	while (true) {
        timing_start();
        timing_t yellow_start_time = timing_counter_get();

		// Wait for signal
		k_condvar_wait(&yellow_signal, &yellow_mutex, K_FOREVER);

		// 1. set led on 
		gpio_pin_set_dt(&green,1);
		gpio_pin_set_dt(&red,1);
		printk("Yellow on\n");
		// 2. sleep for 2 seconds
		k_sleep(K_MSEC(yellow_time));
		// 3. set ledR,500 off
		gpio_pin_set_dt(&green,0);
		gpio_pin_set_dt(&red,0);
		printk("Yellow off\n");

        struct data_t *buf = k_malloc(sizeof(struct data_t));
		if (buf == NULL) {
			return;
		}

        timing_t yellow_stop_time = timing_counter_get();
        timing_stop();
        uint64_t timing_ns = timing_cycles_to_ns(timing_cycles_get(&yellow_start_time, &yellow_stop_time));
		printk("Yellow task: %lld\n", timing_ns);

        buf->time = timing_ns;
		k_fifo_put(&data_fifo, buf);
		printk("Yellow added to fifo: %lld\n",buf->time);

		k_condvar_broadcast(&release_signal);
	}
}

void green_led_task(void *, void *, void*) {
	
	printk("Green led thread started\n");
	while (true) {
        timing_start();
        timing_t green_start_time = timing_counter_get();

		// Wait for signal
		k_condvar_wait(&green_signal, &green_mutex, K_FOREVER);

		// 1. set led on 
		gpio_pin_set_dt(&green,1);
		printk("Green on\n");
		// 2. sleep for 2 seconds
		k_sleep(K_MSEC(green_time));
		// 3. set led off
		gpio_pin_set_dt(&green,0);
		printk("Green off\n");

        struct data_t *buf = k_malloc(sizeof(struct data_t));
		if (buf == NULL) {
			return;
		}

        timing_t green_stop_time = timing_counter_get();
        timing_stop();
        uint64_t timing_ns = timing_cycles_to_ns(timing_cycles_get(&green_start_time, &green_stop_time));
		printk("Green task: %lld\n", timing_ns);

        buf->time = timing_ns;
		k_fifo_put(&data_fifo, buf);
		printk("Green added to fifo: %lld\n",buf->time);

		k_condvar_broadcast(&release_signal);
	}
}

int main(void)
{
	timing_init();
    timing_start();
	timing_t start_time = timing_counter_get();

	init_led();
	int ret = init_uart();
	if (ret != 0) {
		printk("UART initialization failed!\n");
		return ret;
	}

    // Wait for everything to initialize
	k_msleep(100);

	printk("Program started..\n");

	timing_t end_time = timing_counter_get();
    timing_stop();
    uint64_t timing_ns = timing_cycles_to_ns(timing_cycles_get(&start_time, &end_time));
	printk("Initialization: %lld\n", timing_ns);
    k_yield();
        

	return 0;
}

/********************
 * UART task
 */
void uart_task(void *unused1, void *unused2, void *unused3)
{
	// Received character from UART
	char rc=0;
	// Message from UART
	char uart_msg[20];
	memset(uart_msg,0,20);
	int uart_msg_cnt = 0;

	while (true) {
		// Ask UART if data available
		if (uart_poll_in(uart_dev,&rc) == 0) {
			// printk("Received: %c\n",rc);
			// If character is not newline, add to UART message buffer
			if (rc != '\r') {
				uart_msg[uart_msg_cnt] = rc;
				uart_msg_cnt++;
				
			// Character is newline, copy dispatcher data and put to FIFO buffer
			} else {
				printk("UART msg: %s\n", uart_msg);
                
				//FIFO puskuri:
				struct data_t *buf = k_malloc(sizeof(struct data_t));
				if (buf == NULL) {
					return;
				}
				// Copy UART message to dispatcher data
				// strncpy(buf->msg, 20, uart_msg); // mitä ihmettä, miksi kaatuu!!
				snprintf(buf->msg, 20, "%s", uart_msg);

				// You need to:
				// Put dispatcher data to FIFO buffer
				k_fifo_put(&dispatcher_fifo, buf);

				// Clear UART receive buffer
				uart_msg_cnt = 0;
				memset(uart_msg,0,20);
			}
		}
		k_msleep(10);
	}
}

/********************
 * Dispatcher task
 */
void dispatcher_task(void *unused1, void *unused2, void *unused3)
{
	while (true) {
		// Receive dispatcher data from uart_task fifo
		struct data_t *rec_item = k_fifo_get(&dispatcher_fifo, K_FOREVER);
		char sequence[50];
		memcpy(sequence,rec_item->msg,20);
		k_free(rec_item);

		printk("Dispatcher: %s\n", sequence);

		// You need to:
        // Parse color and time from the fifo data
        // Example
		char *osa = strtok(sequence, ",");			//Tämä leikkaa merkkijonon siitä kun tulee ',' jonka
													//jälkeen merkkijonosta voidaan poimia värit ja ajat
		while (osa != NULL){
			char color = osa[0];
			int time = atoi(osa+2);

			printk("Data: %c %d\n", color, time);

			if (color == 'R'){
				red_time = time;
				k_condvar_broadcast(&red_signal);
				k_condvar_wait(&release_signal, &release_mutex, K_FOREVER);
			}
			else if (color == 'Y'){
				yellow_time = time;
				k_condvar_broadcast(&yellow_signal);
				k_condvar_wait(&release_signal, &release_mutex, K_FOREVER);
			}
			else if (color == 'G'){
				green_time = time;
				k_condvar_broadcast(&green_signal);
				k_condvar_wait(&release_signal, &release_mutex, K_FOREVER);
			}
			osa = strtok(NULL, ",");	//siirrytään seuraavaan kohtaan merkkijonossa
		}
	}
}


void debug_task(void *, void *, void*) {
	// Store received data
	struct data_t *received;
	uint64_t total_time = 0;

	while (true) {
		received = k_fifo_get(&data_fifo, K_FOREVER);
		total_time += received->time;
		printk("Debug received: %lld\n", received->time);
		k_free(received);
		
        printk("Led aika: %lld ns | Yhteensä: %lld ns\n", received->time, total_time);

		k_yield();
	}
}