struct message
{
char privfifo[15];     /* Private named pipe */
char filename[100];
};
#define PUBLIC "public" /* Public named pipe */
#define LINESIZE 512
