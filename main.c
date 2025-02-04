#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    // For open()
#include <unistd.h>   // For read(), write(), close()
#include <sys/stat.h> // For mkdir()
#include <errno.h>

#define DIRECTORY "values"
#define ACCL_FILE DIRECTORY "/accl.dat"
#define ROTA_FILE DIRECTORY "/rota.dat"
#define ANGL_FILE DIRECTORY "/angl.dat"

#define DOUBLE_SIZE sizeof(double)
#define VALUES_PER_PACKET 9 

// Function to check errors when opening files
int checkError(int val, const char *msg) {
    if (val == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

int main() {

    // Input data file
    const char *input_file = "data.dat";

    // Create output directory
    if (mkdir(DIRECTORY, 0777) == -1 && errno != EEXIST) {
        perror("Error creating directory");
        return EXIT_FAILURE;
    }

    // Open input and output files
    int input_fd = checkError(open(input_file, O_RDONLY), "Open data.dat");
    int accl_fd = checkError(open(ACCL_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644), "Open accl.dat");
    int rota_fd = checkError(open(ROTA_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644), "Open rota.dat");
    int angl_fd = checkError(open(ANGL_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644), "Open angl.dat");

    // Buffer to store 3 double values
    double buffer[VALUES_PER_PACKET];
    long bytesRead;

    // Read from input data file
    while ((bytesRead = read(input_fd, buffer, sizeof(buffer))) == sizeof(buffer)) {
        // Write double data to 3 separate files
        write(accl_fd, buffer, DOUBLE_SIZE * 3);
        write(angl_fd, &buffer[6], DOUBLE_SIZE * 3);
        write(rota_fd, &buffer[3], DOUBLE_SIZE * 3);
    }

    // Close all files using file descriptors
    close(input_fd);
    close(accl_fd);
    close(rota_fd);
    close(angl_fd);

    printf("Success!! Data was written to the 3 separate files in the values directoy\n");
    return EXIT_SUCCESS;
}
