#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_test_program(char *file_name) {
    // A test program is simply one that contains two consecutive underscores.
    
    char char1 = '\0', char2 = '\0';
    while(*file_name != '\0') {
        char2 = char1;
        char1 = *file_name;
        file_name++;

        if (char1 == '_' && char2 == '_') {
            return true;
        }
    }

    return false;
}

int run_test(char* test_program_name) {
    // We blindly assume that our test programs never have a file name
    // longer than 1024 bytes minus the name of the compiler executable.
    char *command = malloc(1024);

    snprintf(command, 1024, "./babyc test_programs/%s >/dev/null", test_program_name);
    int result = system(command);

    free(command);

    return result;
}

int main() {
    DIR *test_dir = opendir("test_programs");

    if (test_dir == NULL) {
        printf("Could not open test_programs directory!");
        return 1;
    }

    int tests_run = 0, tests_passed = 0;

    struct dirent *file;
    char* file_name;
    int test_result;
    while ((file = readdir(test_dir)) != NULL) {
        file_name = file->d_name;
        
        if (is_test_program(file_name)) {
            test_result = run_test(file_name);

            tests_run++;
            if (test_result == 0) {
                printf("%s: OK\n", file_name);
                tests_passed++;
            } else {
                printf("%s: FAILED\n", file_name);
            }
        }
    }
    printf("\n%d tests run, %d passed, %d failed.\n", tests_run, tests_passed, tests_run - tests_passed);

    closedir(test_dir);

    return tests_run - tests_passed;
}
