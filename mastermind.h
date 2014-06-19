//*************************************************************************************
/** \file mastermind.h
*    The mastermind class implements the data collection and analyzation methods needed
* 	 by the truing algorithm system. It does not hold the memory where this data should
*    be stored, that is left to the controlling class to maintain.
*
*  Revisions:
*    \li 02-15-13 HL, TJ, & SG Methods for data collection and analysis.
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

// This define prevents this .H file from being included multiple times in a .CPP file
#ifndef _MASTERMIND_H_
#define _MASTERMIND_H_

#include "emstream.h"                       // Header for serial ports and devices
#include "FreeRTOS.h"                       // Header for the FreeRTOS RTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // Header for FreeRTOS queues
#include "semphr.h"                         // Header for FreeRTOS semaphores
#include "pot_driver.h"



//-------------------------------------------------------------------------------------
/** \brief Implements the data collection and analysis functionality needed.
*   \details  The mastermind class implements the data collection and analyzation 
* 	methods needed by the truing algorithm system. It does not hold the memory where 
* 	this data should be stored, that is left to the controlling class to maintain.
* 
*/

class mastermind
{
		private:
		protected:
            /// The mastermind class uses this pointer to a ser. port to say stuff
            emstream* ptr_to_serial;
			
			/// Mastermind uses this pot to get wheel measurements at each spoke
			pot_driver* pot;

      public:
            // constructor for the object
            mastermind(emstream*, pot_driver*);
            
			// gets measurements for all spokes
			int16_t* measure_all(int16_t[]);
			
			// convert measurements to offsets
			int16_t* con_to_offs(int16_t[], int16_t avg);
			
			// find the worst offset spoke
			uint8_t find_worst(int16_t[]);
			
			// find the average of the measurements taken
			int16_t find_avg(int16_t[]);
	
}; // end of class mastermind

      // This operator prints out information about the mastermind object. It's not 
      // a part of class mastermind, but it operates on objects of class mastermind
      emstream& operator << (emstream&, mastermind&);
	  
#endif // _MASTERMIND_H_