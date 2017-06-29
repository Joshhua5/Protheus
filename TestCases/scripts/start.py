#!/usr/bin/python3

#       This script compiles Protheus and all test cases found in the
#       TestCases file which end with Test.cpp, on linux further
#       profiling and debugging information is generated with Valgrind.
#       Test logs, hardware and compile information is reported to a server
#       with the intention of providing data for a status page, protheus progress
#       and the ability to detect performance issues on different platforms
#
#       If you're not comfortable with reporting annonymous data about protheus
#       then uncomment the first line in submit_post
#
#	All platforms require clang to be called from the command line
#

import os
import subprocess
import platform
import time
import cpuinfo
import urllib.request
import urllib.parse
import urllib.error
import shutil
import fnmatch
import sys


#Global
root = ""
session_id = ""
util_cpp = []
core_cpp = []


def submit_post(post_data, url_relative):
	#return "Opt-Out, opting in helps us provide improved performance on all platforms"
	try:
		url = urllib.request.Request("http://protheus.net.au/{}.php".format(url_relative))
		url.add_header("Content-Type", "application/x-www-form-urlencoded;charset=utf-8")
		encoded_data = urllib.parse.urlencode(post_data).encode('utf-8')
		return urllib.request.urlopen(url, encoded_data).read().decode('utf-8')
	except urllib.error.URLError:
		return "HTML request failed, check internet connectivity"
	except all:
		return "HTML request failed, check internet connectivity"

def execute_program(exe):
	start = time.time()
	c = subprocess.Popen(exe, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	stdlog, errlog = c.communicate()
	duration = time.time() - start
	stdlog = stdlog.decode(sys.stdout.encoding)
	errlog = errlog.decode(sys.stdout.encoding)
	return stdlog, errlog, duration, c.returncode

def search_dir(directory, postfix):
	results = []
	for root, dirnames, filenames in os.walk(directory):
		for filename in fnmatch.filter(filenames, '*.{}'.format(postfix)):
			results.append(os.path.join(root, filename))
	return results

def clean_headers(directory):
	for file in search_dir(directory, 'cpp'):
		os.remove(file)
	for file in search_dir(directory, 'mm'):
		os.remove(file)
	for file in search_dir(directory, 'vcxproj'):
		os.remove(file)
	for file in search_dir(directory, 'user'):
		os.remove(file)
	for file in search_dir(directory, 'filters'):
		os.remove(file)
	if os.path.isdir(directory+"/Debug"):
		shutil.rmtree(directory+"/Debug")
	if os.path.isdir(directory+"/Release"):
		shutil.rmtree(directory+"/Release")

def collect_headers(directory):
	shutil.copytree(root+"src/Core/", root+"TestCases/bin/Headers/Core")
	shutil.copytree(root+"src/Utilities/", root+"TestCases/bin/Headers/Util")

	# Remove non header files
	clean_headers("{}TestCases/bin/Headers/Core".format(directory))
	clean_headers("{}TestCases/bin/Headers/Util".format(directory))

def generate_cpp_list(directory):
	global core_cpp
	global util_cpp

	util_cpp = search_dir(directory+"src/Utilities", 'cpp')
	print("Found {} files for Utilities".format(len(util_cpp)))

	core_cpp = search_dir(directory+"src/Core", 'cpp')
	print("Found {} files for Core".format(len(core_cpp)))

def archive_obj(directory, library, layer):
	exe = ["ar", "rcs", directory+"TestCases/bin/{}.a".format(library)]

	for root, dirnames, filenames in os.walk(directory+"TestCases/bin/obj/"+layer):
		for filename in fnmatch.filter(filenames, '*.o'):
			exe.append(os.path.join(root, filename))
	subprocess.call(exe)
	return

def request_session():
	info = cpuinfo.get_cpu_info()

	hardware = { 'cpu_model':info['brand'],
		'cpu_stepping':info['stepping'],
		'cpu_family':info['family'],
		'cpu_cachesize':info['l2_cache_size'],
		'cpu_cachealign':info['l2_cache_line_size'],
		'cpu_cache_assos':info['l2_cache_associativity'],
		'cpu_vendor':info['vendor_id'],
		'cpu_arch':info['arch'],
		'cpu_bits':info['bits'],
		'cpu_flags':' '.join(info['flags']),
		'cpu_cores':info['count'],
		'cpu_freq':info['hz_actual'],
		'os':platform.platform(),
		'commit':os.popen('git rev-parse --short HEAD').read()
	}
	return submit_post(hardware, "new_session")

def prepare_workspace():
	if os.path.isdir(root+"TestCases/bin") == True:
		shutil.rmtree(root+"TestCases/bin")
	os.makedirs(root+"TestCases/bin")
	os.makedirs(root+"TestCases/bin/obj")
	os.makedirs(root+"TestCases/bin/obj/Util")
	os.makedirs(root+"TestCases/bin/obj/Core")
	os.makedirs(root+"TestCases/bin/Headers")

def compile_protheus():
	global session_id
	global root

	# Compile Protheus
		
	arguments = ["clang", "-c", "-std=c++11", "-g"]

	os.chdir(root+"TestCases/bin/obj/Util")
	arguments.append("-I")
	arguments.append(root+"src/Utilities")
	util_args = arguments
	for files in util_cpp:
		util_args.append(files)

	stdlog_util, errlog_util, util_dur, retcode = execute_program(util_args)

	archive_obj(root, "libUtilities", "Util")

	os.chdir(root+"TestCases/bin/obj/Core")
	arguments.append("-I")
	arguments.append(root+"src/Core")
	core_args = arguments
	for files in core_cpp:
		core_args.append(files)

	stdlog_core, errlog_core, core_dur, retcode = execute_program(core_args)
	archive_obj(root, "libCore", "Core")
	#Report the compile details

	print("	{} {}".format('Utilities:', submit_post({
			's_id':session_id,
			'layer':'Utilities',
			'duration':util_dur,
			'log':"Standard Log: {} Error Log: {}".format(stdlog_util, errlog_util)},
			"report_protheus")))

	print("	{} {}".format('Core:', submit_post({
			's_id':session_id,
			'layer':'Core',
			'duration':core_dur,
			'log':"Standard Log: {} Error Log: {}".format(stdlog_core, errlog_core)},
			"report_protheus")))

def extract_value(search, string, ending):
	start = string.find(search) + len(search)
	count_len = string.find(ending, start) - start
	return string[start:start + count_len].strip()

def compile_tests():
	global root
	# Scan for tests
	os.chdir(root+"TestCases/bin/")
	for root_dir, dirnames, filenames in os.walk(root+"TestCases"):
		for filename in fnmatch.filter(filenames, '*Test.cpp'):
			output = filename.replace('.cpp','')
			gl_library = ""
			window_library = ""	
			if platform.system() == "Linux":
				openGL_library = "-lGL"
				window_library =  "-lX11"
		
			test = ["clang", "-std=c++11", "-pthread", "-g",
				 "-I", root+"src/Utilities", "-I", root+"src/Core",
				openGL_library, window_library,
				"-L", root+"TestCases/bin/", "-lUtilities", "-lCore",
				"-o", root+"TestCases/bin/"+output, os.path.join(root_dir, filename)]
			print("Testing {}:".format(output))

			stdlog_c, errlog_c, compile_dur, retcode = execute_program(test)

			compile_success = os.path.isfile(root+"TestCases/bin/"+output)

			if compile_success == True:
				stdlog_e, errlog_e, execute_dur, returncode = execute_program(root+"TestCases/bin/"+output)

				#Check if valgrind is installed
				#Will return error code 127 if not found
				if subprocess.call(["which", "valgrind"]) == 0:
					#Valgrind
					stdlog_v, errlog_v, dur, retcode = execute_program(["valgrind","-v",
						"--leak-check=full","--show-leak-kinds=all",
						 root+"TestCases/bin/"+output])

					mem_error_count = extract_value("ERROR SUMMARY: ", errlog_v, " e")
					mem_used = extract_value("in use at exit: ", errlog_v, " b")
					mem_leaked = extract_value("frees, ", errlog_v, " b")

					#CacheGrind

					stdlog_ca, errlog_ca, dur, retcode = execute_program(
						["valgrind", "--tool=cachegrind",
						"--cachegrind-out-file="+root+"TestCases/bin/cache"+output,
						 root+"TestCases/bin/"+output])

					cache_detailed = open(root+"TestCases/bin/cache"+output, 'r').read()

					d1_miss_rate = extract_value("D1  miss rate:", errlog_ca, "%")
					i1_miss_rate = extract_value("I1  miss rate:", errlog_ca, "%")

					# To Be Added : Massif / Helgrind
				else:
					print("Unable to find Valgrind executable")
					d1_miss_rate = 0
					i1_miss_rate = 0
					mem_error_count = 0
					mem_used = 0
					mem_leaked = 0
					stdlog_ca = "No Support"
					errlog_ca = "No Support"
					stdlog_v = "No Support"
					errlog_v = "No Support"
					cache_detailed = "No Support"

			#report test

			post = {'s_id':session_id,
				'compile_success':compile_success,
				'test_title':filename.encode("utf-8"),
				'test_compile_dur':compile_dur,
				'test_com_log':"Standard Log: {} Error Log: {}".format(stdlog_c, errlog_c) }
			if compile_success:
				post.update({   'test_execute_dur':execute_dur,
						'test_exit_status':returncode,
						'test_exe_log':"stdlog:{} errlog:{}".format(stdlog_e, errlog_e),
						'cache_i1_miss_rate':i1_miss_rate,
						'cache_d1_miss_rate':d1_miss_rate,
						'cache_output':errlog_ca,
						'cache_detailed':cache_detailed,
						'mem_error_count':mem_error_count,
						'mem_used':mem_used,
						'mem_leaked':mem_leaked,
						'mem_log':errlog_v})

			print("	{}".format(submit_post(post, "report_test")))
	return

def main():
	global root
	global session_id

	subprocess.Popen(['echo', 'Checking for new git version'])

	root = os.path.dirname(os.path.realpath(__file__)) + '/../../'
	os.chdir(root)
	# Check for new git commit
	subprocess.call(["git", "pull"])

	# Report hardware to get a session_id

	prepare_workspace()

	generate_cpp_list(root)
	collect_headers(root)
	session_id = request_session()

	print("Your Session ID is: {}\n Report this if you're experiencing problems".format(session_id))

	print("Compiling Protheus")
	compile_protheus()
	print("Starting Tests")
	compile_tests()

main()
