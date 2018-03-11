from ctypes import *

from pydlx import _libdlx

def count_solutions(matrix):
	return _libdlx.dlx_recursive_solver_count_solutions(matrix._get_matrix())