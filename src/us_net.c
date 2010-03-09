#include "us_world.h"

#include "us_net.h"

/*
Copyright (c) 2010, Meyer Sound Laboratories, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if US_ENABLE_NETWORK

struct addrinfo *us_net_get_addrinfo(
                                        const char *ip_addr,
                                        const char *ip_port,
                                        int type
                                        )
{
  int e;
  struct addrinfo *ai;
  struct addrinfo hints;
  memset(&hints,'\0',sizeof(hints) );
  hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
  hints.ai_socktype = type;

  do
  {
    e = getaddrinfo( ip_addr,ip_port,&hints,&ai );
  } while( e==EAI_AGAIN );

  if( e!=0 )
  {
    fprintf( stderr, "getaddrinfo: %s\n", gai_strerror(e) );
    ai=0;
  }

  return ai;
}


int us_net_create_udp_socket(
                                struct addrinfo *ai,
                                bool do_bind
                                )
{
  int r = -1;
  int s = -1;

  if( ai )
  {
    s = socket( ai->ai_family, ai->ai_socktype, ai->ai_protocol );
    if( s>=0 )
    {
      int on =1;
      r=s;

      if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) == -1)
      {
        perror("setsockopt SO_BROADCAST:" );
        abort();
      }


      if( do_bind )
      {
        if( bind( s, ai->ai_addr, ai->ai_addrlen )==0 )
        {
          if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
          {
            perror("setsockopt SO_REUSEADDR:" );
            abort();
          }

          r=s;
        }
        else
        {
          perror( "socket: " );
        }
      }
    }
    else
    {
      perror( "socket: " );
    }

    return r;
  }

  if( r==-1 && s!=-1 )
  {
    closesocket(s);
  }

  return s;
}


int us_net_create_tcp_socket(
                                struct addrinfo *ai,
                                bool do_bind
                                )
{
  int r = -1;
  int s = -1;

  if( ai )
  {
    s = socket( ai->ai_family, ai->ai_socktype, ai->ai_protocol );
    if( s>=0 )
    {
      r=s;

      if( do_bind )
      {
        if( bind( s, ai->ai_addr, ai->ai_addrlen )==0 )
        {
          r=s;
        }
        else
        {
          perror( "socket: " );
        }
      }
    }
    else
    {
      perror( "socket: " );
    }

    return r;
  }

  if( r==-1 && s!=-1 )
  {
    closesocket(s);
  }

  return s;
}


#endif


