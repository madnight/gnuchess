/* GNU Chess 6 - components.h - Pipes shared across modules

   Copyright (c) 2001-2017 Free Software Foundation, Inc.

   GNU Chess is based on the two research programs
   Cobalt by Chua Kong-Sian and Gazebo by Stuart Cracraft.

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

   Contact Info:
     bug-gnu-chess@gnu.org
     cracraft@ai.mit.edu, cracraft@stanfordalumni.org, cracraft@earthlink.net
*/

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <pthread.h>

/* File descriptors for the pipes to be used to communicate frontend, adapter and engine */

extern int pipefd_i2f[2];

extern int pipefd_f2a[2];
extern int pipefd_a2f[2];

extern int pipefd_a2e[2];
extern int pipefd_e2a[2];

/*
 * Entry point for the adapter thread
 */
void *input_func(void *arg);
void *adapter_func(void *arg);
void *engine_func(void *arg);

/*
 * Starts the adapter (based on Polyglot 1.4) on a separate thread.
 */
void InitInput();
void InitAdapter();
void InitEngine();
void TerminateAdapterEngine();
void TerminateInput();

#endif /* COMPONENTS_H */
