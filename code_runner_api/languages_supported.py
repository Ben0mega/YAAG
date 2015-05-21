#!/bin/python

# none of the languages support ARGS for build, which I may
# change in the future

languages = {
	'g++': {
		'lang': 'C++',
		'variant': 'g++',
		'build_command': ['g++', '-O3', '$FILES.cpp', '$FILES.cxx', '$FILES.cc', '-o', '$PROG.exe'],
		'run_command': ['./$PROG.exe', '$ARGS'],
	},
	'gcc': {
		'lang': 'C',
		'variant': 'gcc',
		'build_command': ['gcc', '-O3', '$FILES.c', '-o', '$PROG.exe'],
		'run_command': ['./$PROG.exe', '$ARGS'],
	},
	'clang': {
		'lang': 'C',
		'variant': 'clang',
		'build_command': ['clang', '-O3', '$FILES.c', '-o', '$PROG.exe'],
		'run_command': ['./$PROG.exe', '$ARGS'],
	},
	'clang++': {
		'lang': 'C++',
		'variant': 'clang++',
		'build_command': ['clang++', '-O3', '$FILES.c', '-o', '$PROG.exe'],
		'run_command': ['./$PROG.exe', '$ARGS'],
	},
	'openjdk': {
		'lang': 'Java',
		'variant': 'openjdk',
		'build_command': ['javac', '$FILES.java'],
		'run_command': ['java', '$PROG', '$ARGS'],
	},
	'cpython2': {
		'lang': 'Python2',
		'variant': 'cpython',
		'build_command': None,
		'run_command': ['python2', '$PROG.py', '$ARGS'],
	},
	'cpython3': {
		'lang': 'Python3',
		'variant': 'cpython',
		'build_command': None,
		'run_command': ['python3', '$PROG.py', '$ARGS'],
	},
	'bash': {
		'lang': 'bash',
		'variant': 'gnu_bash',
		'build_command': None,
		'run_command': ['bash', '$PROG.sh', '$ARGS'],
	},
}
