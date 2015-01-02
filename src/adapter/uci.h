/* uci.h

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


// uci.h

#ifndef UCI_H
#define UCI_H

// includes

#include "board.h"
#include "engine.h"
#include "line.h"
#include "move.h"
#include "util.h"

namespace adapter {

// constants

const int OptionNb = 256;

// types

struct option_t {
   const char * name;
   const char * value;
};

struct uci_t {

   engine_t * engine;

   const char * name;
   const char * author;

   int option_nb;
   option_t option[OptionNb];

   bool ready;
   int ready_nb;

   bool searching;
   int pending_nb;

   board_t board[1];

   int best_move;
   int ponder_move;

   int score;
   int depth;
   int sel_depth;
   move_t pv[LineSize];

   int best_score;
   int best_depth;
   int best_sel_depth;
   move_t best_pv[LineSize];

   sint64 node_nb;
   double time;
   double speed;
   double cpu;
   double hash;
   move_t current_line[LineSize];

   int root_move;
   int root_move_pos;
   int root_move_nb;
};

enum dummy_event_t {
   EVENT_NONE  = 0,
   EVENT_UCI   = 1 << 0,
   EVENT_READY = 1 << 1,
   EVENT_STOP  = 1 << 2,
   EVENT_MOVE  = 1 << 3,
   EVENT_PV    = 1 << 4,
   EVENT_DEPTH = 1 << 5
};

// variables

extern uci_t Uci[1];

// functions

extern void uci_open              (uci_t * uci, engine_t * engine);
extern void uci_close             (uci_t * uci);

extern void uci_clear             (uci_t * uci);

extern void uci_send_isready      (uci_t * uci);
extern void uci_send_isready_sync (uci_t * uci);
extern void uci_send_stop         (uci_t * uci);
extern void uci_send_stop_sync    (uci_t * uci);
extern void uci_send_ucinewgame   (uci_t * uci);

extern bool uci_option_exist      (uci_t * uci, const char option[]);
extern void uci_send_option       (uci_t * uci, const char option[], const char format[], ...);

extern int  uci_parse             (uci_t * uci, const char string[]);

}  // namespace adapter

#endif // !defined UCI_H

// end of uci.h

