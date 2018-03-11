from ctypes import *

from pydlx import _libdlx

class dlx_iterative_solver:
	__dlx_iterative_solver = None

	def __init__(self, matrix):
		self.__dlx_iterative_solver = _libdlx.dlx_iterative_solver_create(matrix._get_matrix())

	def __enter__(self):
		return self

	def __exit__(self, exc_type, exc_value, traceback):
		_libdlx.dlx_iterative_solver_destroy(self.__dlx_iterative_solver)
		self.__dlx_iterative_solver = None

	def get_next_solution(self):
		found_solution = _libdlx.dlx_iterative_solver_find_solution(self.__dlx_iterative_solver)
		if not found_solution:
			return False
		return [self.get_row(r) for r in xrange(self.num_solution_rows())]

	def num_solution_rows(self):
		return _libdlx.dlx_iterative_solver_num_solution_rows(self.__dlx_iterative_solver)

	def get_row(self, row_index):
		return _libdlx.dlx_iterative_solver_get_solution_row(self.__dlx_iterative_solver, c_size_t(row_index))
