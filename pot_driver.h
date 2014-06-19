//======================================================================================
/** \file pot_driver.h
 *    The pot_driver is really just a wrapper for the adc on the ME405 boards. Calling
 * 		get_value() returns the adc conversion value at that moment on which the
 * 		linear pot is mounted.
 *
 *  Revisions:
 *    \li 03-13-13 HL, TJ, & SG pot_driver is a wrapper for ADC on the board
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
#ifndef _POT_DRIVER_H_
#define _POT_DRIVER_H_

#include "emstream.h"                       // Header for serial ports and devices
#include "FreeRTOS.h"                       // Header for the FreeRTOS RTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // Header for FreeRTOS queues
#include "semphr.h"                         // Header for FreeRTOS semaphores


//-------------------------------------------------------------------------------------
/** \brief Used to get measurements from the linear pot on the stand.
 *  \details The pot_driver is really just a wrapper for the adc on the ME405 boards. 
 * 	Calling	get_value() returns the adc conversion value at that moment for the channel
 * 	on which the linear pot is mounted.
 */

class pot_driver
{
	protected:
		/// The ADC class uses this pointer to the serial port to say hello
		emstream* ptr_to_serial;

      public:
		// The constructor sets up the A/D converter for use. The "= NULL" part is a
		// default parameter, meaning that if that parameter isn't given on the line
		// where this constructor is called, the compiler will just fill in "NULL".
		// In this case that has the effect of turning off diagnostic printouts
		pot_driver (emstream*);

		// This function reads one channel once, returning the result as an unsigned 
		// integer; it should be called from within a normal task, not an ISR
		uint16_t get_value (uint8_t);

}; // end of class pot_driver


// This overloaded operator prints out readings on all 7 ADC input channels (might
// be useful for debugging purposes)
emstream& operator << (emstream&, pot_driver&);

#endif // _POT_DRIVER_H_
