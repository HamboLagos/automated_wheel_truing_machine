//*************************************************************************************
/** \file pot_driver.cpp
 *    The pot_driver is really just a wrapper for the adc on the ME405 boards. Calling
 * 		get_value() returns the adc conversion value at that moment for the channel on 
 * 		which the linear pot is mounted.
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

#include <stdlib.h>                         // Include standard library header files
#include <avr/io.h>

#include "rs232int.h"                       // Include header for serial port class
#include "pot_driver.h"                            // Include header for the A/D class

//-------------------------------------------------------------------------------------
/** \brief This constructor sets up a pot_driver. 
 *  \details This just sets up the ADC in pins as analog inputs, so we can get adc
 * 		readings from them.
 *  @param p_serial_port A pointer to the serial port which writes debugging info. 
 */

pot_driver::pot_driver (emstream* p_serial_port)
{
	ptr_to_serial = p_serial_port;
	
	// Set up the A/D for settings described above
	// ref = avcc w/ ext cap, single ended input = ADC0
	ADMUX &= ~(1<<REFS1) & ~(1<<ADLAR) & ~(1<<MUX4) & ~(1<<MUX3) & ~(1<<MUX2) & ~(1<<MUX1) & ~(1<<MUX0);
	ADMUX |= 1<<REFS0;					
	
	// enable ADC, prescaler = 32
	ADCSRA &= ~(1<<ADSC) & ~(1<<ADATE) & ~(1<<ADIF) & ~(1<<ADIE) & ~(1<<ADPS1);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);		
}


//-------------------------------------------------------------------------------------
/** \brief Return the current potentiometer reading. 
 *  \details The A/D conversion result on the given channel is returned. Use the same
 * 			ADC channel that the linear pot is hooked up to.
 *  @param  ch The A/D channel which is being read must be from 0 to 7
 *  @return The result of the A/D conversion
 */

uint16_t pot_driver::get_value (uint8_t ch)
{	
	ADMUX &= ~(1<<MUX4) & ~(1<<MUX3) & ~(1<<MUX2) & ~(1<<MUX1) & ~(1<<MUX0);
	ADMUX |= ch & ((1<<MUX2) | (1<<MUX1) | (1<<MUX0));	// set chan bits from ch param
	
	ADCSRA |= 1 << ADSC;									// start single conversion
	
	// while ADCSC is 1, conversion is ongoing; HW clears ADCSC when conversion finished
	// dummy watchDog variable forces return in case of hanging conversion
	for(uint16_t watchDog = 0; watchDog < 65535 && ADCSRA & (1<<ADSC); watchDog++) ;
	
	return((uint16_t)ADC);		// return conversion result (10 bit res, right justified)
}

//-------------------------------------------------------------------------------------
/** \brief This overloaded operator prints stuff about the pot_driver. 
 *  \details Currently, it just says hello. We only used this for debugging purposes.
 *  @param serpt Reference to a serial port to which the printout will be printed
 *  @param pd Reference to the A/D driver which is being printed
 *  @return A reference to the same serial device on which we write information.
 *          This is used to string together things to write with "<<" operators.
 */
emstream& operator << (emstream& serpt, pot_driver& pd)
{	
		serpt << "pot_driver says hello" << endl;
		
		pd = pd; // to get rid of unused parameter compiler warnings

	return (serpt);
}

