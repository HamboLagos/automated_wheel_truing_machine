//**************************************************************************************
/** \file task_user_interface.cpp
 *    This file contains source code for the user interface used in the project.
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

#include <avr/io.h>                         // Port I/O for SFR's
#include <avr/wdt.h>                        // Watchdog timer header

#include "shared_data_sender.h"
#include "shared_data_receiver.h"
#include "task_user_interface.h"                      // Header for this file
#include "shares.h"


/** This constant sets how many RTOS ticks the task delays if there's nothing to do.
 *  The duration is calculated to be about 5 ms.
 */
const portTickType ticks_to_delay = ((configTICK_RATE_HZ / 1000) * 5);


//-------------------------------------------------------------------------------------
/** This constructor creates a new data acquisition task. Its main job is to call the
 *  parent class's constructor which does most of the work.
 *  @param a_name A character string which will be the name of this task
 *  @param a_priority The priority at which this task will initially run (default: 0)
 *  @param a_stack_size The size of this task's stack in bytes 
 *                      (default: configMINIMAL_STACK_SIZE)
 *  @param p_ser_dev Pointer to a serial device (port, radio, SD card, etc.) which can
 *                   be used by this task to communicate (default: NULL)
 */

task_user_interface::task_user_interface (const char* a_name, 
					  unsigned portBASE_TYPE a_priority, 
					  size_t a_stack_size,
					  emstream* p_ser_dev)
	: frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
	// Nothing is done in the body of this constructor. All the work is done in the
	// call to the frt_task constructor on the line just above this one
}


//-------------------------------------------------------------------------------------
void task_user_interface::run (void)
{
	for (;;)
	{
		if(!to_ui->is_empty()) {
			switch(to_ui->get()) {
				case HELLO:
					*p_serial << "Wake up Neo..." << endl;
					break;
					
				case GOODBYE:
					*p_serial << "Follow the rabbit, Neo" << endl;
					break;
					
				case TIGHTEN:
					*p_serial << "Tighten the spoke" << endl;
					*p_serial << "Press n to continue" << endl;
					while(!p_serial->check_for_char() || p_serial->getchar() != 'n')
						;
					from_ui->put(DID_THAT);
					break;
					
				case LOOSEN:
					*p_serial << "Loosen the spoke" << endl;
					*p_serial << "Press n to continue" << endl;
					while(!p_serial->check_for_char() || p_serial->getchar() != 'n')
						;
					from_ui->put(DID_THAT);
					break;
					
				case TRY_AGAIN:
					*p_serial << "You did that the wrong way. Let's try again" << endl;
					break;
					
				case MEASURING:
					*p_serial << "Measuring the Wheel. This could take a moment." 
								<< endl;
					break;
					
				case DONE_MEASURING:
					*p_serial << "Done Measuring. Calculating..." << endl;
					break;
					
				case PRINT_SPOKE:
					*p_serial << "At Spoke " << spoke_count << ", going to " 
								<< desired_spoke << endl;
					break;
					
				case GO_BACK:
					*p_serial << "Go to " << desired_spoke << ", you are at " 
								<< spoke_count << endl;
					break;
					
				case DONE:
					*p_serial << "Done with that, on to the next" << endl;
					break;
				
				// this is to be implemented, we just didn't have time to finish it
				/*
				case ENTER_SPOKES:
					*p_serial << "Enter the number of spokes on the bicycle wheel" 
								<< endl;

					while(!p_serial->check_for_char() && !finished){
						val = p_serial->getchar();
						if (val >= '0' || val <= '9'){
 							array[ndex++] = val;
							to_ui->put(ECHO);
							max_spokes = (uint8_t) val;
							
							
						
						} else if (val == 10 || val == 13 || val == 3 || val == 4)  {
 							array[ndex] = '\0';
							finished = 1;
							
 							spoke_count = atoi(array);
							
						}
					}
					break;
					
				case FIRST_SPOKE:
					*p_serial << "Is the first spoke on the left or right (L/R)?" << endl;
					
					while(!p_serial->check_for_char() || p_serial->getchar() != 'l' || p_serial->getchar() != 'r'){
						if(p_serial->getchar() == 'l'){
							left_or_right = 1;
						} else if (p_serial->getchar() == 'r'){
							left_or_right = 0;
						}else{
						
						}
					
						
					from_ui->put(DID_THAT);
					}
					break;
					*/
				
				case WAIT:
					while(to_ui->is_empty() || to_ui->get() != STOP_WAITING)
						;
					break;
				
				// this is to be implemented, we just didn't have time to do it
				/*
 				case ECHO:
 					*p_serial<<max_spokes<<endl;
 					break;
				*/
				default:
					break;
			}
		}
	}
}

