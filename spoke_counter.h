//======================================================================================
/** \file spoke_counter.h
 *	spoke_counter allows the programmer to track how many spokes have past the
 *   laser/phototransistor sensor set up on the bicycle wheel stand. It uses the an
 * 	encoder fixed to the wheel to determine the wheel's true direction of spin, and
 * 	increments or decrements the count accordingly.
 *
 *  Revisions:
 *    \li 03-13-13 HL, TJ, & SG spoke_counter is up and running
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
#ifndef _SPOKE_COUNTER_H_
#define _SPOKE_COUNTER_H_

#include "emstream.h"                       // Header for serial ports and devices
#include "FreeRTOS.h"                       // Header for the FreeRTOS RTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // Header for FreeRTOS queues
#include "semphr.h"                         // Header for FreeRTOS semaphores
#include "wheel_encoder.h"



//-------------------------------------------------------------------------------------
/** \brief Counts the spokes as they pass the laser/phototransistor sensor.
*  \details spoke_counter allows the programmer to track how many spokes have past the
*   laser/phototransistor sensor set up on the bicycle wheel stand. It uses the an
* 	encoder fixed to the wheel to determine the wheel's true direction of spin, and
* 	increments or decrements the count accordingly.
*/
class spoke_counter
{
	private:

	protected:
		/// The encoder_driver class uses this pointer to a ser. port to say stuff
		emstream* ptr_to_serial;
		
	public:
		// creates a new spoke_counter object to count spokes
		spoke_counter(emstream*, wheel_encoder*, uint8_t);
		
		// update the shared variable spoke_counter
		void update();
}; // end of class spoke_counter

// This operator prints out information about the encoder_driver object. It's not 
// a part of class encoder_driver, but it operates on objects of class encoder_driver
emstream& operator << (emstream&, spoke_counter&);
	  
#endif // _SPOKE_COUNTER_H_
