//*************************************************************************************
/** \file motordriver.cpp
*    A motordriver object allows access and control of a motor connected to either
*    VNH3SP30 h-bridge motor driver chip on the ATMEGA1281 development boards used
*    in the ME405 class. A motor driver object is created for the appropriate channel
*    (channel is 1 or 2) the programmer wants to control, and convenient methods for 
*    turning on and off, controlling power, and braking are contained within this class.
*
*  Revisions:
*    \li 01-28-13 HL/TJ Version 1.0 created to allow basic control of motors from board
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
//*************************************************************************************

#include <stdlib.h>                         // Include standard library header files
#include <avr/io.h>

#include "rs232int.h"                       // Include header for serial port class
#include "motordriver.h"                    // Include header for the motordriver class

//-------------------------------------------------------------------------------------
/** \brief Creates a new motordriver object to control motor channel 1 or 2.
*  \details A motordriver object can affect one of the two channels on the ME405
*     development board. The channels are labeled on the board. A motordriver object
*     controls a motor by providing an interface to the two VNH3SP30 motor driver chips
*     on the development board. A motordriver object can only affect a motor on the
*     channel it is instantiated for. There is no error checking to prevent multiple
*     motordriver objects from being created for the same channel; if this situation
*     occurs, the behavior of the driver is undefined. <b>ATTENTION:</b> A motordriver
*     object is by default in the off state, you must call the function on() to turn the
*     VHN3SP30 chip on.
*     
*  @pre motorChannel parameter is either 1 or 2.
*  @pre there is not more than one motordriver object per given channel
*  @param p_serial_port a pointer to the serial port this object can use to print messages
*  @param motorChannel Select which h-bridge motor driver chip this object controls (1 or 2).
*/
motordriver::motordriver(emstream* p_serial_port, uint8_t motorChannel) {
      ptr_to_serial = p_serial_port;
      
      // This constructor creates a new motordriver object for use by the ME 405 board. 
      // Depending on the input, the variables inA, inB, EnB will be assigned
      // different values. These are the correct bitshift values depending on whether the
      // motordriver object will affect motor 1 or motor 2
      if(motorChannel == 1) {
            
            whichchannelami = 1;
            
            // initialize member variables (for controlling motor direction and velocity)
            settingsReg = &PORTC; // settings Reg allows us to manipulate H-Bridge driver
            inA = PORTC0; // assign correct inA value for motor 1 bitshift value
            inB = PORTC1; // assign correct inB value for motor 1 bitshift value
            en = PORTC2; // assign correct en value for motor 1 bitshift value
            compareReg = &OCR1B; // assign correct Output Compare Register for this channel
            
            // set initial output compare value at 0 (sets 0v DC output). Do this before
            // setting up pwm output so there is no error output from data previously in
            // this register
            *compareReg = 0;
            
            // Set output pins as outputs in data direction register. Port C pins set the 
            // H-Bridge chip settings (inA, inB, en), and port B pin 6 is the pwm output pin
            DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2);
            DDRB |= (1<<DDB6);
            
            // turn the chip off, in case there is previous junk in the Port C register which
            // would cause the motor to begin running at this point
            off();
            
            // pwm settings: fast pwm (WGM3:0 = 1110), non-inverting (COM1x1:0 = 10)
            // clock source: internal clock, no prescaling (CS12:0 = 001)
            TCCR1A |= (1<<COM1B1) | (1<<WGM11);
            TCCR1A &= ~(1<<COM1B0) & ~(1<<WGM10);
            TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS10);
            TCCR1B &= ~(1<<CS12) & ~(1<<CS11);

      } else if (motorChannel == 2) {
                  
            whichchannelami = 2;
            
            // initialize member variables (for controlling motor direction and velocity)
            settingsReg = &PORTD; // settings Reg allows us to manipulate H-Bridge driver
            inA = PORTD5; // assign correct inA value for motor 1 bitshift value
            inB = PORTD6; // assign correct inB value for motor 1 bitshift value
            en = PORTD7; // assign correct en value for motor 1 bitshift value
            compareReg = &OCR1A; // assign correct Output Compare Register for this channel
            
            // set initial output compare value at 0 (sets 0v DC output). Do this before
            // setting up pwm output so there is no error output from data previously in
            // this register
            *compareReg = 0;
            
            // Set output pins as outputs in data direction register. Port C pins set the 
            // H-Bridge chip settings (inA, inB, en), and port B pin 6 is the pwm output pin
            DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
            DDRB |= (1<<DDB5);
            
            // turn the chip off, in case there is previous junk in the Port D register which
            // would cause the motor to begin running at this point
            off();
            
            // Set output pins as outputs in data direction register. Port D pins set the 
            // H-Bridge chip settings (inA, inB, en), and port B pin 5 is the pwm output pin
            TCCR1A |= (1<<COM1A1) | (1<<WGM11);
            TCCR1A &= ~(1<<COM1A0) & ~(1<<WGM10);
            TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS10);
            TCCR1B &= ~(1<<CS12) & ~(1<<CS11);
            
      } else {
            
            nullFlag = true; // set error flag for other methods within object
            
            // let the user know there was a problem
            DBG (ptr_to_serial, "Channel parameter error: " << motorChannel << 
                  " is not a valid channel for motordriver" << endl);
      }
      
      // If valid channel parameter was passed to this constructor, finish initializing
      // the motor object by performing the set up which is common to either motor
      // motor channelrChan
      if(!nullFlag) {
            // Set the maximum count value for PWM at 2^12 (16,382)
            // Testing shows this gives a frequency of ~400Hz for pwm
            ICR1 = 0x0FFF;
      }	
}


//-------------------------------------------------------------------------------------
/** \brief Set the power ouput of the motor this motordriver controls.
*  \details The power parameter is a signed 16bit number, allowing the motor to be turned
*     forwards or backwards, and at varying speeds. The higher the magnitude of the power
*     parameter, the more power the motor will output. The sign of the power parameter
*     determines the motor spin direction. (Positive is clockwise, negative is 
*     counterclockwise).
*  @param power Power setting for motor (valid range -32768 to 32767)
*/
void motordriver::set_power(int16_t power) {
      if(!nullFlag) {
            // using clockwise as positive, ccw as negative
            if(power < 0) {
                  
                  // for negative power, go ccw, inA is 0 and inB is 1
                  *settingsReg |= (1<<inB);
                  *settingsReg &= ~(1<<inA);
                  
                  power  *= -1;
            }	else {
                  
                  // for positive power, go cw, inA is 1 and inB is 0
                  *settingsReg |= (1<<inA);
                  *settingsReg &= ~(1<<inB);
            }
            
            // current power is the percentage of total power output
            current_power = (uint8_t)(power / 32768.0) * 100;
            
            // power has a max value of 2^15 (32,768), but out TOP value for pwm is 
            // 2^12 (4096), so to fit the power setting we must divide by 8 (2^3)
            // oscilloscope probing shows this yields a frequency of ~400Hz
            power = power/8;
            
            // a higher power parameter passed in means the pwm output will be high 
            // for a longer time before hitting the value in compareReg;
            *compareReg = power;	
      } else {
            // null flag is set, which means this object was created with an invalid channel
            // paramter. Do nothing when set_power is called.
            return;
      }
}

//-------------------------------------------------------------------------------------
/** \brief Turns the VHN3SP30 h-bridge motor driver chip on.
*  \details This is used to turn the motor driver chip on, either after it was turned off
*     manually or upon creation. Note that motordriver objects are created by default in
*     the off state, so this function should be called immediately after creation in
*     order to control the motor it drives.
*/ 
void motordriver::on() {
      if(!nullFlag) {
            // to turn on, set en
            *settingsReg |= (1<<en);
      } else {
            // null flag is set, which means this object was created with an invalid channel
            // paramter. Do nothing when on is called.
            return;
      }
}

//-------------------------------------------------------------------------------------
/** \brief Turns the VHN3SP30 h-bridge motor driver chip off.
*  \details This is used to turn the motor driver chip off. Minimal current will be 
*     drawn when the driver is turned off, however, the user will not be able to
*     control the motor while the motordriver is off.
*/
void motordriver::off() {
      if(!nullFlag) {
            // to turn off, clear en
            *settingsReg &= ~(1<<en);
      } else {
            // null flag is set, which means this object was created with an invalid channel
            // paramter. Do nothing when on is called.
            return;
      }
}

//-------------------------------------------------------------------------------------
/** \brief Sets the VHN3SP30 h-bridge motor driver chip to brake to VCC mode.
*  \details This is used to dynamically brake the motor by shorting its leads together.
*     This brake method shorts the leads to VCC, however, the chip also supports braking
*     to ground.
*/

void motordriver::brake() {
      if(!nullFlag) {
            // to brake, set inA and inB
            *settingsReg |= (1<<inA) | (1<<inB);
      } else {
            // null flag is set, which means this object was created with an invalid channel
            // paramter. Do nothing when on is called.
            return;
      }
}

//-------------------------------------------------------------------------------------
/** \brief This overloaded operator prints data about the motordriver 
 *  \details This method prints the motordrivers channel parameter and current power
 * 	output setting (as a percentage of full power). This operator does not tell
 * 	the user whether the motor is off or in brake mode; the power setting is the 
 * 	output power this driver will set when/if the driver is in the on setting.
 *  @param serpt Reference to a serial port to which the printout will be printed
 *  @param md Reference to the motordriver which is being printed
 *  @return A reference to the same serial device on which we write information.
 *          This is used to string together things to write with "<<" operators.
 */
emstream& operator << (emstream& serpt, motordriver& md) {
		serpt <<  "motorChannel " << md.whichchannelami << "is currently running at " 
		<< md.current_power << "% power" << endl;
		
		return serpt;
}