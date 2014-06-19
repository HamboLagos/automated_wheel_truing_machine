//*************************************************************************************
/** \file shares.h
 *    This file contains extern declarations for queues and shared variables used in
 * 	  the auto-truing program.
 *
 *  Revisions:
 *    \li 03-13-13 HL, TJ, & SG all shared queues and variables included
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

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _SHARES_H_
#define _SHARES_H_

#include "frt_text_queue.h"
#include "frt_queue.h"

//-------------------------------------------------------------------------------------
// Externs:  In this section, we declare variables and functions that are used in all
// (or at least two) of the files in the data acquisition project. Each of these items
// will also be declared exactly once, without the keyword 'extern', in one .cpp file
// as well as being declared extern here. 

/** These are the messages which can be passed from the truing algorithm task
 *	to the user interface task */
typedef enum ui_messages { HELLO, GOODBYE, TIGHTEN, LOOSEN, TRY_AGAIN, MEASURING, DONE, 
							PRINT_SPOKE, GO_BACK, DONE_MEASURING, WAIT, STOP_WAITING, 
							ENTER_SPOKES, FIRST_SPOKE, ECHO} ui_messages;

/** These are the messages which the user interface task can send back to the truing
 * algorithm task, which originate from user input */
typedef enum messages_from_ui { DID_THAT, ACK } messages_from_ui;

/** This is the index of the spoke which most recently passed the spoke_counter */
extern volatile int8_t spoke_count;

/** This is the number of spokes on the wheel */
extern uint8_t max_spokes;

/** This is true if the wheel is spinning cw, false if spinning ccw, when viewed from
 *  the quick release lever side of the wheel */
extern volatile bool wheel_direction;

/** set this to let the pos_controller where to go */
extern volatile int8_t desired_spoke;

/** lets the user tell us whether the first spoke is on the left or right, so we know
 * later on whether to tell them to loosen or tighten a given spoke */
extern bool left_or_right;

/** This queue allows allows us to print stuff from anywhere in the code to the
  terminal console through the serial usb port. */
extern frt_text_queue* print_ser_queue;

/** This queue is used to send messages to the user interface, so it knows what to 
 * print out or prompt the user for */
extern frt_queue<ui_messages> *to_ui;

/** This queue is used by the user interface to send responses it gets from the user
 * back to the mastermind task. */
extern frt_queue<messages_from_ui> *from_ui;

/** Useful when we need to find the absolute value of something */
#define ABS(x) ((x) < 0 ? (-(x)) : (x))

#endif // _SHARES_H_
