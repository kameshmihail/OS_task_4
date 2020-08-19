#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#include <string.h>

int main (int argc, char * argv[])
{
	int fd;
	struct stat mystat;
	char *pmap;
	pid_t p;



	fd = open("test.txt", O_RDWR);
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}

	if(fstat(fd, &mystat) < 0)
	{	
		perror("fstat");
		close(fd);
		exit(1);
	}

	pmap = mmap(0, mystat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(pmap == MAP_FAILED)
	{
		perror("mmap");
		close(fd);
		exit(1);
	}

	p = fork(); 
    	if (p < 0) 
    	{ 
       	 fprintf(stderr, "fork Failed" ); 
        	return 1; 
    	} 
   	 else if (p > 0) 
    	{
		char a[256] = "1";
		char b[256] = "1";
	
		scanf("%s %s", a, b);
		char* con = (char*)malloc(strlen(a)+strlen(b)+1);
		for (int i = 0; i < strlen(a); i++)
		{
			con[i]=a[i];
		}
		con[strlen(a)]='#';
		for (int i = 0; i < strlen(b); i++)
		{
			con[strlen(a)+i+1]=b[i];
		}
		con[strlen(a)+strlen(b)+1]='#';
		for (int i = 0; i < strlen(con); i++)
		{
			pmap[i] = con[i];
		}

		wait(NULL);
		printf ("Point lies in %c coordinate square\n", pmap[0]);
	}
	else
	{
		sleep(4);
		char a[256] = "1";
		char b[256] = "1";
		char coord = 'a';
		int c = 0;

		for (int i = 0; i < strlen(pmap); i++)
		{
			if (pmap[i] != '#') {
				a[i] = pmap [i];
				c++;
			}
			else {
				break;
			}
		}
		for (int i = 0; i < strlen(pmap)-c-1; i++)
		{
			if (pmap[i+c+1] != '#') {
				b[i] = pmap[i+c+1];
			}
			else {
				break;
			}
		}
		int firstcoordinate = atoi(a);
		int secondcoordinate = atoi(b);	
		if (firstcoordinate >= 0 && secondcoordinate >= 0) {
			coord = '1';
		}
		if (firstcoordinate < 0 && secondcoordinate < 0) {
			coord = '3';
		}
		if (firstcoordinate >= 0 && secondcoordinate < 0) {
			coord = '4';
		}
		if (firstcoordinate < 0 && secondcoordinate >= 0) {
			coord = '2';
		}
		pmap[0] = coord;

	}
	close(fd);
	return 0;
}
