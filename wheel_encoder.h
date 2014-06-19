//======================================================================================
/** \file wheel_encoder.h
*    wheel_encoder is a driver for the follower wheel's rotary encoder. Its purpose
* 	 is to set the shared variable wheel_direction, so that the rest of the program
* 	 knows which direction the wheel is currently spinning. This is useful when we want
*    to switch the direction of angular velocity of the wheel, so we know exactly when 
*    it begins spinning the opposite direction.
*  Revisions:
*    \li 02-05-13 HL/TJ Version 1.0 created to encode motor position according to lab3 spec.
* 
*  License:
*    This file is copyright 2013 by Hamilton Little and Trevor Jones and is released
*    under the Lesser GNU Public License, version 2. It intended for educational use 
*    only, but its use is not limited thereto. */
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
//======================================================================================

// This define prevents this .H file from being included multiple times in a .CPP file
#ifndef _WHEEL_ENCODER_H_
#define _WHEEL_ENCODER_H_

#include "emstream.h"                       // Header for serial ports and devices
#include "FreeRTOS.h"                       // Header for the FreeRTOS RTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // Header for FreeRTOS queues
#include "semphr.h"                         // Header for FreeRTOS semaphores



//-------------------------------------------------------------------------------------
/** \brief A wheel_encoder is used to track the direction of angular velocity of the wheel.
*  \details This class sets up a wheel_encoder to track the direction of angular
* 			velocity of the wheel. It updates the value of the variable wheel_direction,
* 			which can be accessed through shares.h, or by calling this object's
* 			get_direction method.
*/
class wheel_encoder
{
		private:
		/// The encoder_driver class uses this pointer to a ser. port to say stuff
		emstream* ptr_to_serial;
		
		protected:
            
        public:
            // creates a new wheel_encoder object whose channel A and B pins are
            // connected to the specified external input pins (pInt[5:6])
            wheel_encoder(emstream*);
            
			bool get_direction();	
}; // end of class wheel_encoder

      // This operator prints out information about the wheel_encoder object. It's not 
      // a part of class wheel_encoder, but it operates on objects of class wheel_encoder
      emstream& operator << (emstream&, wheel_encoder&);
	  
#endif // _ENCODER_DRIVER_H_
