#!/bin/python

class ProjectRunner:
	def __init__(self, project, submission):
		self.project = project
		self.submission = submission
		self.test_lr = None
		self.judge_lr = None
		pass

	def grade(self):
		return self.serialGrade()

	# may want to use an iterator here
	def serialGrade(self):
		test_case_data = []
		stat_data = []
		for test in self.project.testcases:
			retcode, sub_stds, sub_other, stats = self.runTestCase(testcase)
			stat_data.append(stats)
			test_case_data.append(
					(
						test.args,
						retcode,
						test.stdin,
						test.extra_files,
						sub_stds,
						test.model.stds,
						sub_other,
						test.model.changed_files
					))
		self.test_lr.end()
		out=[]
		for test_data,stats in zip(test_case_data, stat_data):
			yield stas, self.gradeTestCase(*test_data)
		self.judge_lr.end()

	def parallelGrade(self):
		test_case_data = []
		for test in self.project.testcases:
			retcode, sub_stds, sub_other, stats = self.runTestCase(testcase)
			yield stats, self.gradeTestCase(
						test.args,
						retcode,
						test.stdin,
						test.extra_files,
						sub_stds,
						test.model.stds,
						sub_other,
						test.model.changed_files
					)
		self.test_lr.end()
		self.judge_lr.end()

	def runTestCase(self, testcase):
		# returns returncode, (stdout, stderr), [('changed_file_name', 'changed_file_content')], {'stat_name':'stat_val'}
		pass

	def gradeTestCase(self, args, retcode, stdin, extra_files, sub_stds, model_stds, sub_other, model_other):
		# returns (score, display_msg/stdout, private_msg/stderr)
		# score as a floating value/percent between 0 and 1 - 1 is 100%, 0 is 0%
		# throws error if the return code is not 0
		# with score = Nan or -1
		# with display_msg = 'Error encountered during grading!'
		# and private_msg = stdout + stderr + stats + whatever_else_I_can_think_of_that_could_help_debugging
		# note: we only allow one judge per project, not per test case
		# we could allow multiple judges, but it would be a poor separation of concerns
		# (most test cases need only one judge, and it is easier to make one judge that can
		# do multiple things than upload several judges and keep track of which one goes to which test case)
		pass
