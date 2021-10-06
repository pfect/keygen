/*
 * keygen - simple example using PCG for random generation
 * Copyright (C) 2021 Pasi Patama, <pasi@patama.net>
 * 
 * Based on PCG examples (pcg64-demo.c)
 * 
 * PCG Random Number Generation for C.
 *
 * Copyright 2014-2017 Melissa O'Neill <oneill@pcg-random.org>,
 *                     and the PCG Project contributors.
 *
 * SPDX-License-Identifier: (Apache-2.0 OR MIT)
 *
 * Licensed under the Apache License, Version 2.0 (provided in
 * LICENSE-APACHE.txt and at http://www.apache.org/licenses/LICENSE-2.0)
 * or under the MIT license (provided in LICENSE-MIT.txt and at
 * http://opensource.org/licenses/MIT), at your option. This file may not
 * be copied, modified, or distributed except according to those terms.
 *
 * Distributed on an "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied.  See your chosen license for details.
 *
 * For additional information about the PCG random number generation scheme,
 * visit http://www.pcg-random.org/.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>




#include "pcg_variants.h"
#include "entropy.h"                    /* Wrapper around /dev/random */
#include "log.h"

int main(int argc, char** argv)
{
    uint64_t rounds = 0;
	pcg64_random_t rng;
	char *filename=NULL;
	char *stopstring;
	
	/* Command line options */
	int c;
	while ((c = getopt (argc, argv, "r:f:h")) != -1)
	switch (c)
	{
		case 'r':
			rounds = strtoul(optarg, &stopstring, 0); 
			break;		
		case 'f':
			filename = optarg;
			break;
		case 'h':
			log_info("[%d] keygen usage: keygen -f [filename] -r [rounds]", getpid() );
			log_info("[%d] Each round outputs 8 bytes.", getpid() );
			return 1;
			break;
		default:
			break;
	}
	if ( rounds == 0  ) 
	{
		log_error("[%d] missing rounds ", getpid() );
		log_info("[%d] keygen usage: keygen -f [filename] -r [rounds]", getpid() );
		log_info("[%d] Each round outputs 8 bytes.", getpid() );
		return 1;
	}
	if ( filename == NULL )
	{
		log_error("[%d] missing filename ", getpid() );
		log_info("[%d] keygen usage: keygen -f [filename] -r [rounds]", getpid() );
		log_info("[%d] Each round outputs 8 bytes.", getpid() );
		return 1;
	}

	log_info("[%d] Generating %ld rounds to %s ", getpid(),rounds,filename);
		
    /* You should *always* seed the RNG.  The usual time to do it is the
       point in time when you create RNG (typically at the beginning of the
       program). */

    /* pcg64_srandom_r takes two 128-bit constants (the initial state, and the
       rng sequence selector; rngs with different sequence selectors will
       *never* have random sequences that coincide, at all) - the code below
       shows three possible ways to do so. */

	pcg128_t seeds[2];
	entropy_getbytes((void*)seeds, sizeof(seeds));
	pcg64_srandom_r(&rng, seeds[0], seeds[1]);

		/* File write */	
		FILE *fptr;
		if ((fptr = fopen(filename,"wb")) == NULL){
		   log_error("[%d] error opening file", getpid() );
		   exit(1);
		}

		for (uint64_t round = 1; round <= rounds; ++round) {
			uint64_t number = pcg64_random_r(&rng);
			fwrite(&number, sizeof(number), 1, fptr); 
		}
		fclose(fptr);
	
	log_info("[%d] Wrote %ld bytes to %s ", getpid(),rounds * 8,filename); 
    return 0;
}
