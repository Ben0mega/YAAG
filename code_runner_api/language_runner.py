#!/bin/python
import vm_api.vm_api
from code_runner_api.languages_supported import languages

class LanguageRunner:
	def __init__(self, submission):
		self.run_counter = 0 # used to make sure stdin/stdout/stderr files are unique
		self.submission = submission
		self.lang = languages[submission.lang]
		self.run_session = vm_api.vm_api.VMSessionProvider.getDefault().createSession()
		self.build_dirs= []
		self.build_files= []

	def getBuildFiles(self):
		''' undefined behavior if called before build'''
		return self.build_files

	def getRunFiles(self):
		''' undefined behavior if called before build
		or before all run jobs have finished'''
		fils = self.run_session.getFileList()
		out = []
		for a in fils:
			if a in self.getBuildFiles():
				continue
			out.append(a)
		return out

	def getFile(self, name):
		return self.run_session.getFile(name)

	def end(self):
		vm_api.vm_api.VMSessionProvider.getDefault().endSession(self.run_session)

	# don't allow build_dirs or run_dirs right now.
	# could be added...
	def build(self, prog, args=[], extra_build_files=[], stdin_content= None):
		''' builds the executable with the extra build files, and stdin,
		stdin is the content.  returns job for build or None if there is none'''
		for name in self.submission.dirs:
			self.run_session.mkDir(name)
		for name, content in self.submission.files:
			self.run_session.putFile(name, content)

		stdin_name = None
		stdout_name = '.std.out.'+str(self.run_counter)
		stderr_name = '.std.err.'+str(self.run_counter)
		if stdin_content is not None:
			self.run_session.putFile('.std.in', stdin_content)
			stdin_name = '.std.in.'+str(self.run_counter)
		self.run_counter+=1

		# extra build files over write submission and stdin files
		for name, content in extra_build_files:
			self.run_session.putFile(name, content)

		jb = None
		if self.lang['build_command'] is not None:
			cmd = substitute(self.lang['build_command'], ARGS=args, FILES=[ name for name, cont in self.submission.files], PROG=prog)
			cmd = stat_run_env(cmd, self.run_counter-1)
			jb = self.run_session.runCmd(cmd, stdin_name, stdout_name, stderr_name)
			jb = Job(jb, self, [], self.run_counter-1)
			jb.wait()

		self.build_dirs += self.run_session.getDirList()
		self.build_files += self.run_session.getFileList()
		return jb

	def run(self, prog, args=[], extra_run_files=[], stdin_content=None):
		''' runs the executable with the extra run files, and stdin,
		stdin is the content.  returns job for run'''
		stdin_name = None
		stdout_name = '.std.out.'+str(self.run_counter)
		stderr_name = '.std.err.'+str(self.run_counter)
		if stdin_content is not None:
			self.run_session.putFile('.std.in', stdin_content)
			stdin_name = '.std.in.'+str(self.run_counter)
		self.run_counter+=1

		# extra build files over write submission and stdin files
		for name, content in extra_run_files:
			self.run_session.putFile(name, content)

		cmd = substitute(self.lang['run_command'], ARGS=args, FILES=[ name for name, cont in self.submission.files], PROG=prog)
		cmd = stat_run_env(cmd, self.run_counter-1)
		jb = self.run_session.runCmd(cmd, stdin_name, stdout_name, stderr_name)
		jb = Job(jb, self, self.build_files, self.run_counter-1)
		return jb

	def clear_to_build(self):
		''' undefined if there is a run job that has not completed'''
		fl = self.run_session.getFileList()
		dl = self.run_session.getDirList()

		# verify that we did not lose any files
		for a in self.build_dirs:
			if a not in dl:
				self.clear_to_initial()
				self.build()
				return
		for a in self.build_files:
			if a not in fl:
				self.clear_to_initial()
				self.build()
				return

		# if all the files from build are here (and more)
		# remove the 'extra' files/dirs
		for name, hashs in fl:
			if (name, hashs) in self.build_files:
				continue
			self.run_session.rmFile(name)
		for name in dl:
			if name in self.build_dirs:
				continue
			self.run_session.rmDir(name)

	def clear_to_initial(self):
		''' undefined if there is a run job that has not completed'''
		for name, hashs in self.run_session.getFileList():
			self.run_session.rmFile(name)
		for name, hashs in self.run_session.getDirList():
			self.run_session.rmDir(name)
		self.build_dirs= []
		self.build_files= []

class Job:
	def __init__(self, session_job, lang_runner, ign_files,  std_count):
		self.jb = session_job
		self.lr = lang_runner
		self.stats = None
		self.ign_files = ign_files
		self.stderr = None
		self.stdout = None
		self.std_count=str(std_count)

	def wait(self):
		self.jb.wait()
		self._initStats()

	def kill(self):
		self.jb.kill()

	def getStat(self, stat_name):
		self._initStats()
		return self.stats[stat_name]

	def getExitCode(self):
		return self.getReturnCode()

	def getReturnCode(self):
		#using ptrace's response
		return int(self.getStat('return_code'))

	# in seconds
	def getRuntimeWall(self):
		#using gettimeofday
		return float(self.getStat('wall_time_sec'))+10**-6*float(self.getStat('wall_time_usec'))

	def getRuntimeCPU(self):
		#using perf stuff for now
		return float(self.getStat('task_clock'))*10**-9

	# in kilobytes
	def getMaxMemory(self):
		#using getrusage stuff for now
		return int(self.getStat('ru_maxrss'))

	def _initStats(self):
		if self.stats is not None:
			return
		if self.jb.getExitCode() is None:
			self.wait()
			return
		fils = self.getFileContent('.stat.out.'+self.std_count)
		self.lr.run_session.rmFile('.stat.out.'+self.std_count)
		self.stats = {}
		for a in fils.split('\n'):
			if a== '' or a=='\n':
				continue
			key, val = a.split(':', 1)
			self.stats[key]=val

	# currently no changed dirs since, I expect it
	# to be an unusual case - could add it
	def getChangedFiles(self):
		self.wait()
		fils = self.lr.run_session.getFileList()
		out = []
		for a in fils:
			if a not in self.ign_files:
				out.append(a)
		return out

	def getStdOut(self):
		if self.stdout is None:
			self.stdout = self._getAndRmFile('.std.out.'+self.std_count)
		return self.stdout

	def getStdErr(self):
		if self.stderr is None:
			self.stderr = self._getAndRmFile('.std.out.'+self.std_count)
		return self.stderr

	def _getAndRmFile(self, name):
		out = self.getFileContent(name)
		self.lr.run_session.rmFile(name)
		return out

	def getFileContent(self, name):
		return self.lr.run_session.getFile(name)

def substitute(lists, **kwargs):
	out = []
	for item in lists:
		for rewrite in kwargs.keys():
			rewrite = rewrite
			if '$'+rewrite not in item:
				continue
			if str(kwargs[rewrite]) == kwargs[rewrite]:
				item = item.replace('$'+rewrite, kwargs[rewrite])
			else:
				pre, post = item.split('$'+rewrite,1)
				for possible in kwargs[rewrite]:
					if possible.startswith(pre) and possible.endswith(post):
						out.append(possible)
				item = None
			break
		if item is not None:
			out.append(item)
	return out

assert(substitute(['g++', '$FILES.cpp', '$ARGS', '$PROG.exe'], FILES=['match.cpp', 'match.c'], ARGS=['a','b','c'], PROG='stuff')==
		['g++', 'match.cpp', 'a', 'b', 'c', 'stuff.exe']
		)

def stat_run_env(cmd, count):
	return ['/usr/bin/stat_finder', '-f', '.stat.out.'+str(count)] + cmd
