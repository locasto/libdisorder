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

#include <math.h> //for log2()
#include <stdio.h> //for NULL
#include "../include/disorder.h"
#include "../config.h"

#if !(HAVE_LOG2)
#define        log2(x) (log((x)) * (1./M_LN2))
#endif

/**
 * Set m_num_tokens by iterating over m_token_freq[] and maintaining
 * a counter of each position that does not hold the value of zero.
 */
static void
count_num_tokens(struct entropy_ctl *ctl)
{
  int i = 0;
  int counter = 0;
  for(i=0;i<LIBDO_MAX_BYTES;i++)
    if(ctl->m_token_freqs[i])
      counter++;
  ctl->m_num_tokens = counter;
  return;
}

/**
 * Sum frequencies for each token (i.e., byte values 0 through 255)
 * We assume the `length' parameter is correct.
 *
 * This function is available only to functions in this file.
 */
static void
get_token_frequencies(struct entropy_ctl *ctl,
		      char* buf,
		      long long length)
{
  int i=0;
  char* itr=NULL;
  unsigned char c=0;

  itr = buf;

  //reset number of tokens
  ctl->m_num_tokens = 0;

  //make sure freqency and probability arrays are cleared
  for(i=0;i<LIBDO_MAX_BYTES;i++)
  {
    ctl->m_token_freqs[i] = 0;
    ctl->m_token_probs[i] = 0.0;
  }

  for(i=0;i<length;i++)
  {
    c = (unsigned char)*itr;
    //assert(0<=c<LIBDO_MAX_BYTES);
    ctl->m_token_freqs[c]++;
    itr++;
  }
}

/**
 * Return entropy (in bits) of this buffer of bytes. We assume that the
 * `length' parameter is correct. This implementation is a translation
 * of the PHP code found here:
 *
 *    http://onlamp.com/pub/a/php/2005/01/06/entropy.html
 *
 * with a helpful hint on the `foreach' statement from here:
 *
 *    http://php.net/manual/en/control-structures.foreach.php
 */
float
shannon_H(struct entropy_ctl *ctl, char* buf, long long length)
{
  int i = 0;
  float bits = 0.0;
  char* itr=NULL; //values of itr should be zero to 255
  unsigned char token;
  int num_events = 0; //`length' parameter
  float freq = 0.0; //loop variable for holding freq from m_token_freq[]
  float entropy = 0.0; //running entropy sum

  if(NULL==buf || 0==length)
    return 0.0;

  itr = buf;
  ctl->m_maxent = 0.0;
  ctl->m_ratio = 0.0;
  num_events = length;
  get_token_frequencies(ctl, itr, num_events); //modifies m_token_freqs[]
  //set m_num_tokens by counting unique m_token_freqs entries
  count_num_tokens(ctl);

  if(ctl->m_num_tokens > LIBDO_MAX_BYTES)
  {
    //report error somehow?
    return 0.0;
  }

  //iterate through whole m_token_freq array, but only count
  //spots that have a registered token (i.e., freq>0)
  for(i=0;i<LIBDO_MAX_BYTES;i++)
  {
    if(ctl->m_token_freqs[i])
    {
      token = i;
      freq = ((float)ctl->m_token_freqs[token]);
      ctl->m_token_probs[token] = (freq / ((float)num_events));
      entropy += ctl->m_token_probs[token] * log2(ctl->m_token_probs[token]);
    }
  }

  bits = -1.0 * entropy;
  ctl->m_maxent = log2(ctl->m_num_tokens);
  ctl->m_ratio = bits / ctl->m_maxent;

  return bits;
}
