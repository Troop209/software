/**
 * SD Support
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

#ifndef SD_SUPPORT_H
#define SD_SUPPORT_H


/**
 * Configuration file and structure
 * We define the file name and one routine, get
 */
typedef struct {
    int kernelID;
    char label[20];
    char exp_wait_duration[18];
    int defrost_wait_duration;
	char exp_end[18];
	char rtc_start[18];
} SDConfig;

typedef struct {
    char filename[80];
    void (*get)(SDConfig *);
} ConfigFile;

/**
 * Output files have the file name and one routine, write
 */
typedef struct {
    char filename[80];
    void (*write)(char *, int);
    void (*writeln)(char *);
} OutputFile;

extern const OutputFile SDEventFile, SDDataFile;
extern OutputFile SDExtraFile;

extern const ConfigFile SDConfigFile;

extern SDConfig config;

#endif