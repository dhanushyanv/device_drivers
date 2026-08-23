#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define MY_CHAR_MAGIC 'D'
#define CLEAR_BUFFER _IO(MY_CHAR_MAGIC,1)

int main()
{
    int fd = open("/dev/mychar", O_RDWR);
    if(fd <0)
    {
      perror("Failed to open the device");
      return 1;
    }
    printf("device opened successfully\n");
    
    const char *message ="Hello from user Dhanu";
    ssize_t bytes_written;
    bytes_written = write(fd,message,strlen(message));
    if(bytes_written < 0)
    {
        perror("Failed to write");
        close(fd);
        return 1;
    }
    printf("bytes written %ld\n", bytes_written);
    // Clear the buffer using ioctl
    if(ioctl(fd, CLEAR_BUFFER) < 0)
    {
        perror("Failed to clear buffer");
        close(fd);
        return 1;
    }
    
    //Full read 
    char buffer[1024];
    ssize_t bytes_read;
    bytes_read = read(fd,buffer,sizeof(buffer)-1);
    if(bytes_read < 0)
    {
        perror("Failed to read");
        close(fd);
        printf("device closed successfully\n");
        return 1;
    }
    buffer[bytes_read] = '\0';
    printf("bytes read %ld: %s\n", bytes_read, buffer);

    close(fd);
    printf("device closed successfully\n");
    return 0;
}
