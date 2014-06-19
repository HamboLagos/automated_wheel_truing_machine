//*************************************************************************************
/** \file auto_truing_stand_main.cpp
 *   This class contains the main routine of the automated truing stand. It is the
 *   launching point for the program that runs the automated truing stand, helping the
 *   mechanic to true a bicycle wheel. It starts each of the tasks our system uses
 *   (the spoke counter, PI controller, mastermind, and the user interface), and 
 *  creates the queues used for intertask communication. 
 *
 *  Revisions:
 *    \li 02-11-13 HL, TJ, & SG creates a task_spoke_count to verify spoke counter works
 * 	  \li 02-15-13 HL, TJ, & SG implemented truing stand tasks and queues
 *
 *  License:
*    This file is copyright 2013 by Hamilton Little, Trevor Jones and Sean Green 
*    and is released under the Lesser GNU Public License, version 2. It intended for 
*    educational use only, but its use is not limited thereto. */
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
*    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
*    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
*    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
*    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUEN-
*    TIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
*    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
*    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
//*************************************************************************************


#include <stdlib.h>                         // Prototype declarations for I/O functions
#include <avr/io.h>                         // Port I/O for SFR's
#include <avr/wdt.h>                        // Watchdog timer header
#include <string.h>                         // Functions for C string handling

#include "FreeRTOS.h"                       // Primary header for FreeRTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // FreeRTOS inter-task communication queues
#include "croutine.h"                       // Header for co-routines and such

#include "rs232int.h"                       // ME405/507 library for serial comm.
#include "time_stamp.h"                     // Class to implement a microsecond timer
#include "frt_task.h"                       // Header of wrapper for FreeRTOS tasks
#include "frt_text_queue.h"                 // Wrapper for FreeRTOS character queues
#include "frt_queue.h"                      // Header of wrapper for FreeRTOS queues
#include "frt_shared_data.h"                // Header for thread-safe shared data

#include "shares.h"                         // Global ('extern') queue declarations
#include "task_spoke_count.h"
#include "spoke_counter.h"
#include "task_mastermind.h"
#include "task_pos_controller.h"
#include "task_user_interface.h"
#include "pos_controller.h"	



// Declare the queues which are used by tasks to communicate with each other here. 
// Each queue must also be declared 'extern' in a header file which will be read 
// by every task that needs to use that queue. The format for all queues except 
// the serial text printing queue is 'frt_queue<type> name (size)', where 'type' 
// is the type of data in the queue and 'size' is the number of items (not neces-
// sarily bytes) which the queue can hold

/** This is a print queue, descended from \c emstream so that things can be printed 
 *  into the queue using the "<<" operator and they'll come out the other end as a 
 *  stream of characters. It's used by tasks that send things to the user interface 
 *  task to be printed. 
 */
frt_text_queue* print_ser_queue;

/** This is the queue used to send information to the user interface task, so it knows
	what to tell the user or prompt them for at the appropriate time. */
frt_queue<ui_messages> *to_ui;

/** This is the queue used by the user interface to let another task know what the user
	has entered after being prompted for something. */
frt_queue<messages_from_ui> *from_ui;

//=====================================================================================
/** \brief Starts the RTOS and sets up the tasks and queues used.
 * 		After all these have been set up, it calls the task scheduler to start running
 * 		the tasks, so the mechanic can fix their wheel!
 *  @return This is a real-time microcontroller program which doesn't return. Ever.
 */

int main (void)
{
	// Disable the watchdog timer unless it's needed later. This is important because
	// sometimes the watchdog timer may have been left on...and it tends to stay on
	MCUSR = 0;
	wdt_disable ();

	// Configure a serial port which can be used by a task to print debugging infor-
	// mation, or to allow user interaction, or for whatever use is appropriate.  The
	// serial port will be used by the user interface task after setup is complete and
	// the task scheduler has been started by the function vTaskStartScheduler()
	rs232* ser_port = new rs232 (9600, 1);
	*ser_port << clrscr << PMS ("ME405 Auto Truing Stand Starting") << endl;

	// Create the queues and other shared data items here
	print_ser_queue = new frt_text_queue (32, ser_port, 10);
	to_ui = new frt_queue<ui_messages> (20);
	from_ui = new frt_queue<messages_from_ui> (20);
	
	// These are the tasks we designed to count the spokes as they go by, control the 
	// wheel position, implement the truing algorithm we developed, and interface with
	// the user, respectively.
 	new task_spoke_count("Spokes On", task_priority(1), 400, ser_port);
 	new task_pos_controller("Motor On", task_priority(1), 400, ser_port);
	new task_mastermind("Logic On", task_priority (1), 400, ser_port);
	new task_user_interface("UI on", task_priority(1), 200, ser_port);
	
	// Here's where the RTOS scheduler is started up. It should never exit as long as
	// power is on and the microcontroller isn't rebooted.
	vTaskStartScheduler ();
}

