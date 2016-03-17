/*
 * Mp3Wrap -- Utility for mp3 wrapping
 *
 * Copyright (c) 2002 M. Trotta - <matteo.trotta@lib.unimib.it>
 *
 * http://mp3wrap.sourceforge.net
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Tested on: Linux 2.4.18 #4 Fri May 31 01:25:31 PDT 2002 i686
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "wrap.h"
#include "mp3wrap.h"
#include "crc.h"

unsigned char charat (char *s, int pos)
{
	if (pos<strlen(s))
		return s[pos];
	else return 0xFF;
}

int getid3v1 (FILE *file_input) {
	fseek(file_input, -128, SEEK_END);
	if (fgetc(file_input)=='T')
	   if (fgetc(file_input)=='A')
	      if (fgetc(file_input)=='G')
			return -128;
	return 0;
}

unsigned long flength (FILE *in)
{
	fseek(in, 0, SEEK_END);
	return ftell (in);
}

int cindexsize (int files, char *argv[], short optoffset)
{
	int i, size=0;
	for (i=0; i<files; i++)
		size += strlen(argv[i+2+optoffset])+1;
	return (size + strlen(TAG)+(files+2)*4 + PADDING + CRCLEN);
}

char *buildindex (char *wrapindex, int indexsize, int files, char *argv[], short optoffset, unsigned long splitpoints[], short addoption, int oldfiles)
{
	int i, j, k;
	char c;
	j = 12;

	strncpy(wrapindex, TAG, strlen(TAG));
	wrapindex[4]=charat(VERSION, 0);
	wrapindex[5]=charat(VERSION, 2);
	wrapindex[6]=INDEXVERSION;
	wrapindex[7]=files + oldfiles;
	files +=oldfiles;
	for (i=0; i<=files; i++) {
		wrapindex[j++] = ((splitpoints[i])>> 24) & 0xFF ;
		wrapindex[j++] = ((splitpoints[i])>> 16) & 0xFF;
		wrapindex[j++] = ((splitpoints[i])>> 8) & 0XFF;
		wrapindex[j++] = (splitpoints[i]) & 0XFF;
		if (i<files) {
			k=0;
			if ((addoption) && (i<oldfiles))
				while (wrapindex[j++]!='\0');
			else {
				do {
					c=argv[i+2+optoffset-oldfiles][k++];
					wrapindex[j++] = c;
				}
				while (c!=0x00);
			}
		}
	}

	if (j!=indexsize-PADDING) {
		fprintf (stderr, "Error: index build error!\n");
		exit(-1);
	}

	return wrapindex;
}

unsigned long getid3v2 (FILE *in, unsigned long start)
{
	char c1, c2, c3, c4;
	fseek(in, start, SEEK_SET);
	if (fgetc(in)=='I')
		if (fgetc(in)=='D')
			if (fgetc(in)=='3') {
				fseek(in, 3, SEEK_CUR);
				c1 = fgetc(in);
				c2 = fgetc(in);
				c3 = fgetc(in);
				c4 = fgetc(in);
				return c1*2097151+c2*16384+c3*128+c4;
			}

	return 0;
}

unsigned long getword (FILE *in, unsigned long offset, int mode)
{
	int i; unsigned long head=0;
	fseek (in, offset, mode);
	for (i=0; i<4; i++) {
		head = head << 8;
		head |= fgetc(in);
	}
	return head;
}

char **exchange (char **argv, int offset, int tot)
{
	char **first = &argv[offset];
	while (offset < tot) {
		*first = argv[offset+1];
		first++;
		offset++;
	}
	
	return argv;
}
