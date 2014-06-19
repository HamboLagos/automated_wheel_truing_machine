//*************************************************************************************
/** \file pos_controller.cpp
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

#include <stdlib.h>                         // Include standard library header files
#include <avr/io.h>

#include "rs232int.h"                       // Include header for serial port class
#include "pos_controller.h"                 // Include header for the pos_controller class
#include "shares.h"
#include "pos_controller.h"

// this is the desired spoke to move to. it can be set by anyone.
volatile int8_t desired_spoke = 0;

//-------------------------------------------------------------------------------------
/** \brief Sets up a PID controller to automate moving the wheel to different positions.
*  \details KP, KI, FeedForward gain, and integrator limit can be set here. Integrator
* 	clamping is set internally.
*  @param p_serial_port a pointer to the serial port this object can use to print messages
*  @param md the motor used to turn the wheel
*  @param KP_input Proportional gain 
*  @param KI_input Integral gain
*  @param FF_gain_input FeedForward gain
*  @param int_limit integrator limit (number of spokes)
*/
pos_controller::pos_controller(emstream *p_serial_port, motordriver *md, uint8_t KP_input, 
							   uint8_t KI_input, uint8_t FF_gain_input, uint8_t int_limit) {	
	
	// save the p_serial_port so we can print stuff out if needed
	ptr_to_serial = p_serial_port;
	
	// set up the motor
	motor = md;
	motor->on();
	
	// save member variables
	KP = KP_input;
	KI = KI_input;
	FF_gain = FF_gain_input;
	limit = int_limit;
	
	// clear esum, just in case
	esum = 0;
}

//-------------------------------------------------------------------------------------
/** \brief Updates the motor actuation signal.
*  \details Update must be called frequently to be able to precisely position the wheel.
* 		This used to be done in a timer-compare interrupt to set the update frequency,
* 		but now we let the RTOS handle its scheduling.
*/
void pos_controller::update() {
	int8_t pos_act = spoke_count;
	int8_t pos_des = desired_spoke;
	int32_t KI_control = 0, KP_control = 0;
	int16_t control = 0;
	int8_t e = pos_des - pos_act;
	
	// motor braking if we are at desired spoke
	if(!e) {
		motor->set_power(0);
	} 
	else {
		// Integrator control, with limiting
		if(ABS(e) <= limit) {
			
			// integrator clamping
			if(esum + e > 127) {
				esum = 127;
			} else if (esum + e < -128) {
				esum = -128;
			} else {
				esum += e;
			}
			KI_control = esum * FF_gain * KI;
		} 
		else {
			esum = 0;
		}
		
		// Proportional control
		KP_control = e * FF_gain * KP;
		
		// Control saturation (so we don't overflow motor actuation signal)
		if(KP_control + KI_control > 32000) {
			control = 32000;
		} else if(KP_control + KI_control < -32000) {
			control = -32000;
		} else {
			control = KP_control + KI_control;
		}
		
		// update the motor actuation signal
		motor->set_power(control);
	}
}

//-------------------------------------------------------------------------------------
/** \brief This overloaded operator prints data about the pos_controller.
 *  \details It just says hello. This was used for debugging purposes only.
 *  @param serpt Reference to a serial port to which the printout will be printed
 *  @param pc Reference to the pos_controller which is being printed
 *  @return A reference to the same serial device on which we write information.
 *          This is used to string together things to write with "<<" operators.
 */
emstream& operator << (emstream& serpt, pos_controller& pc) {
	serpt << "position controller says hi" << endl;
	
	pc = pc;	// to get rid of unused paramter compiler warnings
	
	return serpt;
}