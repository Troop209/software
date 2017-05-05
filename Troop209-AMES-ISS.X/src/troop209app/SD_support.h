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
 * We have so far defined three things:
 *   1. An event log (this is the general error and info log)
 *   2. A data log (this is our CSV output file)
 *   3. A configuration file (this is an input INI file)
 */

#ifndef SD_SUPPORT_H
#define SD_SUPPORT_H


/**
 * Configuration file and structure
 * We define the file name and one routine, get
 */
typedef struct {
    int kernelID;
	char motor;
	char camera;
	char sensors;
	char date[11];
	char time[9];
    char label[20];
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