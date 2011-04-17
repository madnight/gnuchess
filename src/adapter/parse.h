/* parse.h

   GNU Chess protocol adapter

   Copyright (C) 2001-2011 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


// parse.h

#ifndef PARSE_H
#define PARSE_H

// includes

#include "util.h"

namespace adapter {
  
// constants

const int STAR_NUMBER = 16;
const int KEYWORD_NUMBER = 256;

// types

struct parse_t {
   const char * string;
   int pos;
   int keyword_nb;
   const char * keyword[KEYWORD_NUMBER];
};

// variables

extern char * Star[STAR_NUMBER];

// functions

extern bool match             (char string[], const char pattern[]);

extern void parse_open        (parse_t * parse, const char string[]);
extern void parse_close       (parse_t * parse);

extern void parse_add_keyword (parse_t * parse, const char keyword[]);

extern bool parse_get_word    (parse_t * parse, char string[], int size);
extern bool parse_get_string  (parse_t * parse, char string[], int size);

}  // namespace adapter

#endif // !defined PARSE_H

// end of parse.h

