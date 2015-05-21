import paramiko
import stat
import os.path
import codecs
from vm_api.vm_api import VMSession,VMJob
from vm_api.vm_providers.util.bash_utils import bash_safe_join

# this class has some insecurities - it would be better
# if we could prevent the user from killing the ssh
# process
# TODO: revisit

class SSHSession(VMSession):
	def __init__(self, host, ssh_config):
		self.writable_dirs=['/var/tmp','/tmp', '/home/'+ssh_config['username']]
		self.host=host
		self.ssh_config = ssh_config
		self.ssh = None
		self.sftp = None
		self._cleanup()

		self.whitelist_dirs = []
		self.whitelist_files = []
		self.whitelist_dirs = self.getDirList()
		self.whitelist_files = self.getFileList()

	def _fullCleanup(self):
		self._cleanup()
		self.runCmd(['/usr/bin/pkill', '-u', self.ssh_config['username'], '-9'])

	def _cleanup(self):
		self.connectSFTP()
		for folder in self.writable_dirs:
			for file_or_dir in self.sftp.listdir(folder):
				file_or_dir = os.path.join(folder, file_or_dir)
				lstat = self.sftp.lstat(file_or_dir)
				try:
					if stat.S_ISDIR(lstat.st_mode):
						self.rmDir(file_or_dir)
					else:
						self.rmFile(file_or_dir)
				except PermissionError:
					#print("Cannot remove: ", file_or_dir)
					pass

	def getDirList(self):
		jb = self.runCmd(['/usr/bin/find'] + self.writable_dirs + ['-type', 'd', '-readable', '-print0'], None, '.find.out')
		jb.wait()
		fl = self.getFile('.find.out').split('\0')
		self.rmFile('.find.out')
		# remove files that were already there
		out = []
		for a in fl:
			if a == '': #dirs cannot have an empty name
				continue
			if a not in self.whitelist_dirs:
				out.append(a)
		return out

	def putDir(self, name):
		self.connectSFTP()
		self.sftp.mkdir(name)

	def rmDir(self, name):
		self.connectSFTP()
		self.sftp.rmdir(name)

	def getFileList(self):
		jb = self.runCmd(['/usr/bin/find'] + self.writable_dirs + ['-type', 'f', '-readable', '-exec', 'md5sum', '{}', ';'], None, '.find.out')
		jb.wait()
		fl = md5Parse(self.getFile('.find.out'))
		self.rmFile('.find.out')
		# remove files that were already there
		out = []
		for a in fl:
			if a[0].endswith('.find.out'): #ignore .find.out
				continue
			if a not in self.whitelist_files:
				out.append(a)
		return out

	def getFile(self,name):
		self.connectSFTP()
		return self.sftp.open(name).read().decode('utf-8')

	def putFile(self, name, content):
		self.connectSFTP()
		print(content, file=self.sftp.open(name, 'wb'))

	def rmFile(self, name):
		self.connectSFTP()
		self.sftp.remove(name)

	def runCmd(self, cmd, stdin_file_name='/dev/null', stdout_file_name='/dev/null', stderr_file_name='/dev/null'):
		self.connectSSH()
		# has dirty trick to get pid
		bash_cmd='echo $$; exec '+bash_safe_join(cmd, stdin_file_name, stdout_file_name, stderr_file_name)
		#print("Command: ", bash_cmd)
		return SSHJob(self.ssh.exec_command(bash_cmd), self)

	def connectSSH(self):
		if self.ssh is not None and self.ssh.get_transport().is_active():
			return
		self.ssh = paramiko.client.SSHClient()
		self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy()) #TODO: review this
		self.ssh.load_system_host_keys()
		self.ssh.connect(self.host,**self.ssh_config)

	def connectSFTP(self):
		self.connectSSH()
		if self.sftp is not None:
			return
		self.sftp = self.ssh.open_sftp()

	def cleanup(self):
		self._fullCleanup()
		self.sftp.close()
		self.ssh.close()

class SSHJob(VMJob):
	def __init__(self, ssh_exec_cmd_tuple, session):
		self.ssh_job_tuple = ssh_exec_cmd_tuple
		self.pid = self.ssh_job_tuple[1].readline()
		self.exit_code = None
		self.session = session

	def wait(self):
		if self.exit_code is not None:
			return
		self.exit_code = self.ssh_job_tuple[1].channel.recv_exit_status()

	def getExitCode(self):
		return self.exit_code

	def kill(self):
		jb = self.session.runCmd(['/usr/bin/kill','-9',self.pid])
		jb.wait()

def md5Parse(strs):
	''' returns a list of tuples of filename, hash of content'''
	lines = strs.split('\n')
	out = []
	for a in lines:
		if a == '':
			continue
		md5sum, filename = a.split('  ', 1)
		if md5sum[0] == '\\':
			md5sum = md5sum[1:]
			filename = codecs.getdecoder('unicode_escape')(filename)[0]
		out.append((filename, md5sum))
	return out

