//*************************************************************************************
/** \file spoke_counter.cpp
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

#include <stdlib.h>                         // Include standard library header files
#include <avr/io.h>

#include "rs232int.h"                       // Include header for serial port class
#include "shares.h"
#include "spoke_counter.h"                 // Include header for the spoke_counter class


		
// This is the shared position variable, which is set in the update() function
volatile int8_t spoke_count;

// This is the number of spokes on the wheel. It is also a shared variable
uint8_t max_spokes;
		
// the wheel encoder uses this encoder to tell direction we are spinning currently
static wheel_encoder *wheel;

/* This is the local copy of current wheels position. We use a local copy here so that
 * no one has write access to this class's actual count, but can have read access to it
 * by frequently updating the shared variable spoke_count */
static volatile int8_t count;

//-------------------------------------------------------------------------------------
/** \brief Sets up the spoke counter (implemented by the laser/phottransistor sensor).
*  \details We detect, using an external interrupt, when the laser beam has been
* 	broken, and update or decrement the count of the spoke we just passed based on 
*   what direction the wheel is spinning.
*  @param p_serial_port a pointer to the serial port this object can use to print messages
*  @param we the wheel encoder used to tell the direction the wheel is spinning
*  @param num_spokes the number of spokes on the wheel
*/
spoke_counter::spoke_counter(emstream* p_serial_port, 
							  wheel_encoder *we, uint8_t num_spokes) {
	
	// initialize member data
	ptr_to_serial = p_serial_port;
	wheel = we;
	
	// clear the current count (assign 0 to current position), and initialize the shared
	// variables
	count = 0;
	spoke_count = 0;
	max_spokes = num_spokes;
	
	
	// Set up external interrupts on PE4 (the phototransistor is hooked up to this chan)
	EICRB |=  (1 << ISC40) | (1 << ISC41);		// interrupt rising edge
	DDRE &= ~(1 << PE4);						// set PE4 as input	
	EIMSK |= (1 << INT4);						// enable bit in mask
	
	sei(); // enable global interrupts, duh
}

//-------------------------------------------------------------------------------------
/** \brief Updates the globally shared spoke_count.
*  \details This method should be called frequently to make sure everyone has access
* 		to the actual position of the wheel.
*/
void spoke_counter::update() {
	// just to be extra safe, we disable interrupts when updating the shared variable
	cli();
	spoke_count = count;
	sei();
}

//-------------------------------------------------------------------------------------
/** \cond NOT_ENABLED ISR for external interrupt on pin 4 (PortE pin 4). This has been
 * 	set up to trigger only on rising edge. This is where count is incremented or
 *  decremented, accordingly.
*/
ISR(INT4_vect) {
	
	// if wheel direction is true, increment. Else Decrement
	if (wheel->get_direction()){
		count++;
	}else{
		count--;	
	}	
}

/** \endcond end of undocumented code */

//-------------------------------------------------------------------------------------
/** \brief This overloaded operator prints data about the spoke_counter.
 *  \details Currently, it just says hello. This was used for debugging purposes.
 *  @param serpt Reference to a serial port to which the printout will be printed
 *  @param se Reference to the spoke_counter which is being printed
 *  @return A reference to the same serial device on which we write information.
 *          This is used to string together things to write with "<<" operators.
 */
emstream& operator << (emstream& serpt, spoke_counter& se) {
		
	serpt << "spoke_counter says hello" << endl;
	
	se = se; // to get rid of unused parameter compiler warnings
	
	return serpt;
}