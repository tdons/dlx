import sys
import os
import glob
from ctypes import *

def _get_libdlx_location():
	"""
	Finding libdlx this way is kind of cumbersome but probably the best, using
	ctypes.util.find_library requires that the library is installed on the
	system for straightforward usage.  We use a glob because the file-naming of
	a dynamic library is different across platforms...
	"""
	cwd = os.path.dirname(os.path.abspath(__file__))
	_glob = os.path.join(cwd, '../build/lib/*dlx*')
	matches = glob.glob(_glob)
	if len(matches) != 1:
		print "error: can't find libdlx, follow build-instructions.txt"
		exit(1)
	return matches[0]

_libdlx_location = _get_libdlx_location()

class _dlx_matrix(Structure):
	pass

class _dlx_iterative_solver(Structure):
	pass

_libdlx = cdll.LoadLibrary(_libdlx_location)

_libdlx.dlx_matrix_create.argtypes = [c_size_t, c_size_t, c_size_t]
_libdlx.dlx_matrix_create.restype = POINTER(_dlx_matrix)

# Actual signature in C is [POINTER(_dlx_matrix), c_void_p], note the difference
# in the 2nd argument.  This is ok (as long as sizeof c_void_p == sizeof c_size_t), it's just an identifier.
_libdlx.dlx_matrix_begin_new_row.argtypes = [POINTER(_dlx_matrix), c_size_t]
_libdlx.dlx_matrix_begin_new_row.restype = None

_libdlx.dlx_matrix_current_row_set_one.argtypes = [POINTER(_dlx_matrix), c_size_t]
_libdlx.dlx_matrix_current_row_set_one.restype = None

_libdlx.dlx_matrix_destroy.argtypes = [POINTER(_dlx_matrix)]
_libdlx.dlx_matrix_destroy.restype = None


_libdlx.dlx_iterative_solver_create.argtypes = [POINTER(_dlx_matrix)]
_libdlx.dlx_iterative_solver_create.restype = POINTER(_dlx_iterative_solver)

_libdlx.dlx_iterative_solver_find_solution.argtypes = [POINTER(_dlx_iterative_solver)]
_libdlx.dlx_iterative_solver_find_solution.restype = c_bool

_libdlx.dlx_iterative_solver_num_solution_rows.argtypes = [POINTER(_dlx_iterative_solver)]
_libdlx.dlx_iterative_solver_num_solution_rows.restype = c_size_t
 
_libdlx.dlx_iterative_solver_get_solution_row.argtypes = [POINTER(_dlx_iterative_solver), c_size_t]
# Actual return type in C is c_void_p, note the difference (we use c_size_t here).
_libdlx.dlx_iterative_solver_get_solution_row.restype = c_size_t

_libdlx.dlx_iterative_solver_destroy.argtypes = [POINTER(_dlx_iterative_solver)]
_libdlx.dlx_iterative_solver_destroy.restype = None

_libdlx.dlx_recursive_solver_count_solutions.argtypes = [POINTER(_dlx_matrix)]
_libdlx.dlx_recursive_solver_count_solutions.restype = c_size_t

__all__ = ["dlx_matrix", "dlx_iterative_solver"]
