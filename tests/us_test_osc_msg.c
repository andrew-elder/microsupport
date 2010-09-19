#include "us_world.h"
#include "us_testutil.h"
#include "us_logger_printer.h"

#include "us_buffer.h"
#include "us_osc_msg.h"

#if US_ENABLE_PRINTING
#include "us_osc_msg_print.h"
#include "us_buffer_print.h"
#endif

/** \addtogroup avbc_test_msg */
/*@{*/

bool
do_test_osc_msg(
    us_allocator_t *allocator,
    us_buffer_t *buffer,
    us_print_t *printer
);

bool
do_test_osc_msg_bundle(
    us_allocator_t *allocator,
    us_buffer_t *buffer,
    us_print_t *printer
);

bool
test_unflatten(
    us_allocator_t *allocator,
    us_buffer_t  *buffer,
    us_print_t *printer
);



bool
test_unflatten(
    us_allocator_t *allocator,
    us_buffer_t  *buffer,
    us_print_t *printer
)
{
    bool r=false;
    us_log_probe();
    us_log_info( "unflattening message" );
    if ( avbc_is_msg_bundle(buffer) )
    {
        us_osc_msg_bundle_t *bundle;
        us_log_info( "unflattening bundle" );
        bundle = us_osc_msg_bundle_unflatten(allocator,buffer);
        if ( bundle )
        {
            r=true;
#if US_ENABLE_PRINTING
            printer->printf( printer, "Parsed bundle: \n" );
            bundle->print( bundle, printer );
            printer->printf( printer, "\n" );
#endif
            {
                us_buffer_t *second_buffer = us_buffer_create(allocator, 4096);
                if ( second_buffer )
                {
                    if ( bundle->flatten( bundle, second_buffer, 0 ) )
                    {
#if US_ENABLE_PRINTING
                        {
                            printer->printf( printer, "Flattened message buffer contents:\n" );
                            second_buffer->print( second_buffer, printer );
                        }
#endif
                    }
                }
            }
        }
    }
    else if ( avbc_is_msg(buffer) )
    {
        us_osc_msg_t *msg;
        us_log_info( "unflattening message" );
        msg = us_osc_msg_unflatten( allocator, buffer );
        if ( msg )
        {
            r=true;
#if US_ENABLE_PRINTING
            printer->printf( printer, "Parsed message: \n" );
            msg->print( msg, printer );
            printer->printf( printer, "\n" );
#endif
            {
                us_buffer_t *second_buffer = us_buffer_create(allocator, 4096);
                if ( second_buffer )
                {
                    if ( msg->flatten( msg, second_buffer, 0 ) )
                    {
#if US_ENABLE_PRINTING
                        {
                            printer->printf( printer, "Flattened message buffer contents:\n" );
                            second_buffer->print( second_buffer, printer );
                        }
#endif
                    }
                }
            }
        }
    }
    return r;
}


bool do_test_osc_msg(
    us_allocator_t *allocator,
    us_buffer_t *buffer,
    us_print_t *printer
)
{
    bool r=false;
    us_osc_msg_t *msg;
    us_log_probe();
    us_log_info( "creating test message" );
#if 0
    msg = us_osc_msg_form(
              allocator,
              "/in/1/level/db",
              ",f",
              -10.0f
          );
#endif
#if 0
    msg = us_osc_msg_form(
              allocator,
              "/out/1/filter/1/delay/ms",
              ",f",
              777.0f
          );
#endif
#if 1
    msg = us_osc_msg_form(
              allocator,
              "/out/1/gain/db",
              ",f",
              20.0f
          );
#endif
    if ( msg )
    {
#if US_ENABLE_PRINTING
        us_osc_msg_print( msg, printer );
#endif
        if ( buffer )
        {
            us_log_debug( "flattening test message" );
            if ( msg->flatten( msg, buffer, 0 ) )
            {
#if US_ENABLE_PRINTING
                {
                    printer->printf( printer, "Flattened message buffer contents:\n" );
                    buffer->print( buffer, printer );
                }
#endif
                r=test_unflatten(allocator, buffer, printer );
            }
        }
    }
#if US_ENABLE_PRINTING
    printer->printf( printer, "\nResult: %d\n\n", r );
#endif
    return r;
}


bool do_test_osc_msg_bundle(
    us_allocator_t *allocator,
    us_buffer_t *buffer,
    us_print_t *printer
)
{
    bool r=false;
    us_osc_msg_bundle_t *bundle;
    us_log_probe();
    us_log_info( "creating test bundle" );
    bundle = us_osc_msg_bundle_create(
                 allocator,
                 0,
                 0
             );
    bundle->append(
        bundle,
        us_osc_msg_form(
            allocator,
            "/in/1/level/db",
            ",f",
            -10.0f
        )
    );
    bundle->append(
        bundle,
        us_osc_msg_form(
            allocator,
            "/out/1/level/db",
            ",f",
            40.0
        )
    );
    if ( bundle )
    {
#if US_ENABLE_PRINTING
        us_osc_msg_bundle_print( bundle, printer );
#endif
        if ( buffer )
        {
            if ( bundle->flatten( bundle, buffer, 0 ) )
            {
#if US_ENABLE_PRINTING
                int32_t bundle_size = buffer->m_cur_length;
                {
                    printer->printf( printer, "bundle size: %ld\n", bundle_size );
                    printer->printf( printer, "Flattened message bundle buffer contents:\n" );
                    buffer->print( buffer, printer );
                }
#endif
                r=test_unflatten(allocator, buffer, printer );
            }
        }
    }
#if US_ENABLE_PRINTING
    printer->printf( printer, "\nResult: %d\n\n", r );
#endif
    return r;
}


int main( int argc, char **argv )
{
    bool r=true;
    r=us_testutil_start(8192,8192,argc,argv);
    if ( r )
    {
#if US_ENABLE_LOGGING
        us_logger_printer_start( us_testutil_printer_stdout, us_testutil_printer_stderr );
#endif
        us_log_info( "Hello world from %s compiled on %s", __FILE__, __DATE__ );
        {
            us_buffer_t *buffer = 0;
            us_log_debug( "resetting session allocator" );
            us_allocator_reset(us_testutil_session_allocator);
            us_log_debug( "allocating 4k buffer from session" );
            buffer = us_buffer_create(us_testutil_session_allocator, 4096);
            if ( buffer )
            {
                us_log_debug( "testing msg" );
                r &= do_test_osc_msg(
                         us_testutil_session_allocator,
                         buffer,
                         us_stdout
                     );
                buffer->destroy( buffer );
            }
            else
            {
                us_log_error( "error allocating buffer" );
            }
        }
        {
            us_buffer_t *buffer = 0;
            us_log_debug( "resetting session allocator" );
            us_allocator_reset(us_testutil_session_allocator);
            us_log_debug( "allocating 4k buffer from session" );
            buffer = us_buffer_create(us_testutil_session_allocator, 4096);
            if ( buffer )
            {
                us_log_debug( "testing bundle" );
                r &= do_test_osc_msg_bundle(
                         us_testutil_session_allocator,
                         buffer,
                         us_stdout
                     );
                buffer->destroy( buffer );
            }
            else
            {
                us_log_error( "error allocating buffer" );
            }
        }
        us_logger_finish();
        us_testutil_finish();
    }
    return r ? 0:1;
}

/*@}*/
