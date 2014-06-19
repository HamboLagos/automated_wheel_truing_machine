//**************************************************************************************
/** \file task_spoke_count.h This task runs the spoke_counter, which counts spokes
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

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _TASK_SPOKE_COUNT_H_
#define _TASK_SPOKE_COUNT_H_

#include <stdlib.h>                         // Prototype declarations for I/O functions
#include <avr/io.h>                         // Header for special function registers

#include "FreeRTOS.h"                       // Primary header for FreeRTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // FreeRTOS inter-task communication queues

#include "frt_task.h"                       // ME405/507 base task class
#include "time_stamp.h"                     // Class to implement a microsecond timer
#include "frt_queue.h"                      // Header of wrapper for FreeRTOS queues
#include "frt_shared_data.h"                // Header for thread-safe shared data

#include "rs232int.h"                       // ME405/507 library for serial comm.


//-------------------------------------------------------------------------------------
/** \brief This task is used to count spokes as they pass the laser/phototransistor.
 *  \details This runs as a separate task to let the RTOS handle its scheduling. The
 * 	run method is used to call spoke_counter's update() function, which updates the
 * 	shared variable spoke_count, so everyone has access to the most recent postion
 * 	read.
 */
class task_spoke_count : public frt_task
{
private:
	// No private variables or methods for this class

protected:
	// No protected variables or methods for this class

public:
	// This constructor creates a generic task of which many copies can be made
	task_spoke_count (const char*, unsigned portBASE_TYPE, size_t, emstream*);

	// This method is called by the RTOS once to run the task loop for ever and ever.
	void run (void);
};

#endif // _TASK_SPOKE_COUNT_H_
