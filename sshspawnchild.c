/* sshspawnchild.c,v 1.51 2015/12/11 00:20:04 mmcc Exp $ */
/*
 * Copyright (c) 2001 Markus Friedl.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "sshspawnchild.h"

 /*
 * spawn a child process
 * - specified by cmd with agruments argv
 * - with std handles set to in, out, err
 * - flags are passed to CreateProcess call
 *
 * cmd will be internally decoarated with a set of '"'
 * to account for any spaces within the commandline
 * this decoration is done only when additional arguments are passed in argv
 */
int
spawn_child(char** arglist, int* pin, int* pout, int err, unsigned long flags)
{
	int ret = -1;
	char *file;
	int pid = fork();
	file = arglist[0];
	if (pid == 0) {
		/* Child. */
		close(pin[1]);
		close(pout[0]);
		dup2(pin[0], 0);
		dup2(pout[1], 1);
		close(pin[0]);
		close(pout[1]);

		execvp(file, arglist);
		perror(file);
		exit(1);
	}
	return ret;
}
