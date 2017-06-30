require "mkmf"

have_header("sys/kdebug_signpost.h")

create_makefile("instruments/instruments")
