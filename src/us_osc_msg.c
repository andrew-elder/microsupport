#include "us_world.h"

#include "us_osc_msg.h"
#include "us_osc_msg_print.h"

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




bool
avbc_is_msg_bundle(
    us_buffer_t *buffer
)
{
    bool r=false;
    if ( buffer->m_cur_read_pos+8 < buffer->m_cur_length )
    {
        if ( strncmp( (const char *)buffer->m_buffer + buffer->m_cur_read_pos, "#bundle", 7 ) == 0 )
        {
            r=true;
        }
    }
    return r;
}

bool
avbc_is_msg(
    us_buffer_t *buffer
)
{
    bool r=false;
    if ( buffer->m_cur_read_pos+8 < buffer->m_cur_length )
    {
        const char * p = (const char *)buffer->m_buffer + buffer->m_cur_read_pos;
        if ( p != NULL && *p=='/' )
        {
            r=true;
        }
    }
    return r;
}


us_osc_msg_t *
us_osc_msg_form(
    us_allocator_t *allocator,
    const char *address,
    const char *typetags,
    ...
)
{
    us_osc_msg_t *r = 0;
    us_osc_msg_t *self;
    va_list ap;
    const char *t=typetags;
    us_osc_msg_element_t *e;
    va_start( ap, typetags );
    if ( *t==',' )
        ++t;
    self = us_osc_msg_create( allocator, address );
    if ( self )
    {
        r = self;
        for ( ; *t; ++t )
        {
            e = 0;
            switch ( *t )
            {
            case 'a':
            {
                uint32_t high, low;
                high = va_arg( ap, uint32_t );
                low = va_arg( ap, uint32_t );
                e = us_osc_msg_element_a_create( allocator, high, low );
            }
            break;
            case 'b':
            {
                const uint8_t *data;
                int32_t length;
                data = va_arg( ap, const uint8_t * );
                length = va_arg( ap, int32_t );
                e = us_osc_msg_element_b_create( allocator, data, length );
            }
            break;
#if US_ENABLE_DOUBLE
            case 'd':
            {
                double v;
                v = va_arg( ap, double );
                e = us_osc_msg_element_d_create( allocator, v );
            }
            break;
#endif
            case 'F':
            {
                e = us_osc_msg_element_F_create( allocator );
            }
            break;
#if US_ENABLE_FLOAT
            case 'f':
            {
                float v;
                v = (float)va_arg( ap, double );
                e = us_osc_msg_element_f_create( allocator, v );
            }
            break;
#endif
            case 'h':
            {
                uint32_t high, low;
                high = va_arg( ap, uint32_t );
                low = va_arg( ap, uint32_t );
                e = us_osc_msg_element_h_create( allocator, high, low );
            }
            break;
            case 'I':
            {
                e = us_osc_msg_element_I_create( allocator );
            }
            break;
            case 'i':
            {
                int32_t v;
                v = va_arg( ap, int32_t );
                e = us_osc_msg_element_i_create( allocator, v );
            }
            break;
            case 'N':
            {
                e = us_osc_msg_element_N_create( allocator );
            }
            break;
            case 's':
            {
                const char *s;
                s = va_arg( ap, const char * );
                e = us_osc_msg_element_s_create( allocator, s );
            }
            break;
            case 'T':
            {
                e = us_osc_msg_element_T_create( allocator );
            }
            break;
            case 't':
            {
                uint32_t high, low;
                high = va_arg( ap, uint32_t );
                low = va_arg( ap, uint32_t );
                e = us_osc_msg_element_t_create( allocator, high, low );
            }
            break;
            default:
            {
                e = 0;
            }
            break;
            }
            if ( e )
            {
                self->append( self, e );
            }
            else
            {
                r = 0;
                break;
            }
        }
    }
    va_end( ap );
    return r;
}

us_osc_msg_bundle_t *
us_osc_msg_bundle_create(
    us_allocator_t *allocator,
    uint32_t timetag_high,
    uint32_t timetag_low
)
{
    us_osc_msg_bundle_t * r = 0;
    us_osc_msg_bundle_t * self = 0;
    self = us_new(allocator, us_osc_msg_bundle_t );
    if (self)
    {
        self->m_timetag_high = timetag_high;
        self->m_timetag_low = timetag_low;
        self->append = us_osc_msg_bundle_append;
        self->flatten = us_osc_msg_bundle_flatten;
#if US_ENABLE_PRINTING
        self->print = us_osc_msg_bundle_print;
#endif
        self->m_first_msg = 0;
        self->m_last_msg = 0;
        r = self;
    }
    return r;
}

us_osc_msg_t *
us_osc_msg_bundle_append(
    us_osc_msg_bundle_t *self,
    us_osc_msg_t *msg
)
{
    us_osc_msg_t *r = 0;
    if (self && msg)
    {
        if (!self->m_first_msg)
        {
            self->m_first_msg = msg;
        }
        else
        {
            self->m_last_msg->m_next = msg;
        }
        self->m_last_msg = msg;
        r = msg;
    }
    return r;
}

bool
us_osc_msg_bundle_flatten(
    us_osc_msg_bundle_t *self,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    us_osc_msg_t *cur = 0;
    int32_t start_length = (int32_t)buf->m_cur_length;
    r &= buf->append(buf, "#bundle", 8);
    r &= us_buffer_append_uint64(
             buf,
             self->m_timetag_high,
             self->m_timetag_low
         );
    cur = self->m_first_msg;
    if (!cur)
    {
        /* empty bundle! */
        r &= us_buffer_append_int32(
                 buf,
                 0
             );
    }
    else
    {
        int32_t msg_size = 0;
        uint8_t *last_msg_size_ptr = 0;
        while (cur && r)
        {
            /* get the position of the length field in the buffer */
            last_msg_size_ptr = buf->m_cur_write_ptr;
            /* put a placeholder there */
            r &= us_buffer_append_int32(
                     buf,
                     0
                 );
            r &= cur->flatten(
                     cur,
                     buf,
                     &msg_size
                 );
            /* fill in the placeholder with the actual length */
            *last_msg_size_ptr++ = US_GET_BYTE_3(msg_size);
            *last_msg_size_ptr++ = US_GET_BYTE_2(msg_size);
            *last_msg_size_ptr++ = US_GET_BYTE_1(msg_size);
            *last_msg_size_ptr++ = US_GET_BYTE_0(msg_size);
            /* go to the next msg in list */
            cur = cur->m_next;
        }
    }
    /* finish off the bundle with a zero length */
    r &= us_buffer_append_int32(
             buf,
             0
         );
    /* figure out how long the whole thing was and  fill in total_length */
    if (total_length)
    {
        *total_length = buf->m_cur_length - start_length;
    }
    return r;
}

us_osc_msg_bundle_t *
us_osc_msg_bundle_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_bundle_t *result = 0;
    us_osc_msg_bundle_t *bundle = 0;
    if ( avbc_is_msg_bundle(buf) )
    {
        uint32_t timetag_high;
        uint32_t timetag_low;
        buf->m_cur_read_pos += 8;
        if ( us_buffer_read_uint64(
                    buf,
                    &timetag_high,
                    &timetag_low
                ) )
        {
            us_osc_msg_t *msg = 0;
            bundle = us_osc_msg_bundle_create(
                         allocator,
                         timetag_high,
                         timetag_low
                     );
            while (true)
            {
                bool r = false;
                int32_t msg_size = 0;
                /* read message length */
                r = us_buffer_read_int32(
                        buf,
                        &msg_size
                    );
                /* failure to read message length means fail */
                if ( r==false )
                {
                    return 0;
                }
                /* message length of 0 means no more messages for this bundle */
                if ( msg_size==0 )
                {
                    break;
                }
                /* try unflatten message */
                msg = us_osc_msg_unflatten(
                          allocator,
                          buf
                      );
                /* fail if unflattening message failed */
                if ( !msg )
                {
                    return 0;
                }
                /* append msg to bundle or else fail */
                if ( us_osc_msg_bundle_append(
                            bundle,
                            msg
                        )==0 )
                {
                    return 0;
                }
            }
            /* success */
            result = bundle;
        }
    }
    return result;
}


us_osc_msg_t *
us_osc_msg_create(
    us_allocator_t *allocator,
    const char *address
)
{
    us_osc_msg_t * r = 0;
    us_osc_msg_t * self = 0;
    self = us_new(allocator, us_osc_msg_t );
    if (self && address)
    {
        int32_t address_len = (int32_t)strlen(address);
        self->m_allocator = allocator;
        self->m_address = allocator->alloc(allocator, address_len + 1, 1);
        if (self->m_address)
        {
            strcpy(self->m_address, address);
            self->destroy = us_osc_msg_destroy;
            self->append = us_osc_msg_append;
            self->flatten = us_osc_msg_flatten;
#if US_ENABLE_PRINTING
            self->print = us_osc_msg_print;
#endif
            self->m_first_element = 0;
            self->m_last_element = 0;
            self->m_next = 0;
            r = self;
        }
    }
    return r;
}


void us_osc_msg_destroy( us_osc_msg_t *self )
{
    if ( self->m_allocator )
    {
        self->m_allocator->free( self->m_allocator, self->m_address );
    }
}

us_osc_msg_t *
us_osc_msg_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_t *result = 0;
    us_osc_msg_t *msg = 0;
    char addr[AVBC_MAX_ADDRESS_LENGTH];
    char types[AVBC_MAX_TYPETAGS];
    if ( us_buffer_read_rounded_string(buf, addr, sizeof(addr)) )
    {
        if ( us_buffer_read_rounded_string( buf, types, sizeof(types) ) )
        {
            msg = us_osc_msg_create(allocator, addr );
            if ( msg && types[0]==',' )
            {
                char *cur_type = &types[1];
                while ( *cur_type )
                {
                    us_osc_msg_element_t *e;
                    e = us_osc_msg_element_unflatten(
                            allocator,
                            buf,
                            *cur_type
                        );
                    /* Check for error unflattening element */
                    if ( !e )
                    {
                        return 0;
                    }
                    /* Check for error appending element to message */
                    if ( !us_osc_msg_append(msg, e ) )
                    {
                        return 0;
                    }
                    /* go to next element type */
                    cur_type++;
                }
                /* Success Parsing Message */
                result = msg;
            }
        }
    }
    return result;
}



us_osc_msg_element_t *
us_osc_msg_append(
    us_osc_msg_t *self,
    us_osc_msg_element_t *element
)
{
    us_osc_msg_element_t *r = 0;
    if (self && element)
    {
        if (!self->m_first_element)
        {
            self->m_first_element = element;
        }
        else
        {
            self->m_last_element->m_next = element;
        }
        self->m_last_element = element;
        r = element;
    }
    return r;
}

bool
us_osc_msg_flatten(
    us_osc_msg_t *self,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    int32_t start_length = buf->m_cur_length;
    us_osc_msg_element_t *cur = 0;
    char typetag[128] = ",";
    char *typetag_pos = &typetag[1];
    // append address
    r &= us_buffer_append_rounded_string( buf, self->m_address );
    // create type tag string
    cur = self->m_first_element;
    while (cur && r)
    {
        *typetag_pos = cur->m_code;
        typetag_pos++;
        cur = cur->m_next;
    }
    *typetag_pos = '\0';
    r &= us_buffer_append_rounded_string( buf, typetag );
    // iterate through elements and flatten them
    cur = self->m_first_element;
    while (cur && r)
    {
        r &= cur->flatten(cur, buf, 0);
        cur = cur->m_next;
    }
    if ( total_length )
    {
        *total_length = buf->m_cur_length-start_length;
    }
    return r;
}

us_osc_msg_element_t *
us_osc_msg_element_init(
    us_osc_msg_element_t *self,
    int code
)
{
#if US_ENABLE_PRINTING
    self->print = us_osc_msg_element_print;
#endif
    self->flatten = us_osc_msg_element_flatten;
    self->m_code = code;
    self->m_next = 0;
    return self;
}

bool
us_osc_msg_element_flatten(
    us_osc_msg_element_t *self,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    /* default behaviour for msg element is to not be able to flatten.
     * This must be override by subclass.
     */
    return false;
}


us_osc_msg_element_t *
us_osc_msg_element_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf,
    char typetag
)
{
    us_osc_msg_element_t *result = 0;
    switch (typetag)
    {
    case 'a':
        result = us_osc_msg_element_a_unflatten(
                     allocator,
                     buf
                 );
        break;
    case 'b':
        result = us_osc_msg_element_b_unflatten(
                     allocator,
                     buf
                 );
        break;
#if US_ENABLE_DOUBLE
    case 'd':
        result = us_osc_msg_element_d_unflatten(
                     allocator,
                     buf
                 );
        break;
#endif
#if US_ENABLE_FLOAT
    case 'f':
        result = us_osc_msg_element_f_unflatten(
                     allocator,
                     buf
                 );
        break;
#endif
    case 'h':
        result = us_osc_msg_element_h_unflatten(
                     allocator,
                     buf
                 );
        break;
    case 'i':
        result = us_osc_msg_element_i_unflatten(
                     allocator,
                     buf
                 );
        break;
    case 's':
        result = us_osc_msg_element_s_unflatten(
                     allocator,
                     buf
                 );
        break;
    case 't':
        result = us_osc_msg_element_t_unflatten(
                     allocator,
                     buf
                 );
        break;
    case 'T':
        result = us_osc_msg_element_T_unflatten(
                     allocator,
                     buf
                 );
        break;
    case 'F':
        result = us_osc_msg_element_F_unflatten(
                     allocator,
                     buf
                 );
        break;
    case 'N':
        result = us_osc_msg_element_N_unflatten(
                     allocator,
                     buf
                 );
        break;
    default:
        /* unknown type code = error */
        result=0;
        break;
    }
    return result;
}



us_osc_msg_element_t *
us_osc_msg_element_s_create(
    us_allocator_t *allocator,
    const char *value
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_s_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_s_t );
    if (self)
    {
        int32_t len = (int32_t)strlen(value);
        int32_t rounded_size = us_round_size(len+1);
        us_osc_msg_element_init(&self->m_base, 's');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_s_print;
#endif
        self->m_base.flatten = us_osc_msg_element_s_flatten;
        self->m_length = len;
        self->m_value = allocator->alloc(allocator, rounded_size, 1);
        if (self->m_value)
        {
            int32_t i;
            for (i = 0; i < len; ++i)
            {
                self->m_value[i] = value[i];
            }
            for (i = len; i < rounded_size; ++i)
            {
                self->m_value[i] = '\0';
            }
            r = &self->m_base;
        }
    }
    return r;
}

bool
us_osc_msg_element_s_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    us_osc_msg_element_s_t *self = (us_osc_msg_element_s_t *) self_;
    int32_t start_length = buf->m_cur_length;
    r&=us_buffer_append_rounded_string(buf, self->m_value );
    if ( total_length )
    {
        *total_length = buf->m_cur_length-start_length;
    }
    return r;
}

us_osc_msg_element_t *
us_osc_msg_element_s_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    char str[1024];
    if ( us_buffer_read_rounded_string(buf, str, sizeof(str) ) )
    {
        result = us_osc_msg_element_s_create(allocator, str );
    }
    return result;
}



#if US_ENABLE_FLOAT
us_osc_msg_element_t *
us_osc_msg_element_f_create(
    us_allocator_t *allocator,
    float value
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_f_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_f_t );
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 'f');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_f_print;
#endif
        self->m_base.flatten = us_osc_msg_element_f_flatten;
        self->m_value = value;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_f_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    us_osc_msg_element_f_t *self = (us_osc_msg_element_f_t *) self_;
    int32_t start_length = buf->m_cur_length;
    r&=us_buffer_append_float32(buf, (float)self->m_value );
    if ( total_length )
    {
        *total_length = buf->m_cur_length-start_length;
    }
    return r;
}

us_osc_msg_element_t *
us_osc_msg_element_f_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    float value = 0;
    if ( us_buffer_read_float32(buf, &value) )
    {
        result = us_osc_msg_element_f_create(allocator, value );
    }
    return result;
}

#endif

#if US_ENABLE_DOUBLE

us_osc_msg_element_t *
us_osc_msg_element_d_create(
    us_allocator_t *allocator,
    double value
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_d_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_d_t );
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 'd');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_d_print;
#endif
        self->m_base.flatten = us_osc_msg_element_d_flatten;
        self->m_value = value;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_d_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    us_osc_msg_element_d_t *self = (us_osc_msg_element_d_t *) self_;
    int32_t start_length = buf->m_cur_length;
    r&=us_buffer_append_float64(buf, self->m_value );
    if ( total_length )
    {
        *total_length = buf->m_cur_length-start_length;
    }
    return r;
}

us_osc_msg_element_t *
us_osc_msg_element_d_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    double value = 0;
    if ( us_buffer_read_float64(buf, &value) )
    {
        result = us_osc_msg_element_d_create(allocator, value );
    }
    return result;
}


#endif

us_osc_msg_element_t *
us_osc_msg_element_i_create(
    us_allocator_t *allocator,
    int32_t value
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_i_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_i_t );
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 'i');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_i_print;
#endif
        self->m_base.flatten = us_osc_msg_element_i_flatten;
        self->m_value = value;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_i_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    us_osc_msg_element_i_t *self = (us_osc_msg_element_i_t *) self_;
    int32_t start_length = buf->m_cur_length;
    r&=us_buffer_append_int32( buf, self->m_value );
    if ( total_length )
    {
        *total_length = buf->m_cur_length-start_length;
    }
    return r;
}

us_osc_msg_element_t *
us_osc_msg_element_i_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    int32_t value = 0;
    if ( us_buffer_read_int32(buf, &value) )
    {
        result = us_osc_msg_element_i_create(allocator, value );
    }
    return result;
}





us_osc_msg_element_t *
us_osc_msg_element_b_create(
    us_allocator_t *allocator,
    const uint8_t *data,
    int32_t length
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_b_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_b_t );
    if (self)
    {
        int32_t rounded_length = us_round_size(length);
        us_osc_msg_element_init(&self->m_base, 'b');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_b_print;
#endif
        self->m_base.flatten = us_osc_msg_element_b_flatten;
        self->m_data = allocator->alloc(allocator, rounded_length,1 );
        if (self->m_data)
        {
            int32_t i;
            for (i = 0; i < length; ++i)
            {
                self->m_data[i] = data[i];
            }
            for (i = length; i < rounded_length; ++i)
            {
                self->m_data[i] = '\0';
            }
            r = &self->m_base;
        }
    }
    return r;
}

bool
us_osc_msg_element_b_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    us_osc_msg_element_b_t *self = (us_osc_msg_element_b_t *) self_;
    int32_t start_length = buf->m_cur_length;
    r&=us_buffer_append_rounded_data(buf, self->m_data, self->m_length);
    if ( total_length )
    {
        *total_length = buf->m_cur_length-start_length;
    }
    return r;
}

us_osc_msg_element_t *
us_osc_msg_element_b_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    uint8_t data[1024];
    int32_t data_length = 0;
    if ( us_buffer_read_rounded_data(buf, data, sizeof(data), &data_length ) )
    {
        result = us_osc_msg_element_b_create(allocator, data, data_length);
    }
    return result;
}


us_osc_msg_element_t *
us_osc_msg_element_t_create(
    us_allocator_t *allocator,
    uint32_t time_high,
    uint32_t time_low
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_t_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_t_t );
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 't');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_t_print;
#endif
        self->m_base.flatten = us_osc_msg_element_t_flatten;
        self->m_time_high = time_high;
        self->m_time_low = time_low;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_t_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    us_osc_msg_element_t_t *self = (us_osc_msg_element_t_t *) self_;
    int32_t start_length = buf->m_cur_length;
    r&=us_buffer_append_uint64(buf, self->m_time_high, self->m_time_low );
    if ( total_length )
    {
        *total_length = buf->m_cur_length-start_length;
    }
    return r;
}

us_osc_msg_element_t *
us_osc_msg_element_t_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    uint32_t high;
    uint32_t low;
    if ( us_buffer_read_uint64(buf, &high, &low) )
    {
        result = us_osc_msg_element_t_create(allocator, high, low );
    }
    return result;
}


us_osc_msg_element_t *
us_osc_msg_element_a_create(
    us_allocator_t *allocator,
    uint32_t time_high,
    uint32_t time_low
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_a_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_a_t );
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 'a');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_a_print;
#endif
        self->m_base.flatten = us_osc_msg_element_a_flatten;
        self->m_time_high = time_high;
        self->m_time_low = time_low;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_a_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    us_osc_msg_element_a_t *self = (us_osc_msg_element_a_t *) self_;
    int32_t start_length = buf->m_cur_length;
    r&=us_buffer_append_uint64(buf, self->m_time_high, self->m_time_low );
    if ( total_length )
    {
        *total_length = buf->m_cur_length-start_length;
    }
    return r;
}

us_osc_msg_element_t *
us_osc_msg_element_a_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    uint32_t high;
    uint32_t low;
    if ( us_buffer_read_uint64(buf, &high, &low) )
    {
        result = us_osc_msg_element_a_create(allocator, high, low );
    }
    return result;
}


us_osc_msg_element_t *
us_osc_msg_element_h_create(
    us_allocator_t *allocator,
    uint32_t value_high,
    uint32_t value_low
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_h_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_h_t );
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 'h');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_h_print;
#endif
        self->m_base.flatten = us_osc_msg_element_h_flatten;
        self->m_value_high = value_high;
        self->m_value_low = value_low;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_h_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    us_osc_msg_element_h_t *self = (us_osc_msg_element_h_t *) self_;
    int32_t start_length = buf->m_cur_length;
    r&=us_buffer_append_uint64(buf, self->m_value_high, self->m_value_low);
    if ( total_length )
    {
        *total_length = buf->m_cur_length-start_length;
    }
    return r;
}



us_osc_msg_element_t *
us_osc_msg_element_h_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    uint32_t high;
    uint32_t low;
    if ( us_buffer_read_uint64(buf, &high, &low) )
    {
        result = us_osc_msg_element_h_create(allocator, high, low );
    }
    return result;
}


us_osc_msg_element_t *
us_osc_msg_element_T_create(
    us_allocator_t *allocator
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_T_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_T_t);
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 'T');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_T_print;
#endif
        self->m_base.flatten = us_osc_msg_element_T_flatten;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_T_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    if ( total_length )
    {
        *total_length = 0;
    }
    return r;
}



us_osc_msg_element_t *
us_osc_msg_element_T_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    result = us_osc_msg_element_T_create(allocator);
    return result;
}

us_osc_msg_element_t *
us_osc_msg_element_F_create(
    us_allocator_t *allocator
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_F_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_F_t);
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 'F');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_F_print;
#endif
        self->m_base.flatten = us_osc_msg_element_F_flatten;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_F_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    if ( total_length )
    {
        *total_length = 0;
    }
    return r;
}



us_osc_msg_element_t *
us_osc_msg_element_F_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    result = us_osc_msg_element_F_create(allocator);
    return result;
}


us_osc_msg_element_t *
us_osc_msg_element_N_create(
    us_allocator_t *allocator
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_N_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_N_t);
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 'N');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_N_print;
#endif
        self->m_base.flatten = us_osc_msg_element_N_flatten;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_N_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    if ( total_length )
    {
        *total_length = 0;
    }
    return r;
}



us_osc_msg_element_t *
us_osc_msg_element_N_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    result = us_osc_msg_element_N_create(allocator);
    return result;
}




us_osc_msg_element_t *
us_osc_msg_element_I_create(
    us_allocator_t *allocator
)
{
    us_osc_msg_element_t * r = 0;
    us_osc_msg_element_I_t * self = 0;
    self = us_new(allocator, us_osc_msg_element_I_t);
    if (self)
    {
        us_osc_msg_element_init(&self->m_base, 'F');
#if US_ENABLE_PRINTING
        self->m_base.print = us_osc_msg_element_F_print;
#endif
        self->m_base.flatten = us_osc_msg_element_I_flatten;
        r = &self->m_base;
    }
    return r;
}

bool
us_osc_msg_element_I_flatten(
    us_osc_msg_element_t *self_,
    us_buffer_t *buf,
    int32_t *total_length
)
{
    bool r = true;
    if ( total_length )
    {
        *total_length = 0;
    }
    return r;
}



us_osc_msg_element_t *
us_osc_msg_element_I_unflatten(
    us_allocator_t *allocator,
    us_buffer_t *buf
)
{
    us_osc_msg_element_t *result = 0;
    result = us_osc_msg_element_I_create(allocator);
    return result;
}


