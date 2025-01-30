#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    // For open()
#include <unistd.h>   // For read(), write(), close()
#include <sys/stat.h> // For mkdir()

#define INPUT_FILE "data.dat"
#define DIRECTORY "values"
#define ACCL_FILE DIRECTORY "/accl.dat"
#define ROTA_FILE DIRECTORY "/rota.dat"
#define ANGL_FILE DIRECTORY "/angl.dat"

#define DOUBLE_SIZE sizeof(double)
#define VALUES_PER_PACKET 9 // Each packet has 9 double values

// Function to check errors when opening files
int checkError(int val, const char *msg) {
    if (val == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

int main() {
    // Step 1: Create the "values" directory
    if (mkdir(DIRECTORY, 0777) == -1 && errno != EEXIST) {
        perror("Error creating directory");
        return EXIT_FAILURE;
    }

    // Step 2: Open the required files
    int input_fd = checkError(open(INPUT_FILE, O_RDONLY), "Open data.dat");
    int accl_fd = checkError(open(ACCL_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644), "Open accl.dat");
    int rota_fd = checkError(open(ROTA_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644), "Open rota.dat");
    int angl_fd = checkError(open(ANGL_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644), "Open angl.dat");

    double buffer[VALUES_PER_PACKET]; // Buffer to store a full packet (9 doubles)
    ssize_t bytesRead;

    // Step 3: Read from "data.dat" and write to the appropriate files
    while ((bytesRead = read(input_fd, buffer, sizeof(buffer))) == sizeof(buffer)) {
        // Step 4: Write ax, ay, az to "accl.dat"
        write(accl_fd, buffer, DOUBLE_SIZE * 3);

        // Step 5: Write roll, pitch, yaw to "angl.dat"
        write(angl_fd, &buffer[6], DOUBLE_SIZE * 3);

        // Step 6: Write wx, wy, wz to "rota.dat"
        write(rota_fd, &buffer[3], DOUBLE_SIZE * 3);
    }

    // Step 7: Close all files
    close(input_fd);
    close(accl_fd);
    close(rota_fd);
    close(angl_fd);

    printf("Processing complete. Data written to 'values/' directory.\n");
    return EXIT_SUCCESS;
}
