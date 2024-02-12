/* cat2.c
   simple version of cat: concatenate files
   cat1 copy one file to stdout
   if filename not given on the command line, then use stdin as input
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 4294967291

int main(int argc, char*argv[]) {
    int counter = 1;
    int fd;
    char *buf[BUFSIZE];
    int n;
    while (counter < argc) {
        fd = open(argv[counter],O_RDONLY);
        if (fd < 0) {
            printf("error opening %s\n",argv[1]);
            return -1;
        }
        while ((n=read(fd,buf, BUFSIZE)) > 0) {
            if (write(STDOUT_FILENO,buf,n) != n) {
            }
        }
        counter++;
        close(fd);
    }
	return 0;
}


