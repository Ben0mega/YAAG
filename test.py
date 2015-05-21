import code_runner_api.language_runner
from collections import namedtuple


sub = namedtuple('sub', 'files dirs lang')
m = sub(files=[('temp.cpp', '#include<iostream>\n\nint main(){\n\tstd::cout << "Hi World!\\n";\n}\n')], dirs=[], lang='g++')
l = code_runner_api.language_runner.LanguageRunner(m)

jb = l.build('hello')
jb2 = l.run('hello')

print(jb2.getStdOut())
l.end()

