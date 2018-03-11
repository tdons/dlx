from ctypes import *

from pydlx import _libdlx

class dlx_matrix:
	__dlx_matrix = None

	def __init__(self, num_primary_columns, num_secondary_columns, num_ones):
		self.__dlx_matrix = _libdlx.dlx_matrix_create(c_size_t(num_primary_columns), c_size_t(num_secondary_columns), c_size_t(num_ones))

	def __enter__(self):
		return self

	def __exit__(self, exc_type, exc_value, traceback):
		_libdlx.dlx_matrix_destroy(self.__dlx_matrix)
		self.__dlx_matrix = None

	def _get_matrix(self):
		return self.__dlx_matrix

	def add_row(self, column_indices, user_data):
		_libdlx.dlx_matrix_begin_new_row(self.__dlx_matrix, c_size_t(user_data))
		for index in column_indices:
			_libdlx.dlx_matrix_current_row_set_one(self.__dlx_matrix, c_size_t(index))

	def reset(self):
		self.libdlx.dlx_matrix_reset(self.__dlx_matrix)
