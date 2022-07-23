from os import path
import lit.formats

config.name = 'Triangles testing'
config.test_format = lit.formats.ShTest(True)

config.suffixes = ['.tst']

config.test_source_root = path.dirname(__file__)
config.test_exec_root = path.join(config.my_obj_root, 'bin/test')

config.substitutions.append(('%lvl1', path.join(config.my_obj_root, 'bin/lvl1')))
config.substitutions.append(('%fc', 'FileCheck-11 --allow-empty --match-full-lines'))
