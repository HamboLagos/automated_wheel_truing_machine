//**************************************************************************************
/** \file task_mastermind.h This task runs the truing algorithm developed for the
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

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _TASK_MASTERMIND_H_
#define _TASK_MASTERMIND_H_

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
/** \brief Runs the truing algorithm developed for the project.
 *  \details This class implements the actual details of taking measurements of the
 * 	wheel, analyzing that data, going to a spoke, and telling the user what to do 
 * to it (tighten or loosen).
 */
class task_mastermind : public frt_task
{
private:
	// No private variables or methods for this class

protected:
	// No protected variables or methods for this class

public:
	// This constructor creates a generic task of which many copies can be made
	task_mastermind (const char*, unsigned portBASE_TYPE, size_t, emstream*);

	// This method is called by the RTOS once to run the task loop for ever and ever.
	void run (void);
};

#endif // _TASK_MASTERMIND_H_
