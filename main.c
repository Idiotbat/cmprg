#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

//Define the macro ERROR_OUTPUT to output additional error information
#define ERROR_OUTPUT

//Conditional compilation: Forward or backslashes depending on the system environment
#if defined _WIN32
    #define SLASH "\\"
#else
    #define SLASH "/"
#endif


//Explicit constant: The number of subdirectories
#define Secondary_Directory_NUM 5
//Array of subdirectory names
const char* Secondary_Directory[Secondary_Directory_NUM] = {
    "bin",
    "build",
    "lib",
    "include",
    "src"
};
//Project configuration file name
const char* CMake_Configuration_File = "CMakeLists.txt";

//A function that prints usage information
void printUsageMessage(const char* const Program_Name);
//A function to create directories
bool Creat_Directories(const char* const Program_Name);
//A function to create files
void Creat_Files(const char* const Program_Name);

//Main function
int main(int argc, char* argv[]) {
	//Prompt the user when the command line argument is insufficient
    if (argc <= 1) {
        printUsageMessage(argv[0]);
        exit(EXIT_FAILURE);
    }
	//The loop processes each argument
    for (int i = 1; i < argc; i++) {
        fprintf(stdout, "[Create the %s directory structure]\n", argv[i]);
        if (Creat_Directories(argv[i]))
			Creat_Files(argv[i]);
        printf("\n");
    }
}

void printUsageMessage(const char* const Program_Name) {
	//Null pointer detection
	if (Program_Name == NULL) {
#if defined ERROR_OUTPUT
		fprintf(stderr, "Error:The %s function has one "
			"or more null Pointers in its arguments", __func__);
#endif
		return;
	}
	
    fprintf(stdout, "-Usage:%s \"Project_folder_name\"\n", Program_Name);
    fprintf(stdout, "--Examples:%s MyProject\n", Program_Name);
}

bool Creat_Directories(const char* const Project_Folder_Name) {	
	//Null pointer detection
    if (Project_Folder_Name == NULL) {
#if defined ERROR_OUTPUT
		fprintf(stderr, "Error:The %s function has one "
			"or more null Pointers in its arguments", __func__);
#endif
        return false;
    }
	
	//Determines if the folder being created already exists
    if (_access(Project_Folder_Name, 0) == 0) {
        char temp[5];
        fprintf(stdout, "-The folder \"%s\" already exists,"
            "whether to delete the existing folder to continue" 
            "(no, then skip)\n--[Y/N]", Project_Folder_Name);
        fgets(temp, 5, stdin);
        if (temp[0] == '\n' || temp[0] == 'y' || temp[0] == 'Y') {
			//Delete the original folder
			char rmdir_cmd[_MAX_PATH] = "rm -rf ";
			strcat(rmdir_cmd, Project_Folder_Name);
            system(rmdir_cmd);
        } else if (temp[0] == 'n' || temp[0] == 'N') {
            fprintf(stdout, "--Skiped\n");
            return false;
        } else {
            fprintf(stdout, "--Unknown input, skiped\n");
            return false;
        }
    }
	//Create the project master folder
    fprintf(stdout, "-Create the first-level folder:%s\n", Project_Folder_Name);
	//Returns if the creation failed
    if (_mkdir(Project_Folder_Name) == -1) {
        fprintf(stdout, "-Failed to create folder:%s\n", Project_Folder_Name);
        return false;
    }
	//Create the subdirectories
    fprintf(stdout, "--Create the secondary folders\n");
    for (int i = 0; i < Secondary_Directory_NUM; i++) {
		//Generate relative path
        char temp[_MAX_PATH];
		strcpy(temp, Project_Folder_Name);
        strcat(temp, SLASH);
        strcat(temp, Secondary_Directory[i]);
        fprintf(stdout, "  -%s\n", temp);
        if (_mkdir(temp) == -1) {
            fprintf(stdout, "-Failed to create folder:%s\n", temp);
            continue;
        }
    }
    return true;
}

void Creat_Files(const char* const Project_Folder_Name) {
	//Null pointer detection
    if (Project_Folder_Name == NULL) {
#if defined ERROR_OUTPUT
		fprintf(stderr, "Error:The %s function has one "
			"or more null Pointers in its arguments", __func__);
#endif
        return;
    }
	
	//A file is created only if the corresponding folder exists
    if (_access(Project_Folder_Name, 0) == 0) {    
		//Generate relative path
        char temp[_MAX_PATH];
		strcpy(temp, Project_Folder_Name);
        strcat(temp, SLASH);
        strcat(temp, CMake_Configuration_File);
		fprintf(stdout, "-[Create the file: %s]\n", temp);
		//The fopen function creates a file and writes information to it
        FILE* pFile = fopen(temp, "w");
		
		if (pFile != NULL) {
			//Writes basic information to the project configuration file
            fprintf(pFile, "cmake_minimum_required(VERSION 3.10)\n\n");
            fprintf(pFile, "project(\"%s\" VERSION 1.0 LANGUAGES C)\n\n", Project_Folder_Name);            
            fprintf(pFile, "include_directories(\"include\")\n\n");
            fprintf(pFile, "set(SRC_DIR \"src\")\n");
            fprintf(pFile, "set(SRC_LIST\n");
            fprintf(pFile, "\t\"${SRC_DIR}/main.c\"\n)\n\n");
            fprintf(pFile, "add_executable(\"%s\" ${SRC_LIST})", Project_Folder_Name);
			//Close the file and print an error message if closing fails
            if ((fclose(pFile)) == EOF) {
#if defined ERROR_OUTPUT
                fprintf(stderr, "An error occurred while closing the file:%s\n", temp);
#endif
            }
		} else {
#if defined ERROR_OUTPUT
                fprintf(stderr, "-Failed to create or open file:%s\n", temp);
#endif
        }
    }
	
	//Generate relative path
    char temp2[_MAX_PATH];
	strcpy(temp2, Project_Folder_Name);
    strcat(temp2, SLASH);
    strcat(temp2, "src");
	
    if (_access(temp2, 0) == 0) {
        strcat(temp2, SLASH);
        strcat(temp2, "main.c");
		fprintf(stdout, "-[Create the file: %s]\n", temp2);
		
        FILE* pFile2 = fopen(temp2, "w");

        if (pFile2 != NULL) {
            fprintf(pFile2, "#include <stdio.h>\n\n");
            fprintf(pFile2, "int main(void) {\n\n");
            fprintf(pFile2, "\treturn 0;\n}");
            if ((fclose(pFile2)) == EOF) {
#if defined ERROR_OUTPUT
                fprintf(stderr, "An error occurred while closing the file:%s\n", temp2);
#endif
            }
        } else {
#if defined ERROR_OUTPUT
            fprintf(stderr, "-Failed to create or open file:%s\n", temp2);
#endif
		}
    }
}