#ifndef US_OSC_SENDER_UDP_H
#define US_OSC_SENDER_UDP_H

/*
 Copyright (c) 2013, J.D. Koftinoff Software, Ltd.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of J.D. Koftinoff Software, Ltd.. nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL J.D. KOFTINOFF SOFTWARE, LTD.. BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "us_world.h"
#include "us_osc_sender.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct us_osc_sender_udp_s {
    us_osc_sender_t m_base;
    int m_fd;
    struct addrinfo *m_bind_addr;
    struct addrinfo *m_dest_addr;
} us_osc_sender_udp_t;

bool us_osc_sender_udp_init(us_osc_sender_udp_t *self, struct addrinfo *bind_addr, struct addrinfo *dest_addr);

void us_osc_sender_udp_destroy(us_osc_sender_t *self);

void us_osc_sender_udp_set_dest_addr(us_osc_sender_udp_t *self, struct addrinfo *dest_addr);

bool us_osc_sender_udp_send_msg(us_osc_sender_t *self, const us_osc_msg_t *msg);
bool us_osc_sender_udp_can_send(us_osc_sender_t *self);

#ifdef __cplusplus
}
#endif

#endif
