#include "instruments.h"

VALUE rb_mInstruments;

void
Init_instruments(void)
{
  rb_mInstruments = rb_define_module("Instruments");
}
