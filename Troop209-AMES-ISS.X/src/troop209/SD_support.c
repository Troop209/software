/**
 * SD Card Support
 * Copyright (C) 2016 Pathway Council - Troop 209
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "system.h"
#include "SD_support.h"
#include <fsio.h>

static void process(char *, SDConfig *);
static char *trim(char *);


/**
 * This module defines all the support objects for the SD Card.
 * 
 *   1. An event log (this is the general error and info log)
 *        SDEventFile - EventFile.txt
 *   2. A data log (this is our CSV output file)
 *        SDDataFile - DataFile.txt
 *   3. An extra output file (you can change the file name on this)
 *        SDExtraFile - defaults to ExtraFIle.txt
 *   4. A configuration file (this is an input INI file)
 *        SDConfig - config.ini
 * 
 * Output files have two functions: write and writeln.  writeln is just like
 * write except is also writes an additional CR/LF sequence.
 * 
 * Files are opened, written to and closed.  If the file does not exist it is
 * created.  To write to an ad hoc file, use SDExtraFile and change the 
 * filename value prior to using it.
 * 
 * SDConfigFile has one function, get, which reads and parses the config.ini
 * file and stores those values in the SDConfig structure.  To add new values,
 * change the structure here and the get structure in SD_support.c.
 */

// Definitions for output file support
// Simple routines: open the file, append datam close the file
static void write(OutputFile file, char *outBuffer, int length) {
    FSFILE* outFile = FSfopen(file.filename, "a");
    if (outFile) {
        FSfwrite(outBuffer, 1, length, outFile);
        FSfclose(outFile);
    }
}

static void writeln(OutputFile file, char *outLine) {
    FILE* outFile = FSfopen(file.filename, "a");
    if (outFile) {
        FSfwrite(outLine, 1, strlen(outLine), outFile);
        FSputc('\r', outFile);
        FSputc('\n', outFile);
        FSfclose(outFile);
    }
}

// Member routines - one for each file, redirecting to the correct version
// First, the write routines
static void write1(char *outBuffer, int length) {
    write( SDEventFile, outBuffer, length);
}

static void write2(char *outBuffer, int length) {
    write( SDDataFile, outBuffer, length);
}

static void write3(char *outBuffer, int length) {
    write( SDExtraFile, outBuffer, length);
}

// Next, the writeln routines
static void writeln1(char *outLine) {
    writeln( SDEventFile, outLine);
}

static void writeln2(char *outLine) {
    writeln( SDDataFile, outLine);
}

static void writeln3(char *outLine) {
    writeln( SDExtraFile, outLine);
}


// Now the config file routines; really, only one - get the config
static void getConfig(ConfigFile file, SDConfig *pcfg) {

    #define MAX_LINES 20
    char *lines[MAX_LINES];
    int i;

	FILE *inifile = FSfopen(file.filename, "r");
	int length = FSfread(sysBuffer, 1, SYS_BUFFER_SIZE, inifile);
    FSfclose(inifile);
	sysBuffer[length] = 0;

    // Parse entire file by \n into lines array
	i = 0;
    lines[i] = strtok(sysBuffer, "\n");
    while (i < MAX_LINES) {
        if (!(lines[++i] = strtok( NULL, "\n"))) break;
    }

    // Process lines
    int count = i;
	for (i = 0; i < count; i++)
	{
		switch(lines[i][0]) {
		case '#': break;
		case '$': puts("Done!"); i = count; break;
		default:
			process(lines[i], pcfg);
		}

	}
}

void process(char *line, SDConfig *pcfg) {
	char *key = trim(strtok(line, "="));
	char *value = trim(strtok(NULL, "\n"));
	if (!key || !value) return;
	printf("[%s]=[%s]\n", key, value);
	if (strcmp(key, "kernelId") == 0) pcfg->kernelID = atoi(value);
	if (strcmp(key, "label") == 0) strncpy( pcfg->label, value, sizeof(pcfg->label));
	if (strcmp(key, "exp_wait_duration") == 0) strncpy( pcfg->exp_wait_duration, value, sizeof(pcfg->exp_wait_duration));
	if (strcmp(key, "defrost_wait_duration") == 0) pcfg->defrost_wait_duration = atoi(value);
	if (strcmp(key, "exp_end") == 0) strncpy( pcfg->exp_end, value, sizeof(pcfg->exp_end));
	if (strcmp(key, "rtc_start") == 0) strncpy( pcfg->rtc_start, value, sizeof(pcfg->rtc_start));
}

char *trim(char *src) {
	if (!src) return (src);
    while (isspace((unsigned char)*src)) ++src;
    char *end = src + strlen(src) - 1;
    while (end > src && isspace((unsigned char)*end)) *end-- = 0;
    return (src);
}


void get1(SDConfig *pcfg) {
	getConfig(SDConfigFile, pcfg);
}

// These output files have constant file names
const OutputFile
    SDEventFile = { filename : "EventFile.txt", write : write1, writeln: writeln1  },
    SDDataFile = { filename : "DataFile.txt", write : write2, writeln: writeln2 };

// SDExtraFile filename can be changed
OutputFile
    SDExtraFile = { filename : "ExtraFile.txt", write : write3, writeln: writeln3 };

// Configuration file
const ConfigFile
	SDConfigFile = { filename : "config.ini", get : get1 };

// Utility functions
Boolean CheckFile(char *filename) {
    FSFILE *file = FSfopen( filename, FS_READ);
    if (file != NULL) {
        FSfclose( file);
        return ON;
    }
    return OFF;
}
