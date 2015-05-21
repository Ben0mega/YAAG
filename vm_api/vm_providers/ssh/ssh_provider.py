#!/bin/python
from vm_api.vm_api import VMSessionProvider
from vm_api.vm_providers.ssh.ssh_session import SSHSession

#TODO: move usernames and passwords to file that will not be tracked

class SSHSessionProvider(VMSessionProvider):
	def __init__(self):
		# is list, example extry is something like:
		# ('localhost', {'port':22, 'username':'foobar', 'password':'supacool'})
		self.available= readSSHConfig()

	def createSession(self):
		return SSHSession(*self.available.pop())

	def endSession(self, session):
		session.cleanup()
		self.available.append((session.host, session.ssh_config))
		session.host = session.ssh_config = None

def readSSHConfig():
	f = open('vm_api/vm_providers/ssh/ssh_config')
	# danger will robinson, danger, danger
	return eval(f.read())


VMSessionProvider.default = SSHSessionProvider()

# currently my ssh users are setup with
# limits.conf limits
# iptables limits (no out bound network connections - still, blocks all incoming except ssh)
#	to make really, really secure, ssh should only accept from this  machine, but idk if it is necessary
# lo is still functional, so users could collaborate over that - to make secure, LD_PRELOAD could be used
# but it is probably easier to just check student code at that point
# small partitions for user writable areas
# quota limits
# users cannot change passwd
