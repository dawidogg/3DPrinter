#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct termios config;
int fd;

void setupTermios() {
    cfmakeraw(&config);
    config.c_cflag |= (CLOCAL | CREAD);
    config.c_iflag &= ~(IXOFF | IXANY);

    // set vtime, vmin, baud rate...
    config.c_cc[VMIN] = 0;  // you likely don't want to change this
    config.c_cc[VTIME] = 0; // or this

    cfsetispeed(&config, B115200);
    cfsetospeed(&config, B115200);

    // write port configuration to driver
    tcsetattr(fd, TCSANOW, &config);
}

int main() {
    //setupTermios();
    fd = open("/dev/ttyUSB5", O_RDWR);
    if (fd < 0) {
        perror("Error opening serial port");
        return 0;
    }

    char *first = "1\n\r";
    write(fd, first, sizeof(char));

    while (1) {
        char buffer[1024];
        read(fd, buffer, sizeof(buffer));
        printf("%s\n", buffer);
        write(fd, buffer, sizeof(buffer));
        sleep(1);
    }
    return 0;
}