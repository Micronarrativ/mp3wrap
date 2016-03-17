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
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "mp3wrap.h"
#include "wrap.h"
#include "crc.h"

unsigned char id3[ID3LEN] = {
	0x49, 0x44, 0x33, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x42, 0x54, 0x52, 0x43, 0x4B, 0x00, 0x00,
	0x00, 0x03, 0x00, 0x00, 0x00, 0x30, 0x30, 0x54, 0x45, 0x4E, 0x43, 0x00, 0x00, 0x00, 0x11, 0x40,
	0x00, 0x00, 0x4D, 0x70, 0x33, 0x57, 0x72, 0x61, 0x70, 0x20, 0x20, 0x76, 0x2E, 0x20, 0x20, 0x30, '.', 0x30,
	0x57, 0x58, 0x58, 0x58, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x68, 0x74, 0x74, 0x70,
	0x3A, 0x2F, 0x2F, 0x6D, 0x70, 0x33, 0x77, 0x72, 0x61, 0x70, 0x2E, 0x73, 0x6F, 0x75, 0x72, 0x63,
	0x65, 0x66, 0x6F, 0x72, 0x67, 0x65, 0x2E, 0x6E, 0x65, 0x74, 0x54, 0x43, 0x4F, 0x50, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x54, 0x4F, 0x50, 0x45, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x54, 0x43, 0x4F, 0x4D, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x4F, 0x4D, 0x4D, 0x00,
	0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x65, 0x20, 0x00, 0x54, 0x68, 0x69, 0x73, 0x20, 0x66,
	0x69, 0x6C, 0x65, 0x20, 0x69, 0x73, 0x20, 0x77, 0x72, 0x61, 0x70, 0x70, 0x65, 0x64, 0x20, 0x77,
	0x69, 0x74, 0x68, 0x20, 0x4D, 0x70, 0x33, 0x57, 0x72, 0x61, 0x70, 0x2E, 0x20, 0x47, 0x65, 0x74,
	0x20, 0x6D, 0x70, 0x33, 0x73, 0x70, 0x6C, 0x74, 0x2C, 0x20, 0x74, 0x68, 0x65, 0x20, 0x66, 0x72,
	0x65, 0x65, 0x20, 0x74, 0x6F, 0x6F, 0x6C, 0x20, 0x74, 0x6F, 0x20, 0x73, 0x70, 0x6C, 0x69, 0x74,
	0x20, 0x6F, 0x72, 0x69, 0x67, 0x69, 0x6E, 0x61, 0x6C, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x73, 0x20,
	0x61, 0x74, 0x20, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x6D, 0x70, 0x33, 0x73, 0x70, 0x6C,
	0x74, 0x2E, 0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x66, 0x6F, 0x72, 0x67, 0x65, 0x2E, 0x6E, 0x65,
	0x74, 0x2E, 0x20, 0x50, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x20, 0x64, 0x6F, 0x20, 0x6E, 0x6F, 0x74,
	0x20, 0x72, 0x65, 0x6D, 0x6F, 0x76, 0x65, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x63, 0x6F, 0x6D,
	0x6D, 0x65, 0x6E, 0x74, 0x2E, 0x54, 0x43, 0x4F, 0x4E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x54, 0x59, 0x45, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x54, 0x41, 0x4C, 0x42, 0x00,
	0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x6D, 0x70, 0x33,
	0x73, 0x70, 0x6C, 0x74, 0x2E, 0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x66, 0x6F, 0x72, 0x67, 0x65,
	0x2E, 0x6E, 0x65, 0x74, 0x54, 0x50, 0x45, 0x31, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x46,
	0x69, 0x6C, 0x65, 0x20, 0x77, 0x72, 0x61, 0x70, 0x70, 0x65, 0x64, 0x20, 0x77, 0x69, 0x74, 0x68,
	0x20, 0x4D, 0x70, 0x33, 0x57, 0x72, 0x61, 0x70, 0x54, 0x49, 0x54, 0x32, 0x00, 0x00, 0x00, 0x26,
	0x00, 0x00, 0x00, 0x55, 0x73, 0x65, 0x20, 0x4D, 0x70, 0x33, 0x53, 0x70, 0x6C, 0x74, 0x20, 0x74,
	0x6F, 0x20, 0x6F, 0x62, 0x74, 0x61, 0x69, 0x6E, 0x20, 0x6F, 0x72, 0x69, 0x67, 0x69, 0x6E, 0x61,
	0x6C, 0x20, 0x74, 0x72, 0x61, 0x63, 0x6B, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	} ;

int main (int argc, char *argv[]) {

	FILE *file_output, *file_input;
	unsigned long splitpoints[MAXNUMFILE+1], begin=0, end, len=0, crc, fcrc;
	unsigned char filename[512], *wrapindex, c, ext[512];
	char option, *ptr;
	int i = 0, j = 0, files, oldfiles = 0, indexsize=0, id3offset=0;
	short optoffset = 0, addoption = 0, listoption = 0, verboption = 0, ismp3wrap = 0;

	printf (NAME" Version "VERSION" "DATE". See README and COPYING for more!\n");
	printf ("Written and copyrights by "AUTHOR" - "EMAIL"\n");
	printf ("THIS SOFTWARE COMES WITH ABSOLUTELY NO WARRANTY! USE AT YOUR OWN RISK!\n");

	while ((option=getopt(argc, argv, "lav"))!=EOF) {
	  switch (option) {

		 case 'l': if (optoffset==0) optoffset=1;
				listoption=1;
				addoption=0;
				break;

		 case 'a': if (optoffset==0) optoffset=1;
				addoption=1;
				listoption=0;
				break;

		 case 'v': if (optoffset==0) optoffset=1;
				verboption=1;
				break;

		default: fprintf (stderr, "Run without arguments to see HELP. Read man page for complete documentation.\n");
			      exit(-1);
			      break;
	  }
	}

	if (((!listoption) && (argc<4)) || ((listoption) && (argc < 3))) {
		printf ("HELP INFORMATION - type \"man mp3wrap\" to see complete documentation\n");
		printf ("USAGE\n");
		printf ("\tmp3wrap [options] OUTPUTFILE MP3FILE1 MP3FILE2 [MP3FILE3]...\n");
		printf ("\nDESCRIPTION\n");
		printf ("\tMp3Wrap joins two or more mp3 files in one large playable mp3\n");
		printf ("\twithout losing filename and ID3 informations. You will obtain\n");
		printf ("\ta file named OUTPUTFILE_MP3WRAP.mp3. Do not remove the MP3WRAP\n");
		printf ("\tstring because it's useful to let split program to detect that\n");
		printf ("\tfile is wrapped with this utility.\n");
		printf ("\tTO SPLIT FILE USE Mp3Splt: http://mp3splt.sourceforge.net\n");
		printf ("\nOPTIONS\n");
		printf ("\t-a  Add the specified files to an existing wrap file\n");
		printf ("\t-l  List files wrapped in OUTPUTFILE. (-lv for complete infos)\n");
		printf ("\t-v  Verbose mode. Will display additional informations.\n");
		printf ("\nEXAMPLES\n");
		printf ("\tmp3wrap album.mp3 01.mp3 02.mp3 03.mp3 04.mp3\n");
		printf ("\tmp3wrap -a album_MP3WRAP.mp3 ACDC*.mp3\n");
		exit(-1);
	}

	memset(filename, '\0', 512);

	if ((ptr=getenv("HOME"))!=NULL) sprintf(filename, "%s/"CONFFILE, ptr);
	else strncpy(filename, CONFFILE, strlen(CONFFILE));

	strncpy(ext, EXT, 511);

	if ((file_input=fopen(CONFFILE, "r"))||(file_input=fopen(filename, "r"))) {
		if (verboption) printf ("Reading configuration file...\n");
		while ((fgets(filename, 511, file_input))!=NULL) {
			if ((ptr=strstr(filename, "EXT="))!=NULL) {
				if (strlen(filename)>1) {
					if (filename[strlen(filename)-1] == '\n') filename[strlen(filename)-1] = '\0';
					if (filename[strlen(filename)-1] == '\r') filename[strlen(filename)-1] = '\0';
					ptr += 4;
					if ((strstr(ptr, WRAP)==NULL)&&(strstr(ptr, WRAPL)==NULL)) {
						if (verboption) printf ("Warning: your extension does not contain "WRAP" string!\n");
						sprintf (ext, "_"WRAP"_%s", ptr);
					}
					else strncpy(ext, ptr, 511);
					if (verboption) printf ("Using \"%s\" as extension.\n", ext);
					break;
				}
			}
		}
		fclose(file_input);
	}

	files = (argc - 2 - optoffset);

	for (i=0; i<files; i++) {
		if (opendir(argv[i+2+optoffset])) {
			printf ("mp3wrap: omitting %s directory\n", argv[i+2+optoffset]);
			exchange ((char **) argv, i+2+optoffset, files+2+optoffset);
			files--;
			i--;
		}
	}

	if (files>MAXNUMFILE) {
		fprintf (stderr, "Error: too many files to wrap!\n");
		exit(-1);
	}

	indexsize = cindexsize(files, argv, optoffset);
	if ((wrapindex = malloc (indexsize))==NULL) {
		perror("malloc");
		exit(-1);
	}
	memset (wrapindex, 0x0, indexsize);
	memset (filename, 0x0, 512);

	if (addoption || listoption) {
		if (!(file_input=fopen(argv[2], "r"))) {
			perror(argv[2]);
			exit(-1);
		}
		len = flength(file_input);
		if (len == 0) {
			fprintf (stderr, "Error: file is empty!\n");
			fclose(file_input);
			exit(-1);
		}
		id3offset = getid3v2(file_input, 0);
		fseek(file_input, id3offset, SEEK_SET);
		for (i=0; i<16384; i++) {
			id3offset = ftell (file_input);
			if (fgetc(file_input)=='W')
			  if (fgetc(file_input)=='R')
			    if (fgetc(file_input)=='A')
				if (fgetc(file_input)=='P') {
					ismp3wrap = 1;
					break;
				}
		}
		if (ismp3wrap) {
			fseek (file_input, 2, SEEK_CUR);
			if (((c=fgetc(file_input))<INDEXVERSION)&&addoption) {
				fprintf (stderr, "Warning: old index version. Updating to new version...\n");
			}
			else if (c>INDEXVERSION) {
				fprintf (stderr, "Error: This version of mp3wrap is too old for this file.\nGet the latest version at http://mp3wrap.sourceforge.net!\n");
				fclose(file_input);
				exit(-1);
			}
			oldfiles = fgetc(file_input);
			if (c > 0x0) {
				fcrc = getword (file_input, 0, SEEK_CUR);
				begin = ftell(file_input);
				fseek(file_input, getid3v1(file_input), SEEK_END);
				end = ftell(file_input);
				fprintf (stderr, "Check for file integrity: calculating CRC, please wait... ");
				crc = c_crc (file_input, begin, end);
				if (crc != fcrc) {
					fprintf (stderr, "\nWARNING: Bad CRC. File might be damaged. Continue anyway? (y/n) ");
					if (getchar()!='y')
						exit(1);
				}
				else fprintf (stderr, "OK\n");
				fseek(file_input, begin, SEEK_SET);
			}
			begin = getword (file_input, 0, SEEK_CUR);
			splitpoints[0] = begin;
			if (addoption) {
				if ((files + oldfiles)>MAXNUMFILE) {
					fprintf (stderr, "Error: too many file to wrap in this file!\n");
					exit(-1);
				}
				indexsize = indexsize + begin - PADDING - (strlen(TAG) + 8);
				if (c > 0x0) indexsize -= CRCLEN;
				wrapindex = realloc (wrapindex, indexsize);
				memset (wrapindex, 0x0, indexsize);
				j = 12;
			}
			else {
				printf ("List of wrapped files in %s:\n", argv[2]);
				if (verboption) {
					printf ("  #    Size\t  Name\n");
					printf (" --- --------\t--------\n");
				}
				else printf("\n");
			}
			for (i = 1; i<=oldfiles; i++) {
				if (addoption)
					j = j + 4;
				else {
					j = 0;
					if (verboption) {
						if (i<100) printf (" ");
						if (i<10) printf (" ");
					}
				}
				do {
					c = fgetc(file_input);
					if (addoption)
						wrapindex[j++] = c;
					else filename[j++] = c;

				} while (c!='\0');

				splitpoints[i] = getword(file_input, 0, SEEK_CUR);

				if (listoption) {
					if (verboption) printf ("%d) %lu\t", i, splitpoints[i]-splitpoints[i-1]);
					printf ("%s\n", filename);
				}
			}
			fclose(file_input);
			if (listoption)
			{
				if (verboption) {
					printf (" --- --------\t--------\n");
					printf ("     %lu\t%d files\n", splitpoints[i-1], oldfiles);
				}
				else printf ("\n");
				exit(0);
			}
		}
		else {
			fprintf (stderr, "Error: this is not a valid mp3wrap file!\n");
			exit(-1);
		}
	}

	if (addoption) {
		end = indexsize - splitpoints[0];
		for (i=0; i<=oldfiles; i++)
			splitpoints[i] += end;
	}

	for (i=0; i<files; i++) {
			if (!(file_input=fopen(argv[i+2+optoffset], "r"))) {
				perror(argv[i+2+optoffset]);
				exit(-1);
			}
			if ((!addoption) && (i==0)) splitpoints[i] = indexsize;

			splitpoints[i+1+oldfiles]=flength(file_input) + splitpoints[i+oldfiles];

			fclose(file_input);
	}

	wrapindex = buildindex (wrapindex, indexsize, files, argv, optoffset, splitpoints, addoption, oldfiles);

	if (!addoption) {
		strncpy (filename, argv[1+optoffset], strlen(argv[1+optoffset]));
		if (strstr(filename, ext)==NULL) {
			if ((strlen(filename)>4) && ((strstr(filename, ".mp3"))!=NULL))
				filename[strlen(filename)-4]='\0';
			strncat (filename, ext, strlen(ext));
		}
		if ((file_input=fopen(filename, "r"))) {
			fprintf (stderr, "Warning: %s exists, overwrite ? (y/n) ", filename);
			fclose(file_input);
			if (getchar()!='y')
				exit(-1);
		}

	}
	else
		strncpy (filename, TEMPFILE, strlen(TEMPFILE));

	if (!(file_output=fopen(filename, "w+"))) {
	        perror(argv[1+optoffset]);
		exit(-1);
	}

	if (files + oldfiles > 9) id3[21] = (((files + oldfiles) / 10) % 10) | 0x30;
	id3[22] = ((files + oldfiles) % 10) | 0x30;
	id3[47] = charat(VERSION, 0);
	id3[49] = charat(VERSION, 2);

	for (i=0; i<ID3LEN; i++)
		fputc(id3[i], file_output);

	for (i=0; i<indexsize; i++)
		fputc(wrapindex[i], file_output);

	printf ("\n");

	if (addoption) {
		if (!(file_input=fopen(argv[2], "r"))) {
			perror(argv[2]);
			exit(-1);
		}
		fseek (file_input, id3offset + begin, SEEK_SET);
		printf ("Copying files from input file...  0%%");
		fflush(stdout);
		for (begin=begin + id3offset; begin<len-1; begin++) {
			if ((begin%(len/100))==0) {
				if ((begin/(len/100))>9)
					printf ("\b");
				printf ("\b\b%ld%%", begin/(len/100));
				fflush(stdout);
			}
			fputc(fgetc(file_input), file_output);
		}
		fclose(file_input);
		printf ("\n\n");
	}

	for (i=0; i<files; i++) {

		printf ("  %d %%\t-->", ((i+1)*100)/files);
		if (addoption)
			printf (" Adding ");
		else
			printf (" Wrapping ");
		printf ("%s ...", argv[i+2+optoffset]);
		fflush(stdout);
		if (!(file_input=fopen(argv[i+2+optoffset], "r"))) {
			fprintf (stderr, " FAILED\n");
			perror(argv[i+2+optoffset]);
			exit(-1);
		}

		begin=0;
		end = flength(file_input);
		fseek (file_input, 0, SEEK_SET);
		while (begin++<end)
			fputc(fgetc(file_input), file_output);
		printf (" OK\n");
		fclose (file_input);
	}

	printf ("\n  Calculating CRC, please wait... ");
	fflush(stdout);

	fputc(0x00, file_output);

	len = ftell(file_output);
	crc = c_crc (file_output, ID3LEN+12, len);
	fseek (file_output, ID3LEN+8, SEEK_SET);
	fputc (((crc >> 24) & 0xFF), file_output);
	fputc (((crc >> 16) & 0xFF), file_output);
	fputc(((crc >> 8) & 0xFF), file_output);
	fputc((crc & 0xFF), file_output);

	fclose(file_output);
	
	printf ("OK\n");

	if (addoption) {
		memset(wrapindex, 0x00, indexsize);
		sprintf (wrapindex, "mv -f "TEMPFILE" %s", argv[2]);
		if (system (wrapindex)==-1)
			fprintf (stderr, "Error: could not rename "TEMPFILE"! Do it manually.\n");
		strncpy(filename, argv[2], strlen(argv[2])+1);
	}

	printf ("\n%s has been created successfully!\nUse mp3splt to dewrap file; download at http://mp3splt.sourceforge.net!\n", filename);
	free(wrapindex);
	return 0;
}
