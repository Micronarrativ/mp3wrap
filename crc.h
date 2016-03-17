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

#define CRCLEN 4

unsigned long c_crc (FILE *in, unsigned long begin, unsigned long end);
