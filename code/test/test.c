/***************************************************************************
 *  libdisorder: A Library for Measuring Byte Stream Entropy
 *  Copyright (C) 2010 Michael E. Locasto
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the:
 *       Free Software Foundation, Inc.
 *       59 Temple Place, Suite 330
 *       Boston, MA  02111-1307  USA
 *
 * $Id$
 **************************************************************************/

#include "../include/disorder.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LIMIT   LIBDO_BUFFER_LEN

/**
 * Open and read /dev/urandom, filling a buffer of size LIMIT. After
 * filing the buffer, calculate the entropy of the buffer. Repeat
 * forever.
 *
 */
int
main(int argc, 
     char* argv[])
{
  float entropy_value = 0.0;
  long long current_run = 0L;
  char* buffer = NULL;
  unsigned char c;
  int x = 0;
  FILE* fin=NULL;
  char* itr=NULL;
  int i = 0;

  fin = fopen("/dev/urandom","r");
  if(NULL==fin)
  {
    perror("main():");
    return -1;
  }

  buffer = (char*)calloc(LIMIT, sizeof(char));
  if(NULL==buffer)
  {
    perror("main():");
    return -2;
  }

  for(;;)
  {
    itr = buffer;
    for(i=0;i<LIMIT;i++)
    {
      x = fgetc(fin);
      c = (unsigned char)x;
      *itr=c;
      itr++;
    }
    //NB: doing this caluculation here may allow /dev/unrandom to
    //recover sufficiently to never show a decrease in entropy.
    //Using a larger buffer or using another thread to perform this
    //calculation while the main thread continues to fill the buffer
    //may provide the ability to observe a decrease in entropy.
    entropy_value = shannon_H(buffer, LIMIT);
    fprintf(stdout,
	    "[%8lld] entropy: %f maxent: %f ratio: %f\n",
	    current_run,
	    entropy_value,
	    get_max_entropy(),
	    get_entropy_ratio()
	    );
    current_run++;
    fflush(stdout);
  }

  free(buffer);
  buffer=NULL;
  if(EOF!=fclose(fin))
    perror("exiting main():");
  fin=NULL;

  return 0;
}
