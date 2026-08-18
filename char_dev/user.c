#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    int fd = open("/dev/mychar", O_RDWR);
    if(fd <0)
    {
      perror("Failed to open the device");
      return 1;
    }
    printf("device opened successfully\n");
    close(fd);
    printf("device closed successfully\n");
    return 0;
}
