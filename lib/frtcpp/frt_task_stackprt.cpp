//*************************************************************************************
/** \file frt_task_stackprt.cpp
 *    This file contains functions to print the stacks of all the tasks for debugging
 *    and educational purposes. 
 *
 *  Revisions:
 *    \li 12-02-2012 JRR Split off from time_stamp.cpp to save memory in machine file
 *
 *  License:
 *    This file is copyright 2012 by JR Ridgely and released under the Lesser GNU 
 *    Public License, version 2. It intended for educational use only, but its use
 *    is not limited thereto. */
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

#include "frt_task.h"                       // Pull in the base class header file
#include "ansi_terminal.h"                  // Codes to change printing style on screen


//-------------------------------------------------------------------------------------
/** This function has all the tasks in the task list do a "stack dump", printing their
 *  stacks in hex dump format. The idle task's stack is printed afterwards. 
 *  @param ser_dev Pointer to a serial device on which the information will be printed
 */
void print_task_stacks (emstream* ser_dev)
{
	// Tell the most recently created task to print its stack and keep the process
	// going by telling the next most recently created task to print its stack, etc.
	if (last_created_task_pointer != NULL)
	{
		last_created_task_pointer->print_stack_in_list (ser_dev);
	}

	// Now print the stack for the idle task, which isn't user created and so isn't
	// in the regular task list
	*ser_dev << ATERM_BOLD << PMS ("Task: IDLE") << ATERM_NORM_INT << endl;
	hex_dump_memory ((uint8_t*)(portStackTopForTask - configMINIMAL_STACK_SIZE + 1), 
							(uint8_t*)(portStackTopForTask + 1), ser_dev, true, 0x11);
}


//-------------------------------------------------------------------------------------
/** This method displays the task's stack in the format of a hex dump, with the data 
 *  in the stack shown as a bunch of hexadecimal numbers as well as text. Then it 
 *  finds the previously created task and asks that task to print its stack as well; 
 *  when the process is done all tasks (except the idle task) will have printed their 
 *  stacks. The idle task's stack is printed separately afterwards. 
 *  @param ser_device The serial device to which each task prints its stack
 */

void frt_task::print_stack_in_list (emstream* ser_device)
{
	*ser_device << ATERM_BOLD << PMS ("Task: ") 
				<< (const char*)(pcTaskGetTaskName (handle)) 
				<< ATERM_NORM_INT << endl;

	dump_stack (ser_device);

	if (prev_task_pointer != NULL)
	{
		prev_task_pointer->print_stack_in_list (ser_device);
	}
}
