
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

/******************************************************************************/

/*
 * This function will schedule a packet arrival at a time given by
 * event_time. At that time the function "packet_arrival" (located in
 * packet_arrival.c) is executed. An object can be attached to the event and
 * can be recovered in packet_arrival.c.
 */

long int
schedule_packet_arrival_event(Simulation_Run_Ptr simulation_run,
			      double event_time, unsigned switch_index)
{
  Event event;

  switch (switch_index)
  {

    case 0:
      event.description = "Packet Arrival Switch 1";
      event.function = packet_arrival_event;
      break;

    case 1:
      event.description = "Packet Arrival Switch 2";
      event.function = packet_arrival_event_switch_2;
      break;

    case 2:
      event.description = "Packet Arrival Switch 3";
      event.function = packet_arrival_event_switch_3;
      break;

    case 3:
      event.description = "Packet Arrival Switch 2 no reschedule";
      event.function = packet_arrival_event_switch_2_no_reschedule;
      break;

    case 4:
      event.description = "Packet Arrival Switch 3 no reschedule";
      event.function = packet_arrival_event_switch_3_no_reschedule;
      break;
  }
  event.attachment = (void *) NULL;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when a packet arrival event
 * occurs. It creates a new packet object and places it in either the fifo
 * queue if the server is busy. Otherwise it starts the transmission of the
 * packet. It then schedules the next packet arrival event.
 */

void
packet_arrival_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run, 0);
  data->arrival_count++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->service_time = get_packet_transmission_time(0);
  new_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link) == BUSY) {
    fifoqueue_put(data->buffer, (void*) new_packet);
  } else {
    start_transmission_on_link(simulation_run, new_packet, data->link, 0);
  }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */

  schedule_packet_arrival_event(simulation_run,
			simulation_run_get_time(simulation_run) +
			exponential_generator((double) 1/PACKET_ARRIVAL_RATE), 0);
}


void
packet_arrival_event_switch_2(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run, 1);
  data->arrival_count++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->service_time = get_packet_transmission_time(1);
  new_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link) == BUSY) {
    fifoqueue_put(data->buffer, (void*) new_packet);
  } else {
    start_transmission_on_link(simulation_run, new_packet, data->link, 1);
  }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */

  schedule_packet_arrival_event(simulation_run,
			simulation_run_get_time(simulation_run) +
			exponential_generator((double) 1/PACKET_ARRIVAL_RATE_SWITCH_2), 1);
}


void
packet_arrival_event_switch_2_no_reschedule(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run, 1);
  data->arrival_count++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->service_time = get_packet_transmission_time(1);
  new_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link) == BUSY) {
    fifoqueue_put(data->buffer, (void*) new_packet);
  } else {
    start_transmission_on_link(simulation_run, new_packet, data->link, 1);
  }
}


void
packet_arrival_event_switch_3(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run, 2);
  data->arrival_count++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->service_time = get_packet_transmission_time(2);
  new_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link) == BUSY) {
    fifoqueue_put(data->buffer, (void*) new_packet);
  } else {
    start_transmission_on_link(simulation_run, new_packet, data->link, 2);
  }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */

  schedule_packet_arrival_event(simulation_run,
			simulation_run_get_time(simulation_run) +
			exponential_generator((double) 1/PACKET_ARRIVAL_RATE_SWITCH_3), 2);
}


void
packet_arrival_event_switch_3_no_reschedule(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run, 2);
  data->arrival_count++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->service_time = get_packet_transmission_time(2);
  new_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link) == BUSY) {
    fifoqueue_put(data->buffer, (void*) new_packet);
  } else {
    start_transmission_on_link(simulation_run, new_packet, data->link, 2);
  }
}



