//*************************************************************************************
/** \file mastermind.cpp
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

#include <stdlib.h>                         // Include standard library header files
#include <avr/io.h>

#include "rs232int.h"                       // Include header for serial port class
#include "mastermind.h"                 // Include header for the mastermind class
#include "shares.h"

//-------------------------------------------------------------------------------------
/** \brief Constructor for the mastermind object. 
 *  \details This object has methods which implement the measurement collecting
 * 		and analyzing functionality needed by the system. However, it does not hold
 * 		the memory where this data should be stored, it must be passed by the calling
 * 		function to each of this object's methods.
 *  @param  p_serial_port a serial port to allow the object to say stuff
 *  @param  ptr_to_pot the potentiometer to get measurements from
 */
mastermind::mastermind(emstream* p_serial_port, pot_driver* ptr_to_pot) {
	
	// Initialize class variables
	ptr_to_serial = p_serial_port;
	pot = ptr_to_pot;
}

//-------------------------------------------------------------------------------------
/** \brief Measure each of the spokes' pot readings and stores them in the given param. 
 *  \details This takes the reading from the potentiometer for each spoke and saves
 * 				it in the given meas array.
 *  @param  meas the array to save the pot readings into.
 *  @return the given array, so methods can be chain called.
 */
int16_t *mastermind::measure_all(int16_t meas[]){
	int8_t prev_spoke = -127;  // set this to something we should never reach

	// go back -10 to eliminate torque on wheel problem
	desired_spoke = -10;
	while(spoke_count != desired_spoke) {	
		if(prev_spoke != spoke_count) {	
			to_ui->put(GO_BACK);		// tell the user what spoke when it changes
			prev_spoke = spoke_count;
		}
	}
	
	// go 10 past the last spoke to eliminate torque on wheel problem
	desired_spoke = max_spokes+10;
	prev_spoke = -127;				// set this to something we should never reach
	while(spoke_count != desired_spoke) {	
		if(prev_spoke != spoke_count){
			to_ui->put(PRINT_SPOKE);	// tell the user what spoke when it changes
		
			// take a reading each time we pass a spoke
			if(spoke_count >= 0 && spoke_count <= max_spokes) {
				meas[spoke_count] = (int16_t)(pot->get_value(0));
			}
			prev_spoke = spoke_count;
		}
	}
	
	desired_spoke = max_spokes;		// go back to the last spoke (we are now past it)
	return meas;
}

//-------------------------------------------------------------------------------------
/** \brief Converts the given meas array into offset values based on the avg param. 
 *  \details This overwrites the values in the meas array, converting them from the 
 * 			absolute readings to offset values based on the avg value given.
 *  @param  meas the array to save the pot readings into
 *  @param  avg  the average of all the values in the meas array
 *  @return the converted array, so methods can be chain called.
 */
int16_t *mastermind::con_to_offs(int16_t meas[], int16_t avg) {
	uint8_t ndx;
	
	// for each element in the array, convert it from an absolute measurement to an
	// offset by subtracting the avg value passed in.
	for(ndx = 0; ndx < max_spokes; ++ndx) {
		meas[ndx] = (int16_t)(meas[ndx] - avg);
	}
	return meas;
}

//-------------------------------------------------------------------------------------
/** \brief Finds the highest value in offs. 
 *  \details Compares the absolute values of the data in the given array, and returns 
 * 			the index (spoke) where the highest value resides.
 *  @param  offs the array of offset values
 *  @return the spoke whose offset value is the highest (worst)
 */
uint8_t mastermind::find_worst(int16_t offs[]) {
	uint8_t ndx, worst_spoke;
	int16_t worst_val;
	
	worst_spoke = 0;
	worst_val = ABS(offs[worst_spoke]);
	for(ndx = 0; ndx < max_spokes; ++ndx) {
		if(ABS(offs[ndx]) > worst_val) {
			worst_spoke = ndx;
			worst_val = ABS(offs[ndx]);
		}
	}
	
	return worst_spoke;
}

//-------------------------------------------------------------------------------------
/** \brief Finds the average of the values in the given array. 
 *  \details takes the sum of the elements in meas and returns the average.
 * 
 *  @pre max_spokes has been set appropriately
 *  @param  meas the array whose elements the average will be found
 *  @return average of the values of the elements of meas
 */
int16_t mastermind::find_avg(int16_t meas[]) {
int32_t sum = 0;
uint8_t ndx;

	for(ndx = 0; ndx < max_spokes; ++ndx) {
		sum += meas[ndx];
	}
	
	return (int16_t)(sum / max_spokes);
}

/** \brief This overloaded operator prints data about the mastermind.
 *  \details It just says hello. This was used for debugging purposes only.
 *  @param serpt Reference to a serial port to which the printout will be printed
 *  @param m Reference to the mastermind which is being printed
 *  @return A reference to the same serial device on which we write information.
 *          This is used to string together things to write with "<<" operators.
 */
emstream& operator << (emstream& serpt, mastermind& m) {
	serpt << "mastermind says hello" << endl;
	
	m = m;	// to get rid of compiler warning about unused parameter m
	
	return serpt;
}