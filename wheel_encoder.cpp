//*************************************************************************************
/** \file wheel_encoder.cpp
*    wheel_encoder is a driver for the follower wheel's rotary encoder. Its purpose
* 	 is to set the shared variable wheel_direction, so that the rest of the program
* 	 knows which direction the wheel is currently spinning. This is useful when we want
*    to switch the direction of angular velocity of the wheel, so we know exactly when 
*    it begins spinning the opposite direction.
* 
 *  Revisions:
 *    \li 03-13-13 HL, TJ, & SG wheel_encoder can tell the direction of the wheel
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
#include "wheel_encoder.h"                 // Include header for the wheel_encoder class
#include "shares.h"

/** Used internally to save the direction we see the wheel is spinning */
volatile bool wheel_direction;

/** Used internally when doing grey_code calculations to tell direction */
static volatile bool chan5high, chan6high;

//-------------------------------------------------------------------------------------
/** \brief Creates a new wheel_encoder object to read wheel velocity direction.
*  \details This constructor sets up a new wheel_encoder to read on PE[5:6]
*/
wheel_encoder::wheel_encoder(emstream* p_serial_port) {
	ptr_to_serial = p_serial_port;
	
	chan5high = 0;
	chan6high = 0;
	wheel_direction = true;
	
	// Set up interrupts on PE[6:5]
	EICRB |= (1 << ISC50) | (1 << ISC60);		// interrupt on logical change
	EICRB &= ~((1 << ISC51) & (1 << ISC61));
	DDRE &= ~((1 << PE5) & (1 << PE6));		// set PE[5:6] as input	
	EIMSK |= (1 << PE5) | (1 << PE6);			// Enable interrupt masks
	
	// Set the global interrupt flag on the status control register
	sei();
}

//-------------------------------------------------------------------------------------
/** \brief Returns the direction the wheel is spinning.
*  \details This can also be obtained by including shares.h, and reading the
* 			wheel_direction variable.
*/
bool wheel_encoder::get_direction() {
	bool direction;
	
	cli();
	direction = wheel_direction;
	sei();
	return direction;
}

//-------------------------------------------------------------------------------------
/** \cond NOT_ENABLED ISR for external interrupt on pin 4 (PortE pin 4). updates
 * 	either count1 or count2 through use of count_pin4 pointer, which was initialized
 *  in the constructor.
*/

/** ISR for external interrupt on pin 5 (PortE pin 5). Updates wheel direction.
*/
ISR(INT5_vect) {
	// current logic level of pin
	chan5high = (bool)(PINE & (1<<PE5));
	wheel_direction = chan5high ? !chan6high : chan6high;
}

/** ISR for external interrupt on pin 6 (PortE pin 6). Updates wheel direction.
*/
ISR(INT6_vect) {
	// current pin logic level
	chan6high = (bool)(PINE & (1<<PE6));
	wheel_direction = chan6high ? chan5high : !chan5high;	
}

/** \endcond end of nondocumented code */

//-------------------------------------------------------------------------------------
/** \brief This overloaded operator prints data about the wheel_encoder.
 *  \details Currently it just says hello. This was used for debugging purposes only.
 *  @param serpt Reference to a serial port to which the printout will be printed
 *  @param we Reference to the wheel_encoder which is being printed
 *  @return A reference to the same serial device on which we write information.
 *          This is used to string together things to write with "<<" operators.
 */
emstream& operator << (emstream& serpt, wheel_encoder& we) {
		serpt <<  "wheel encoder says hi" << endl;
		
		we = we;
		
		return serpt;
}