/* common.c - Contains functions needed for a number of barcodes */

/*
    libzint - the open source barcode library
    Copyright (C) 2008 Robin Stuart <robin@zint.org.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define SSET	"0123456789ABCDEF"

const int ustrlen(unsigned char data[]) {
	/* Local replacement for strlen() with unsigned char strings */
	int i;
	for (i=0;data[i];i++);

	return i;
}

void ustrcpy(unsigned char target[], unsigned char source[]) {
	/* Local replacement for strcpy() with unsigned char strings */
	int i, len;

	len = ustrlen(source);
	for(i = 0; i < len; i++) {
		target[i] = source[i];
	}
	target[i] = '\0';
}

void concat(char dest[], char source[])
{ /* Concatinates dest[] with the contents of source[], copying /0 as well */
	unsigned int i, j;

	j = strlen(dest);
	for(i = 0; i <= strlen(source); i++) {
		dest[i + j] = source[i]; }
}

void uconcat(unsigned char dest[], unsigned char source[])
{ /* Concatinates dest[] with the contents of source[], copying /0 as well */
	unsigned int i, j;

	j = ustrlen(dest);
	for(i = 0; i <= ustrlen(source); i++) {
		dest[i + j] = source[i]; }
}


int ctoi(char source)
{ /* Converts a character 0-9 to its equivalent integer value */
	if((source >= '0') && (source <= '9'))
		return (source - '0');
	return(source - 'A' + 10);
}

char itoc(int source)
{ /* Converts an integer value to its hexadecimal character */
	if ((source >= 0) && (source <= 9)) {
		return ('0' + source); }
	else {
		return ('A' + (source - 10)); }
}

void to_upper(unsigned char source[])
{ /* Converts lower case characters to upper case in a string source[] */
	unsigned int i;

	for (i = 0; i < ustrlen(source); i++) {
		if ((source[i] >= 'a') && (source[i] <= 'z')) {
			source [i] = (source[i] - 'a') + 'A'; }
	}
}

int is_sane(char test_string[], unsigned char source[])
{ /* Verifies that a string only uses valid characters */
	unsigned int i, j, latch;

	for(i = 0; i < ustrlen(source) - 1; i++) {
		latch = FALSE;
		for(j = 0; j < strlen(test_string); j++) {
			if (source[i] == test_string[j]) { latch = TRUE; } }
		if (!(latch)) { 
			return ERROR_INVALID_DATA; }
	}
	
	return 0;
}

int posn(char set_string[], char data)
{ /* Returns the position of data in set_string */
	unsigned int i;

	for(i = 0; i < strlen(set_string); i++) {
		if (data == set_string[i]) { return i; } }
	return 0;
}

void lookup(char set_string[], char *table[], char data, char dest[])
{ /* Replaces huge switch statements for looking up in tables */
	unsigned int i;

	for(i = 0; i < strlen(set_string); i++) {
		if (data == set_string[i]) { concat(dest, table[i]); } }
}

int module_is_set(struct zint_symbol *symbol, int y_coord, int x_coord)
{
	int x_char, x_sub, result;
	
	x_char = x_coord / 7;
	x_sub = x_coord % 7;
	result = 0;
	
	switch(x_sub) {
		case 0: if((symbol->encoded_data[y_coord][x_char] & 0x01) != 0) { result = 1; } break;
		case 1: if((symbol->encoded_data[y_coord][x_char] & 0x02) != 0) { result = 1; } break;
		case 2: if((symbol->encoded_data[y_coord][x_char] & 0x04) != 0) { result = 1; } break;
		case 3: if((symbol->encoded_data[y_coord][x_char] & 0x08) != 0) { result = 1; } break;
		case 4: if((symbol->encoded_data[y_coord][x_char] & 0x10) != 0) { result = 1; } break;
		case 5: if((symbol->encoded_data[y_coord][x_char] & 0x20) != 0) { result = 1; } break;
		case 6: if((symbol->encoded_data[y_coord][x_char] & 0x40) != 0) { result = 1; } break;
	}
	
	return result;
}

void set_module(struct zint_symbol *symbol, int y_coord, int x_coord)
{
	if(module_is_set(symbol, y_coord, x_coord)) { return; }
	
	int x_char, x_sub;
	
	x_char = x_coord / 7;
	x_sub = x_coord % 7;
	
	switch(x_sub) {
		case 0: symbol->encoded_data[y_coord][x_char] += 0x01; break;
		case 1: symbol->encoded_data[y_coord][x_char] += 0x02; break;
		case 2: symbol->encoded_data[y_coord][x_char] += 0x04; break;
		case 3: symbol->encoded_data[y_coord][x_char] += 0x08; break;
		case 4: symbol->encoded_data[y_coord][x_char] += 0x10; break;
		case 5: symbol->encoded_data[y_coord][x_char] += 0x20; break;
		case 6: symbol->encoded_data[y_coord][x_char] += 0x40; break;
	} /* The last binary digit is reserved for colour barcodes */
}

void unset_module(struct zint_symbol *symbol, int y_coord, int x_coord)
{
	if(!(module_is_set(symbol, y_coord, x_coord))) { return; }
	
	int x_char, x_sub;
	
	x_char = x_coord / 7;
	x_sub = x_coord % 7;
	
	switch(x_sub) {
		case 0: symbol->encoded_data[y_coord][x_char] -= 0x01; break;
		case 1: symbol->encoded_data[y_coord][x_char] -= 0x02; break;
		case 2: symbol->encoded_data[y_coord][x_char] -= 0x04; break;
		case 3: symbol->encoded_data[y_coord][x_char] -= 0x08; break;
		case 4: symbol->encoded_data[y_coord][x_char] -= 0x10; break;
		case 5: symbol->encoded_data[y_coord][x_char] -= 0x20; break;
		case 6: symbol->encoded_data[y_coord][x_char] -= 0x40; break;
	} /* The last binary digit is reserved for colour barcodes */
}

void expand(struct zint_symbol *symbol, char data[])
{ /* Expands from a width pattern to a bit pattern */
	
	int reader, writer, i;
	char latch;
	
	writer = 0;
	latch = '1';
	
	for(reader = 0; reader < strlen(data); reader++) {
		for(i = 0; i < ctoi(data[reader]); i++) {
			if(latch == '1') { set_module(symbol, symbol->rows, writer); }
			writer++;
		}
		if(latch == '1') {
			latch = '0';
		} else {
			latch = '1';
		}
	}
	
	if(symbol->symbology != BARCODE_PHARMA) {
		if(writer > symbol->width) {
			symbol->width = writer;
		}
	} else {
		/* Pharmacode One ends with a space - adjust for this */
		if(writer > symbol->width + 2) {
			symbol->width = writer - 2;
		}
	}
	symbol->rows = symbol->rows + 1;
}

int is_stackable(int symbology) {
	/* Indicates which symbologies can have row binding */
	if(symbology < BARCODE_PDF417) { return 1; }
	if(symbology == BARCODE_CODE128B) { return 1; }
	if(symbology == BARCODE_ISBNX) { return 1; }
	if(symbology == BARCODE_EAN14) { return 1; }
	if(symbology == BARCODE_NVE18) { return 1; }
	if(symbology == BARCODE_KOREAPOST) { return 1; }
	if(symbology == BARCODE_PLESSEY) { return 1; }
	if(symbology == BARCODE_TELEPEN_NUM) { return 1; }
	if(symbology == BARCODE_ITF14) { return 1; }
	if(symbology == BARCODE_CODE32) { return 1; }
	
	return 0;
}

int is_extendable(int symbology) {
	/* Indicates which symbols can have addon */
	if(symbology == BARCODE_EANX) { return 1; }
	if(symbology == BARCODE_UPCA) { return 1; }
	if(symbology == BARCODE_UPCE) { return 1; }
	if(symbology == BARCODE_ISBNX) { return 1; }
	if(symbology == BARCODE_UPCA_CC) { return 1; }
	if(symbology == BARCODE_UPCE_CC) { return 1; }
	if(symbology == BARCODE_EANX_CC) { return 1; }
	
	return 0;
}

int roundup(float input)
{
	float remainder;
	int integer_part;
	
	integer_part = (int)input;
	remainder = input - integer_part;
	
	if(remainder > 0.1) {
		integer_part++;
	}
	
	return integer_part;
}

