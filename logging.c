#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* get_datetime() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    static char formatted_time[100];

    strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S", tm_info);
    // Had to switch to this because asctime() appends a newline, which messed up the log file.
    return formatted_time;
}

void log(char *message, int flag) {
    char log_filename[] = "log.txt"; // Log file name
    char log_entry[256];

    FILE* log_file = fopen(log_filename, "a");

    snprintf(log_entry, sizeof(log_entry), "[%s] %s\n", get_datetime(), message);
    fputs(log_entry, log_file);

    if (flag == 1){
        printf("%s", log_entry);
    }

    fclose(log_file);
}

void clear_log() {
    char log_filename[] = "log.txt"; // Log file name

    FILE* log_file = fopen(log_filename, "w");
    fclose(log_filename);
}

int main(int argc, char *argv[]) {
    clear_log();
    log("Hello World!", 0);
    log("Example 1", 1);
    log("Example 2", 1);
    log("Example 3", 1);
    log("Example 4", 1);

    log("Example 5", 1);

}