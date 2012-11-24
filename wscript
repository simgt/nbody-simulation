#! /usr/bin/env python

import glob

APPNAME = 'Experiments'
VERSION = '0.1'

top = '.'
out = 'build'

def options(ctx):
	ctx.load('compiler_cxx compiler_c')
	ctx.add_option(
		'-m', '--mode',
		action			= 'store',
		default			= 'debug',
		help			= 'build mode, release or debug'
	)

def configure(ctx):
	ctx.load('compiler_cxx compiler_c')

	ctx.check(
		uselib_store	= 'OpenCL',
		framework_name	= 'OpenCL'
	)

	# GLFW / OpenGL
	ctx.check(
		uselib_store    = 'GLFW',
		lib             = 'glfw',
		framework       = ['Cocoa', 'OpenGL', 'IOKit']
	)
	ctx.check(
		uselib_store    = 'Eigen',
		header_name     = 'Eigen/Eigen'
	)

def build(ctx):
	cxxflags = {
		'release': ['-O4', '-w', '-DNDEBUG'],
		'debug': ['-O0', '-W', '-Wall', '-g']
	}
	ctx.env.append_unique('CXXFLAGS', cxxflags[ctx.options.mode])
	ctx.env.append_unique('CXXFLAGS', '-std=c++11')

	# toy-engine library
	ctx.stlib(
		target          = 'glte',
		use             = ['GLFW', 'Eigen'],
		source          = ctx.path.ant_glob([
								'externals/glte/**.cc',
								'externals/glte/**/*.cc',
								'externals/glte/**.c',
								'externals/glte/**/*.c'
							]),
		includes        = 'externals'
	)

	ctx.stlib(
		target          = 'glsw',
		source          = ctx.path.ant_glob([
								'externals/glsw/**.cc',
								'externals/glsw/**/*.cc',
								'externals/glsw/**.c',
								'externals/glsw/**/*.c'
							]),
		includes        = 'externals'
	)

	ctx.program(
		target			= 'particles',
		use 			= ['OpenCL', 'glsw', 'glte'],
		includes 		= 'externals',
		source			= ctx.path.ant_glob([
								'source/*.cc',
								'source/**/*.cc',
								'source/*.c',
								'source/**/*.c'
							])
	)