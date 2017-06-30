#include "instruments.h"

VALUE rb_mInstruments;

struct signpost_args {
  VALUE self;
  VALUE code;
  VALUE arg1;
  VALUE arg2;
  VALUE arg3;
  VALUE arg4;
};

static VALUE
signpost_start(VALUE self, VALUE code, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4)
{
  return INT2NUM(kdebug_signpost_start(NUM2UINT(code), NUM2ULL(arg1), NUM2ULL(arg2), NUM2ULL(arg3), NUM2ULL(arg4)));
}

static VALUE
signpost_end(VALUE self, VALUE code, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4)
{
  return INT2NUM(kdebug_signpost_end(NUM2UINT(code), NUM2ULL(arg1), NUM2ULL(arg2), NUM2ULL(arg3), NUM2ULL(arg4)));
}

static VALUE
block_invoke(VALUE arg)
{
  struct signpost_args *args = (struct signpost_args *)arg;
  signpost_start(args->self, args->code, args->arg1, args->arg2, args->arg3, args->arg4);
  return rb_yield(Qundef);
}

static VALUE
block_ensure(VALUE arg) {
  struct signpost_args *args = (struct signpost_args *)arg;
  signpost_end(args->self, args->code, args->arg1, args->arg2, args->arg3, args->arg4);
  return Qnil;
}

// FIXME: Assuming that this is a 64bit arch where uintptr_t is unsigned long long.
static VALUE
signpost(VALUE self, VALUE code, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4)
{
  if (rb_block_given_p()) {
    struct signpost_args args;
    args.self = self;
    args.code = code;
    args.arg1 = arg1;
    args.arg2 = arg2;
    args.arg3 = arg3;
    args.arg4 = arg4;
    return rb_ensure(block_invoke, (VALUE)&args, block_ensure, (VALUE)&args);
  } else {
    return INT2NUM(kdebug_signpost(NUM2UINT(code), NUM2ULL(arg1), NUM2ULL(arg2), NUM2ULL(arg3), NUM2ULL(arg4)));
  }
}

void
Init_instruments(void)
{
  rb_mInstruments = rb_define_module("Instruments");
  rb_define_singleton_method(rb_mInstruments, "signpost", signpost, 5);
  rb_define_singleton_method(rb_mInstruments, "signpost_start", signpost_start, 5);
  rb_define_singleton_method(rb_mInstruments, "signpost_end", signpost_end, 5);
}
