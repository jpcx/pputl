.build/lang/eat.o: lang/eat.cc lang/eat.h codegen.h codegen.tcc
.build/lang/va_rest.o: lang/va_rest.cc lang/va_rest.h codegen.h \
 codegen.tcc
.build/lang/va_first.o: lang/va_first.cc lang/va_first.h codegen.h \
 codegen.tcc config/build.h
.build/codegen.o: codegen.cc codegen.h codegen.tcc
.build/main.o: main.cc codegen.h codegen.tcc
.build/config/uint_max.o: config/uint_max.cc config/uint_max.h codegen.h \
 codegen.tcc
.build/config/build.o: config/build.cc config/build.h codegen.h \
 codegen.tcc
