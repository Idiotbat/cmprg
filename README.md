# cmprg

cmprg(CMake project),a simple command-line tool for creating base directories for CMake based projects.

## What does cmprg do

First, cmprg creates the project folder based on the parameters.

````cmd
cmprg test
````

cmprg will then create a basic folder and two basic files: XX, XX in the project directory structure, and add some basic content to the files.

````cmd
-test
 -bin
 -build
 -lib
 -include
 -src
  main.c
 CMakeLists.txt
````

- CMakeLists.txt

````cmake
cmake_minimum_required(VERSION 3.10)

project("test" VERSION 1.0 LANGUAGES C)

include_directories("include")

set(SRC_DIR "src")
set(SRC_LIST
	"${SRC_DIR}/main.c"
)

add_executable("test" ${SRC_LIST})
````

- main.c

````c
#include <stdio.h>

int main(void) {

	return 0;
}
````

