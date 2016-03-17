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

#define TAG "WRAP"
#define WRAP "MP3WRAP"
#define WRAPL "mp3wrap"
#define ID3LEN 460
#define PADDING 4
#define INDEXVERSION 1

unsigned char charat (char *s, int pos);

unsigned long flength (FILE *in);

int getid3v1 (FILE *file_input);

int cindexsize (int files, char *argv[], short optoffset);

char *buildindex (char *wrapindex, int indexsize, int files, char *argv[], short optoffset, unsigned long splitpoints[], short addoption, int oldfiles);

unsigned long getid3v2 (FILE *in, unsigned long start);

unsigned long getword (FILE *in, unsigned long offset, int mode);

char **exchange (char **argv, int offset, int tot);
