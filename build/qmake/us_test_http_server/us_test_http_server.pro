! include( ../common.pri ) {
  error( need common.pri file )
}


TARGET=us_test_http_server

SOURCES += $$TOPDIR/tests/us_test_http_server.c

HEADERS += $$TOPDIR/include/*.h

