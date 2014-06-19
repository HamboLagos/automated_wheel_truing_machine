//======================================================================================
/** \file motordriver.h
*    A motordriver object allows access and control of a motor connected to either
*    VNH3SP30 h-bridge motor driver chip on the ATMEGA1281 development boards used
*    in the ME405 class. A motor driver object is created for the appropriate channel
*    (channel is 1 or 2) the programmer wants to control, and convenient methods for 
*    turning on and off, controlling power, and braking are contained within this class.
*
*  Revisions:
*   \li 01-28-13 HL/TJ Version 1.0 created to allow basic control of motors from board
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
#ifndef _MOTORDRIVER_H_
#define _MOTORDRIVER_H_

#include "emstream.h"                       // Header for serial ports and devices
#include "FreeRTOS.h"                       // Header for the FreeRTOS RTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // Header for FreeRTOS queues
#include "semphr.h"                         // Header for FreeRTOS semaphores


//-------------------------------------------------------------------------------------
/** \brief Driver for the h-bridge motor driving chips on the ME405 board.
*  \details This class sets up a motor driver object which communicates with the h-bridge
*          motor driver chips on the ME405 development board to control a motor. Functions
*          are provided to easily change the power setting, brake the motor, and power
*          saving capabilities which turn the chips on and off. 
*/
class motordriver
{
      protected:
            /// The motordriver class uses this pointer to the serial port to say things
            emstream* ptr_to_serial;
            
            /// This register controls the settings of the h-bridge chip
            volatile uint8_t *settingsReg;
            
            /// this is the inA h-bridge chip setting
            uint8_t inA;
			
			/// this is the inB h-bridge chip setting
			uint8_t inB;
				
			/// this is the en h-bridge chip setting
			uint8_t en;
            
            /// This is the OCR1x register, used by the PWM control, which sets duty cycle
            volatile uint16_t *compareReg;
            
            /// Set if an invalid channel param is passed to constructor; prevents use
            bool nullFlag;

      public:
            // creates a new motor driver object for channel 1 or 2
            motordriver(emstream*, uint8_t);
            
            // Sets the power output for this motordriver object
            void set_power(int16_t);
            
            // Turns the selected channel's VHN3SP30 chip on
            void on();
            
            // Turns the selected channel's VHN3SP30 chip off
            void off();
            
            // Sets the selected channel's VHN3SP30 chip to brake mode (shorts motor leads)
            void brake();
			
			 /// Lets a motordriver object know which channel it controls (1 or 2)
            uint8_t whichchannelami;
			
			/// A saved value of the current power output, as a percentage of full output
            uint8_t current_power;
}; // end of class motordriver

      // This operator prints out information about the motordriver object. It's not 
      // a part of class motordriver, but it operates on objects of class motordriver
      emstream& operator << (emstream&, motordriver&);

#endif // _MOTORDRIVER_H_
