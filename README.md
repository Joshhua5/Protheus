Protheus

https://gitter.im/Protheus-Engine?utm_source=share-link&utm_medium=link&utm_campaign=share-link
========
Protheus is a Open Source Cross-Platform Game Engine which is currently _under development_.

The entire engine is build around the idea of working effectively on multiple threads. With that being said there are bugs and functionality might change at any time with no warning as development is still in progress.

Tests:

To run the test script which compiles, executes and analyses all tests on your platform than reports it for aggregated performance information on different platforms and to catch bugs on different platforms.

on OSX: python3 ./path_to_prothues/TestCases/scripts/start.py
on Linux: ./path_to_protheus/TestCases/scripts/start.py
on Windows: windows support is being added

All test data will be publicly available soon, information which is reported is CPU hardware, current operating system and information regarding the execution, compilation and analysis of the tests. To opt out uncomment the first line of submit_post in start.py

Dependencies:

To install dependencies on Linux execute the /TestCases/scripts/install_deps.sh
This will install g++-4.9, valgrind, libglew-dev, libopenal-dev

Execution on linux requires g++-4.9, compiler bugs prevent compilation in < 4.9

Other Libraries used:
	picopng
	lib3ds
	