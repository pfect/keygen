# 
# PCG Random Number Generation for C.
# 
# Copyright 2014-2017 Melissa O'Neill <oneill@pcg-random.org>,
#                     and the PCG Project contributors.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#
# Licensed under the Apache License, Version 2.0 (provided in
# LICENSE-APACHE.txt and at http://www.apache.org/licenses/LICENSE-2.0)
# or under the MIT license (provided in LICENSE-MIT.txt and at
# http://opensource.org/licenses/MIT), at your option. This file may not
# be copied, modified, or distributed except according to those terms.
#
# Distributed on an "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, either
# express or implied.  See your chosen license for details.
#
# For additional information about the PCG random number generation scheme,
# visit http://www.pcg-random.org/.
#
# -std=c99

TARGETS = keygen

LDFLAGS  += -Lsrc
LDLIBS   += -lpcg_random
CPPFLAGS += -Iinclude -Iextras
CFLAGS   += -Wall

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)
	rm -rf actual

extras/entropy.o: extras/entropy.c include/pcg_variants.h \
  extras/pcg_spinlock.h extras/entropy.h

keygen: keygen.o log.o extras/entropy.o
keygen.o: keygen.c log.c include/pcg_variants.h extras/entropy.h
