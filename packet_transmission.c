
/*
 * 
 * Simulation_Run of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/******************************************************************************/

#include <stdio.h>
#include "trace.h"
#include "main.h"
#include "output.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

/******************************************************************************/

/*
 * This function will schedule the end of a packet transmission at a time given
 * by event_time. At that time the function "end_packet_transmission" (defined
 * in packet_transmissionl.c) is executed. A packet object is attached to the
 * event and is recovered in end_packet_transmission.c.
 */

long
schedule_end_packet_transmission_event(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link, unsigned switch_index)
{
  Event event;

  switch (switch_index)
  {

    case 0:
      event.description = "Packet Xmt End Switch 1";
      event.function = end_packet_transmission_event;
      break;

    case 1:
      event.description = "Packet Xmt End Switch 2";
      event.function = end_packet_transmission_event_switch_2;
      break;

    case 2:
      event.description = "Packet Xmt End Switch 3";
      event.function = end_packet_transmission_event_switch_3;
      break;
  }

  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when the end of a packet
 * transmission event occurs. It updates its collected data then checks to see
 * if there are other packets waiting in the fifo queue. If that is the case it
 * starts the transmission of the next packet.
 */

void
end_packet_transmission_event(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run,0);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);

  /* Collect statistics. */
  data->number_of_packets_processed++;
  data->accumulated_delay += simulation_run_get_time(simulation_run) - 
    this_packet->arrive_time;
  if(1e3* (simulation_run_get_time(simulation_run) - this_packet->arrive_time) > 20)
  {
    data->number_above_20_ms++;
  }

  /* Output activity blip every so often. */
  output_progress_msg_to_screen(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void *) this_packet);

  // Pass new packet to either switch 2 or 3
  // unsigned switch_index = ((unsigned)rand() % 2) + 3; // 50/50 chance
  unsigned switch_index = ((unsigned)rand() % 100); // 50/50 chance
  if (switch_index < (unsigned)(PROB1_2*100u))
    switch_index = 3;
  else
    switch_index = 4;
  schedule_packet_arrival_event(simulation_run, simulation_run_get_time(simulation_run), switch_index);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
    start_transmission_on_link(simulation_run, next_packet, link, 0);
  }
}

void
end_packet_transmission_event_switch_2(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run,1);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);

  /* Collect statistics. */
  data->number_of_packets_processed++;
  data->accumulated_delay += simulation_run_get_time(simulation_run) - 
    this_packet->arrive_time;
  if(1e3* (simulation_run_get_time(simulation_run) - this_packet->arrive_time) > 20)
  {
    data->number_above_20_ms++;
  }

  /* Output activity blip every so often. */
  // output_progress_msg_to_screen(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void *) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
    start_transmission_on_link(simulation_run, next_packet, link, 1);
  }
}


void
end_packet_transmission_event_switch_3(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run,2);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);

  /* Collect statistics. */
  data->number_of_packets_processed++;
  data->accumulated_delay += simulation_run_get_time(simulation_run) - 
    this_packet->arrive_time;
  if(1e3* (simulation_run_get_time(simulation_run) - this_packet->arrive_time) > 20)
  {
    data->number_above_20_ms++;
  }

  /* Output activity blip every so often. */
  // output_progress_msg_to_screen(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void *) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
    start_transmission_on_link(simulation_run, next_packet, link, 2);
  }
}

/*
 * This function ititiates the transmission of the packet passed to the
 * function. This is done by placing the packet in the server. The packet
 * transmission end event for this packet is then scheduled.
 */

void
start_transmission_on_link(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link,
         unsigned switch_index)
{
  TRACE(printf("Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link, switch_index);
}

/*
 * Get a packet transmission time. For now it is a fixed value defined in
 * simparameters.h
 */

double
get_packet_transmission_time(unsigned switch_index)
{
  return switch_index ? ((double) PACKET_XMT_TIME_SWITCH_2_3) : ((double) PACKET_XMT_TIME);
}


