#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd = open("/dev/mychar", O_RDWR);
    if(fd <0)
    {
      perror("Failed to open the device");
      return 1;
    }
    printf("device opened successfully\n");
    
    const char *message ="Hello from user";
    ssize_t bytes_written;
    bytes_written = write(fd,message,strlen(message));
    if(bytes_written < 0)
    {
        perror("Failed to write");
        close(fd);
        return 1;
    }
    printf("bytes written %ld\n", bytes_written);
    
    close(fd);
    printf("device closed successfully\n");
    return 0;
}
