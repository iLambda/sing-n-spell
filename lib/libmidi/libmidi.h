/*
 * Copyright (c) 2018, The libmidi authors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef MIDI_H_INCLUDED_
#define MIDI_H_INCLUDED_

#include <mbed.h>

/*
 * As libmidi is primarily intended for use in embedded environments, users
 * will likely need to configure their UART. The MIDI_BAUD_RATE macro is
 * provided as a convenience and as a reminder of the correct baud rate.
 * This constant is not used by libmidi.
 */
#define MIDI_BAUD_RATE  31250

/*
 * Error codes returned by library functions.
 */
enum {
    E_MIDI_BAD_EVENT_HANDLER = -1,
    E_MIDI_BAD_CHANNEL_STATE = -2
};


/*
 * As noted elsewhere, libmidi is event-driven, and handles received data
 * via a callback mechanism. The enumeration values of libmidi_event_type describe
 * the different types of events for which callers may register. 
 */
enum libmidi_event_type : uint8_t {
    /*
     * System real-time messages.
     */
    EVT_SYS_REALTIME_TIMING_CLOCK = 0,
    EVT_SYS_REALTIME_RESERVED_F9,
    EVT_SYS_REALTIME_SEQ_START,
    EVT_SYS_REALTIME_SEQ_CONTINUE,
    EVT_SYS_REALTIME_SEQ_STOP,
    EVT_SYS_REALTIME_RESERVED_FD,
    EVT_SYS_REALTIME_ACTIVE_SENSE,
    EVT_SYS_REALTIME_RESET,
    
    /*
     * Channel messages.
     */
    EVT_CHAN_NOTE_OFF,
    EVT_CHAN_NOTE_ON,
    EVT_CHAN_POLY_AFTERTOUCH,
    EVT_CHAN_CONTROL_CHANGE,
    EVT_CHAN_PROGRAM_CHANGE,
    EVT_CHAN_AFTERTOUCH,
    EVT_CHAN_PITCH_BEND,
    
    /*
     * Placeholder whose value is equal to the total number of event types
     * that we support above. 
     */
    EVT_MAX
};


/*
 * This library uses an event-driven paradigm for receiving data: callers
 * register callback functions for events they are interested in, and these
 * are invoked when events are received.
 * 
 * For the sake of simplicity, we define one type, libmidi_event_callback_t for
 * all callback functions. Please note that some parameters are unused in
 * certain contexts: the documentation is clear on what parameters are valid
 * for each event type.
 * 
 * The caller is only obligated to register callback functions for those
 * events she is interested in; other events will be dispatched to a null
 * handler implements within the library.
 */
typedef void (*libmidi_event_callback_t)(libmidi_event_type event, uint8_t chan, uint8_t data1, uint8_t data2);


/**
 * Register an event handler for incoming midi events. To clear an event
 * handler, simply pass a NULL pointer for the callback argument.
 * 
 */
void libmidi_event_handler(libmidi_event_callback_t cb);


/**
 * Processes a byte arriving via the MIDI input of the device. This function
 * *must* be invoked for every byte that arrives so that the internal state
 * machine maintained by the library is updated properly. Calling this function
 * may result in callback invocations. The library user is cautioned to process
 * callbacks as quickly as feasible to avoid missing data and/or events/
 * 
 * On success, the function returns the number of callbacks that resulted from
 * the new byte's arrival. In the event that the received byte is invalid, a 
 * negative code is returned (see the section above that outlines status codes
 * for this library.)
 * 
 * @param byte Byte of data received from an input port.
 * 
 * @return Number of callback invocations. Returns negative status on error.
 */
int8_t libmidi_receive_byte(uint8_t byte);

#endif  /* MIDI_H_INCLUDED_ */ 