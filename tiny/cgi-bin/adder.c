/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"
/* p991. CGI program that sums 2 integers*/
/* ------------Examples of CGI Environment Variables------------*/
  /* QUERY_STRING :  program arguments */
  /* SERVER_PORT :  port that the parent is listening on*/
  /* REQUEST_METHOD :  GET or POST */
  /* REMOTE_HOST :  domain name of client */
  /* REMOTE_ADDR :  dotted-decimal IP address of client */
  /* CONTENT_TYPE :  !!! POST only !!! MIME type of the request body*/
  /* CONTENT_LENGTH :  !!! POST only !!! Size in bytes of the request body */
/* -----------------------------------------------------------*/

int main(void) {
  char *buf, *p;
  char arg1[MAXLINE] , arg2[MAXLINE] , content[MAXLINE];
  int n1 = 0 , n2 = 0;

  /*Extract the two arguments*/
  if ((buf = getenv("QUERY_STRING")) != NULL ){
    p = strchr(buf, '&');
    *p = '\0';
    strcpy(arg1 , buf);
    strcpy(arg2 , p+1);
    n1 = atoi(arg1);
    n2 = atoi(arg2);
  }

  /*Make the response body*/
  sprintf(content, "QUERY_STRING=%s",buf);
  sprintf(content, "Welcome to add.com: ");
  sprintf(content, "%s The Internet addition portal.\r\n<p>",content);
  sprintf(content, "%s The answer is %d + %d = %d\r\n<p>", content , n1 , n2 , n1 + n2);
  sprintf(content, "%s Thanks for visiting!\r\n",content);

  /*Generate HTTP response*/
  printf("Connection : close\r\n");
  printf("Content-length : %d\r\n", (int)strlen(content));
  printf("Content-type :text\html\r\n\r\n");
  fflush(stdout);

  exit(0);
}
/* $end adder */
/**result 
  ubuntu@ip-172-31-39-203:~/webproxy-lab/tiny/cgi-bin$ ./adder
  Connection : close
  Content-length : 107
  Content-type :texthtml
*/