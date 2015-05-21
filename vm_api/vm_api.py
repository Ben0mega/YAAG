#!/bin/python

class VMSessionProvider:
	def createSession(self):
		''' Returns a VMSession'''
		pass

	def endSession(self, session):
		'''Finishes a session'''
		pass

	@classmethod
	def getDefault(cls):
		return cls.default

class VMSession:
	def getDirList(self):
		''' Returns file names and md5sums created or changed as part of the session '''
		pass

	def rmDir(self, name):
		pass

	def putDir(self, name):
		pass

	def getFileList(self):
		''' Returns file names and md5sums created or changed as part of the session '''
		pass

	def getFile(self, name):
		''' Returns file by name '''
		pass

	def putFile(self, name, content):
		''' Puts file by name'''
		pass

	def rmFile(self, name):
		''' Removes file by name or names'''
		pass

	def runCmd(self, command, stdin_file_name=None, stdout_file_name=None, stderr_file_name=None):
		''' Runs command with that stdin, returns job'''
		pass

class VMJob:
	def wait(self):
		''' does not return until the command is finished'''
		pass

	def kill(self):
		'''halts the command'''
		pass

	def getExitCode(self):
		''' if the process is done, returns the exit code
		or None if the process is not done or no return code is available'''
		pass

import vm_api.vm_providers.provider_impl
