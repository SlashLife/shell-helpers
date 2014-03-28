/*
Copyright (c) 2014, Simon Stienen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char * const g_size_units[] =
	{"", "k", "M", "G", "T", "P", "E", "Z", "Y", NULL};

typedef const char * const *pargv_t;

int format_size(int argc, pargv_t argv, size_t num_switches) {
	int factor = 1024;
	int limit = 0;

	pargv_t begin, first, end;
	for(begin=argv, end=argv+num_switches; begin < end; ++begin) {
		if (
			!strcmp(*begin, "--size=bin") ||
			!strcmp(*begin, "--size=binary")
		) {
			factor = 1024;
		}
		else if (
			!strcmp(*begin, "--size=dec") ||
			!strcmp(*begin, "--size=decimal") ||
			!strcmp(*begin, "--size=si")
		) {
			factor = 1000;
		}
		else if (
			!strcmp(*begin, "--size") ||
			!strcmp(*begin, "--")
		) {
			// ignore
		}
		else {
			fprintf(stderr, "Invalid switch: %s\n", *begin);
			return EXIT_FAILURE;
		}
	}

	if (limit <= 0) {
		limit = factor;
	}

	for(begin=argv+num_switches, first=begin, end=argv+argc; begin < end; ++begin) {
		if (begin != first) putchar(' ');

		const char *num = *begin;
		if (*num == '-') ++num;
		while(isdigit(*num)) ++num;
		
		if (*num == '\0') {
			num = *begin; /* again! */
			if (*num == '-') {
				putchar('-');
				++num;
			}
			unsigned long long number = atoll(num);
			const char * const *unit = g_size_units;
			while(factor <= number && unit[1]) {
				number /= factor;
				++unit;
			}
			printf("%llu%s", number, *unit);
		}
		else {
			fputs(*begin, stdout);
		}
	}

	return EXIT_SUCCESS;
}

int main(int argc, const char **argv) {
	/* ignore program name */
	--argc;
	++argv;

	/* count switches (possibly add choice of algorithm lateron) */
	pargv_t begin, end;
	for(begin=argv, end=argv+argc; begin < end; ++begin) {
		assert(*begin && " argv entries must not be NULL");
		if ((*begin)[0] != '-') {
			break;
		}
		else if ((*begin)[1] == '-' && (*begin)[2] == '\0') {
			++begin;
			break;
		}
	}

	return format_size(argc, argv, begin-argv);
}
