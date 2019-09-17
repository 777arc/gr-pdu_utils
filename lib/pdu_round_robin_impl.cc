/* -*- c++ -*- */
/* 
 * Copyright 2018 gr-pdu_utils author.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pdu_round_robin_impl.h"

namespace gr {
  namespace pdu_utils {

    pdu_round_robin::sptr
    pdu_round_robin::make(int num_outputs)
    {
      return gnuradio::get_initial_sptr
        (new pdu_round_robin_impl(num_outputs));
    }

    /*
     * The private constructor
     */
    pdu_round_robin_impl::pdu_round_robin_impl(int num_outputs)
      : gr::block("pdu_round_robin",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
      d_num_outputs(num_outputs),
      d_counter(0),
      d_port_names(num_outputs, "pdu_out")
    {
      // inputs
      message_port_register_in(pmt::mp("pdu_in"));
      set_msg_handler(pmt::mp("pdu_in"), boost::bind(&pdu_round_robin_impl::pdu_handler, this, _1));

      // outputs
      for (int i=0; i<d_num_outputs; i++) {
        d_port_names[i] += std::to_string(i);
        printf("port name: %s\n", d_port_names[i].c_str());
        message_port_register_out(pmt::mp(d_port_names[i]));
      }
    }

    /*
     * Our virtual destructor.
     */
    pdu_round_robin_impl::~pdu_round_robin_impl()
    {}

    void pdu_round_robin_impl::pdu_handler(pmt::pmt_t pdu) {
      message_port_pub(pmt::mp(d_port_names[d_counter]), pdu);
      d_counter = (d_counter+1) % d_num_outputs;
    }

  } /* namespace pdu_utils */
} /* namespace gr */
