//**************************************************************************************
/** \file task_spoke_count.cpp This task runs the spoke_counter, which counts spokes
 * 	as they pass the laser/phototransistor sensor to let other tasks know what position
 * 	the wheel is in.
 *
 *  Revisions:
 *    \li 03-13-13 HL, TJ, & SG spoke counter don't miss a thang
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
#include "task_spoke_count.h"                // Header for this task
#include "shares.h"                         // Shared inter-task communications
#include "spoke_counter.h"

//-------------------------------------------------------------------------------------
/** \brief Runs the spoke_counter used to determine the wheel's position.
 *  @param a_name A character string which will be the name of this task
 *  @param a_priority The priority at which this task will initially run (default: 0)
 *  @param a_stack_size The size of this task's stack in bytes 
 *                      (default: configMINIMAL_STACK_SIZE)
 *  @param p_ser_dev Pointer to a serial device (port, radio, SD card, etc.) which can
 *                   be used by this task to communicate (default: NULL)
 */

task_spoke_count::task_spoke_count (const char* a_name, 
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
/** \brief This method is called once by the RTOS scheduler. 
 *  \details It runs as an infinite loop, updating the shared variable spoke_count by
 * 	calling spoke_counter's update() function.
 */
void task_spoke_count::run (void)
{		
	// disable the watchdog timer, as we have been warned it can cause problems
	wdt_disable();
	
	// set up the motor's encoder to pass on to the pos_controller
	wheel_encoder *wheel = new wheel_encoder(p_serial);
	
	// create a new spoke_counter to count spokes as they go by
	spoke_counter *spoker = new spoke_counter(p_serial, wheel, 32);

	for(;;)
	{
		spoker->update();
	}
}