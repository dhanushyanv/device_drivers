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
    
    //Full read 
    // char buffer[1024];
    // ssize_t bytes_read;
    // bytes_read = read(fd,buffer,sizeof(buffer)-1);
    // if(bytes_read < 0)
    // {
    //     perror("Failed to read");
    //     close(fd);
    //     printf("device closed successfully\n");
    //     return 1;
    // }
    // buffer[bytes_read] = '\0';
    // printf("bytes read %ld: %s\n", bytes_read, buffer);

    //partial buffer read
    char partial_buffer[6];
    ssize_t bytes_read_partial;
    while((bytes_read_partial = read(fd, partial_buffer, sizeof(partial_buffer)-1)) > 0)
    {
        partial_buffer[bytes_read_partial] = '\0';
        printf("bytes read partial %ld: %s\n", bytes_read_partial, partial_buffer);
    }
    if(bytes_read_partial < 0)
    {
        perror("Failed to read");
        close(fd);
        printf("device closed successfully\n");
        return 1;
    }

    close(fd);
    printf("device closed successfully\n");
    return 0;
}
