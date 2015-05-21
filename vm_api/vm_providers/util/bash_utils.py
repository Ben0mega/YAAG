#!/bin/python

import shlex

def bash_safe_join(cmd, stdin_f, stdout_f, stderr_f):
	out = " ".join(shlex.quote(a) for a in cmd)
	if stdin_f is not None:
		out += ' < ' +stdin_f
	if stdout_f is not None:
		out += ' > ' +stdout_f
	if stderr_f is not None:
		out += ' 2> ' +stderr_f
	return out
