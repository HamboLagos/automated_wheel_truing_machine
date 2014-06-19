//**************************************************************************************
/** \file task_mastermind.cpp This task runs the truing algorithm developed for the
 * 	project.
 * 
 *  Revisions:
 *    \li 03-13-13 HL, TJ, & SG mastermind runs the truing algorithm, v0.1
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

#include "frt_text_queue.h"                 // Header for text queue class
#include "shares.h"                         // Shared inter-task communications
#include "mastermind.h"
#include "task_mastermind.h"

//-------------------------------------------------------------------------------------
/** \brief Runs the truing algorithm developed for the project.
 *  @param a_name A character string which will be the name of this task
 *  @param a_priority The priority at which this task will initially run (default: 0)
 *  @param a_stack_size The size of this task's stack in bytes 
 *                      (default: configMINIMAL_STACK_SIZE)
 *  @param p_ser_dev Pointer to a serial device (port, radio, SD card, etc.) which can
 *                   be used by this task to communicate (default: NULL)
 */

task_mastermind::task_mastermind (const char* a_name, 
								 unsigned portBASE_TYPE a_priority, 
								 size_t a_stack_size,
								 emstream* p_ser_dev
								)
	: frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
	// Nothing is done in the body of this constructor. All the work is done in the
	// call to the frt_task constructor on the line just above this one
}


//-------------------------------------------------------------------------------------
/** \brief This contains the truing algorithm logic for the stand. 
 *  \details It is the main driver for the project.
 */
void task_mastermind::run (void)
{		
	int16_t spokes[32]; // the array of measurements of the wheel
	int16_t avg; // the average value of the measurement readings
	bool finished = false; // set to true when the wheel is within alignment
	uint8_t ndx;
	uint8_t worst_spoke;	// the spoke with the largest absolute offset
	int16_t worst_spoke_val;	// the worst spokes offset
	uint8_t worst_spoke_prev;	// the previously worst spoke
	uint8_t prev_spoke;	// the last spoke information was printed to user about
	
	
	
	// disable the watchdog timer, as we have been warned it can cause problems
	wdt_disable();
	
	// greet the user, tell them we're going to measure the wheel
	to_ui->put(HELLO);
	to_ui->put(MEASURING);
	
	// create mastermind and get the first set of readings on the wheel
	mastermind *master = new mastermind(p_serial, new pot_driver(p_serial));	
	avg = master->find_avg(master->measure_all(spokes));
	vTaskDelay (configMS_TO_TICKS (1000)); // pause for 1 second (looks cool)
	
	// tell us about all the information we just found (for debugging)
	*p_serial << "Measurements are: ";
	for(ndx = 0; ndx < max_spokes; ndx++) {
		*p_serial << spokes[ndx] << " ";
	}
	*p_serial << endl;
	*p_serial << "Average is; " << avg << endl;
	
	// Convert raw measuremnts offset values based on average value
	master->con_to_offs(spokes, avg);
	
	// tell us what the offsets are (for debugging)
	*p_serial << "Offsets are: ";
	for(ndx = 0; ndx < max_spokes; ndx++) {
		*p_serial << spokes[ndx] << " ";
	}
	*p_serial << endl;
	
	worst_spoke = master->find_worst(spokes);
	worst_spoke_val = spokes[worst_spoke];
	worst_spoke_prev = -128;
	prev_spoke = -128;

	while(!finished) {
		
		// tell the user where we're going, and go to it
		*p_serial << "and the worst spoke is " << worst_spoke << " with offset of " 
				  << worst_spoke_val << endl;
		desired_spoke = worst_spoke;
		
		// print out where we are as we go to the worst spoke
		while(spoke_count != worst_spoke) {
			if(prev_spoke != spoke_count) {
				*p_serial << "going to " << worst_spoke << " at " << spoke_count << endl;
				prev_spoke = spoke_count;
			}
		}
		
		// logic missing here to tell the us		worst_spoke_prev = worst_spoke;er whether to tighten or loosen the spoke
		to_ui->put(TIGHTEN);
		while(from_ui->is_empty() || from_ui->get() != DID_THAT)	// wait for response
			;
		
		// get new measurements after fixing a spoke
		to_ui->put(MEASURING);
		master->measure_all(spokes);
		avg = master->find_avg(spokes);
		vTaskDelay (configMS_TO_TICKS (1000)); // pause for 1 second (it looks cool)
		
		// tell us about all the information we just found (for debugging)
		*p_serial << "Measurements are: ";
		for(ndx = 0; ndx < max_spokes; ndx++) {
			*p_serial << spokes[ndx] << " ";
		}
		*p_serial << endl;
		*p_serial << "Average is; " << avg << endl;
		
		// Convert raw measuremnts offset values based on average value
		master->con_to_offs(spokes, avg);
		
		// tell us what the offsets are (for debugging)
		*p_serial << "Offsets are: ";
		for(ndx = 0; ndx < max_spokes; ndx++) {
			*p_serial << spokes[ndx] << " ";
		}
		*p_serial << endl;
		
		// find the worst spoke again, and remember which one is was previously
		worst_spoke_prev = worst_spoke;
		worst_spoke = master->find_worst(spokes);	
		worst_spoke_val = spokes[worst_spoke];
		
		// check if we're within tolerance
		if(ABS(worst_spoke_val) < 10) {
			finished = true;
		}
	}
	
	// tell the user nice job
	to_ui->put(GOODBYE);
}			