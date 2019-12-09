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
/* Compiler preprocessor flags to support large files: */
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE

#include "../include/disorder.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULTBLOCKSIZE   256

static void
do_usage()
{
  fprintf(stderr,
	  "tropy [filename]\n\n");
  fprintf(stderr,
	  "Will print entropy per BLOCKSIZE for file.\n");
  fprintf(stderr,
	  "BLOCKSIZE default is %d.\n", DEFAULTBLOCKSIZE);
  fprintf(stderr,
	  "Set BLOCKSIZE environment variable to change.\n");
  fprintf(stderr,
	  "Use filename \"-\" for stdin.\n");
}


/**
 * Open and read file, filling a buffer of size BLOCKSIZE. After
 * filing the buffer, calculate the entropy of the buffer. Repeat
 * util EOF.
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
  FILE* fin = NULL;
  char* itr = NULL;
  int i = 0;
  char* filename = NULL;
  int BLOCKSIZE = DEFAULTBLOCKSIZE;

  if(2==argc)
  {
    filename = argv[1];
  } else {
    do_usage();
    return -1;
  }
  
  if (getenv("BLOCKSIZE")!=NULL)
  {
    BLOCKSIZE = atoi(getenv("BLOCKSIZE"));
  }

  if (strcmp(filename,"-")) 
  {
    fin = fopen(filename,"r");
  }
  else 
  {
    fin = stdin;
  }
  
  if(NULL==fin)
  {
    perror("main():");
    return -1;
  }

  buffer = (char*)calloc(BLOCKSIZE, sizeof(char));
  if(NULL==buffer)
  {
    perror("main():");
    return -2;
  }

  for(;;)
  {
    itr = buffer;
    for(i=0;i<BLOCKSIZE;i++)
    {
      x = fgetc(fin);
      if (x==EOF)
          return 0;
      c = (unsigned char)x;
      *itr=c;
      itr++;
    }
    entropy_value = shannon_H(buffer, BLOCKSIZE);
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
