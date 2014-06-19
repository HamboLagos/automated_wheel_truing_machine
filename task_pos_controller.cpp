//**************************************************************************************
/** \file task_pos_controller.cpp Runs the task of spinning the wheel to a desired 
 * 	position.
 * 
 *  Revisions:
 *    \li 03-13-13 HL, TJ, & SG controls the wheel's position really well
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
#include "pos_controller.h"
#include "task_pos_controller.h"


//-------------------------------------------------------------------------------------
/** \brief Runs the PID controller used to actuate the motor which spins the wheel.
 *  @param a_name A character string which will be the name of this task
 *  @param a_priority The priority at which this task will initially run (default: 0)
 *  @param a_stack_size The size of this task's stack in bytes 
 *                      (default: configMINIMAL_STACK_SIZE)
 *  @param p_ser_dev Pointer to a serial device (port, radio, SD card, etc.) which can
 *                   be used by this task to communicate (default: NULL)
 */

task_pos_controller::task_pos_controller (const char* a_name, 
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
 *  \details It just runs the update function of the motor controller, which is where
 * 	the PI control logic is located. Other tasks can set the wheel position by changing
 *	 the desired_spoke value (this the shared variable accessed through shares.h).
 */
void task_pos_controller::run (void)
{			
	// disable the watchdog timer, as we have been warned it can cause problems
	wdt_disable();

	// the motor we use to spin the wheel to different positions
	motordriver *ptr_to_md = new motordriver(p_serial, 2);
	
	// the pos_controller will spin the wheel to whichever position we desire
	pos_controller *controller = new pos_controller(p_serial, ptr_to_md, 200, 25, 3, 16);

	
	for(;;)
	{
		controller->update();
			
		vTaskDelay (configMS_TO_TICKS (1));
	}

}