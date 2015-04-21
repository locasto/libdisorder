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
#include <sys/stat.h> // to get file size in bytes for buffer
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


static void
do_usage()
{
  fprintf(stderr,
	  "ropy [filename]\n");
  fprintf(stderr,
	  "ropy -v [filename]\n");
}

/**
 * Open and read a file, filling a buffer. After filing the buffer,
 * calculate the entropy of the buffer. Output the entropy of the file
 * in bits.
 *
 * An attacker could cause this program to read in a large number of
 * bytes or allocate a large amount of memory by supplying a large
 * file.
 * 
 * Usage:
 *  ./ropy [-v] [file]
 */
int
main(int argc, 
     char* argv[])
{
  float entropy_value = 0.0;
  char* filename = NULL;
  int fildes = -1;
  FILE* fin=NULL;
  struct stat stat_fd;
  long long file_size;
  char* buffer = NULL;
  unsigned char c;
  int x = 0;
  char* itr=NULL;
  //long long i = 0;
  long long how_many_bytes_read = 0L;
  int ret = 0;
  int verbose = 0; //be quiet by default

  if(2==argc)
  {
    //point filename to argv[1]; no copying.
    filename = argv[1];
  }else if(3==argc){
    if(0==strncmp(argv[1],"-v",2))
    {
      verbose = 1;
      filename = argv[2];
    }else{
      do_usage();
    }
  }else if(1==argc){
    //assume stdin is being piped to us
    //how to handle fstat in this case?
  }else{
    do_usage();
    return -1;
  }

  fin = fopen(filename, "r");
  if(NULL==fin)
  {
    perror("main(): problem opening file");
    return -1;
  }

  fildes = fileno(fin);

  ret = fstat(fildes, &stat_fd);
  if(-1==ret)
  {
    perror("main(): problem invoking fstat() of file");
  }

  /* Handle regular files and block devices differently */
  if (S_ISREG(stat_fd.st_mode)) {
    file_size = (long long)stat_fd.st_size;
  } else if (S_ISBLK(stat_fd.st_mode)) {
    /*
     * st_size does not work for block devices
     * seek to the end to determine size
     */
    file_size = (long long)lseek(fildes, 0, SEEK_END);
    lseek(fildes, 0, SEEK_SET); /* Seek back to the beginning */
  }

  if(verbose)
    fprintf(stdout,
	    "file size: %lld (bytes)\n",
	    file_size);

  buffer = (char*)calloc(file_size, sizeof(char));
  if(NULL==buffer)
  {
    perror("main(): problem allocating buffer");
    return -2;
  }

  //read the file into the buffer
  itr = buffer;
  //for(i=0;i<file_size;i++) //could also read until EOF
  while(EOF!=(x=fgetc(fin)))
  {
    //x = fgetc(fin);
    c = (unsigned char)x;
    *itr = c;
    itr++;
    how_many_bytes_read++;
  }
  if(verbose)
    fprintf(stdout,
	    "read %lld bytes\n",
	    how_many_bytes_read);

  entropy_value = shannon_H(buffer, how_many_bytes_read);
  fprintf(stdout,
	  "tokens: %d entropy: %f metric: %f maxent: %f ratio: %f\n",
	  get_num_tokens(),
	  entropy_value,
          entropy_value/how_many_bytes_read,
	  get_max_entropy(),
	  get_entropy_ratio()
	  );

  free(buffer);
  buffer=NULL;
  ret = fclose(fin);
  if(EOF==ret)
    perror("exiting main()");
  fin=NULL;

  return 0;
}
