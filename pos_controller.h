//*************************************************************************************
/** \file pos_controller.h
*	 	Implements the PI control used to move the wheel to different positions. This
* 		controller implements feed forward gain, integrator clamping and limiting, and
* 		motor braking once we are at a desired spoke to quickly (and accurately)
* 		control the wheel's position.
*
*  Revisions:
*    \li 03-13-13 HL, TJ, & SG PI control scheme implemented
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
#ifndef _POS_CONTROLLER_H_
#define _POS_CONTROLLER_H_

#include "emstream.h"                       // Header for serial ports and devices
#include "FreeRTOS.h"                       // Header for the FreeRTOS RTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // Header for FreeRTOS queues
#include "semphr.h"                         // Header for FreeRTOS semaphores
#include "motordriver.h"



//-------------------------------------------------------------------------------------
/** \brief PI control scheme to control the position of the wheel.
*  \details Implements the PI control used to move the wheel to different positions. 
*  This controller implements feed forward gain, integrator clamping and limiting, and
*  motor braking once we are at a desired spoke to quickly (and accurately)
*  control the wheel's position.
*/
class pos_controller
{
	private:

	protected:
		/// The pos_controller class uses this pointer to a ser. port to say stuff
		emstream* ptr_to_serial;
		
		/// the motor to drive the wheel
		motordriver *motor;
		
		/// summation of previous errors (used in KI branch)
		int8_t esum;

		/// Proportional gain
		uint8_t KP;

		/// Integral gain
		uint8_t KI;

		/// FeedForward Gain
		uint8_t FF_gain;

		/// integrator limiting range (number of spokes)
		int8_t limit;
		
	public:
		// creates a new pos_controller object to count spokes
		pos_controller(emstream*, motordriver*, uint8_t, uint8_t, uint8_t, uint8_t);

		// update the motor actuation
		void update();
		
		
}; // end of class pos_controller

// This operator prints out information about the pos_controller object. It's not 
// a part of class pos_controller, but it operates on objects of class pos_controller
emstream& operator << (emstream&, pos_controller&);
	  
#endif // _POS_CONTROLLER_H_
