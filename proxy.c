// // #include <stdio.h>

// // /* Recommended max cache and object sizes */
// // #define MAX_CACHE_SIZE 1049000
// // #define MAX_OBJECT_SIZE 102400

// // /* You won't lose style points for including this long line in your code */
// // static const char *user_agent_hdr =
// //     "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
// //     "Firefox/10.0.3\r\n";

// // int main() {
// //   printf("%s", user_agent_hdr);
// //   return 0;
// // }

// // Web proxy

// #include <stdio.h>
// #include "csapp.h"

// /* Recommended max cache and object sizes */
// #define MAX_CACHE_SIZE 1049000
// #define MAX_OBJECT_SIZE 102400

// /* You won't lose style points for including this long line in your code */
// static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
// static const char *conn_hdr = "Connection: close\r\n";
// static const char *prox_hdr = "Proxy-Connection: close\r\n";
// static const char *host_hdr_format = "Host: %s\r\n";
// static const char *requestlint_hdr_format = "GET %s HTTP/1.0\r\n";
// static const char *endof_hdr = "\r\n";

// static const char *connection_key = "Connection";
// static const char *user_agent_key= "User-Agent";
// static const char *proxy_connection_key = "Proxy-Connection";
// static const char *host_key = "Host";
// void *thread(void *vargp);

// void doit(int connfd);
// void parse_uri(char *uri,char *hostname,char *path,int *port);
// void build_http_header(char *http_header,char *hostname,char *path,int port,rio_t *client_rio);
// int connect_endServer(char *hostname,int port,char *http_header);

// int main(int argc,char **argv)
// {
//     int listenfd, connfd;
//     socklen_t  clientlen;
//     char hostname[MAXLINE],port[MAXLINE];
//     pthread_t tid;
//     struct sockaddr_storage clientaddr;

//     if(argc != 2){
//         fprintf(stderr,"usage :%s <port> \n",argv[0]);
//         exit(1);
//     }

//     listenfd = Open_listenfd(argv[1]);
//     while(1){
//         clientlen = sizeof(clientaddr);
//         connfd = Accept(listenfd,(SA *)&clientaddr,&clientlen);

//         /*print accepted message*/
//         Getnameinfo((SA*)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
//         printf("Accepted connection from (%s %s).\n",hostname,port);
//         Pthread_create(&tid, NULL, thread, (void *)connfd);         // 다중 쓰레딩을 통한 conncurrent 접속 구현
//         /*sequential handle the client transaction*/
//     }
//     return 0;
// }
// void* thread(void *vargp){
//     int connfd = (int)vargp;
//     Pthread_detach(pthread_self());
//     doit(connfd);
//     Close(connfd);
// }
// /*handle the client HTTP transaction*/
// void doit(int connfd)
// {
//     int end_serverfd;/*the end server file descriptor*/

//     char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
//     char endserver_http_header [MAXLINE];
//     /*store the request line arguments*/
//     char hostname[MAXLINE],path[MAXLINE];
//     int port;

//     rio_t rio,server_rio;/*rio is client's rio,server_rio is endserver's rio*/

//     Rio_readinitb(&rio,connfd);
//     Rio_readlineb(&rio,buf,MAXLINE);
//     sscanf(buf,"%s %s %s",method,uri,version); /*read the client request line*/

//     if(strcasecmp(method,"GET")){
//         printf("Proxy does not implement the method");
//         return;
//     }
//     /*parse the uri to get hostname,file path ,port*/
//     parse_uri(uri,hostname,path,&port);

//     /*build the http header which will send to the end server*/
//     build_http_header(endserver_http_header,hostname,path,port,&rio);

//     /*connect to the end server*/
//     end_serverfd = connect_endServer(hostname,port,endserver_http_header);
//     if(end_serverfd<0){
//         printf("connection failed\n");
//         return;
//     }

//     Rio_readinitb(&server_rio,end_serverfd);
//     /*write the http header to endserver*/
//     Rio_writen(end_serverfd,endserver_http_header,strlen(endserver_http_header));

//     /*receive message from end server and send to the client*/
//     size_t n;
//     while((n=Rio_readlineb(&server_rio,buf,MAXLINE))!=0)
//     {
//         printf("proxy received %d bytes,then send\n",n);
//         Rio_writen(connfd,buf,n);
//     }
//     Close(end_serverfd);
// }

// void build_http_header(char *http_header,char *hostname,char *path,int port,rio_t *client_rio)
// {
//     char buf[MAXLINE],request_hdr[MAXLINE],other_hdr[MAXLINE],host_hdr[MAXLINE];
//     /*request line*/
//     sprintf(request_hdr,requestlint_hdr_format,path);
//     /*get other request header for client rio and change it */
//     while(Rio_readlineb(client_rio,buf,MAXLINE)>0)
//     {
//         if(strcmp(buf,endof_hdr)==0) break;/*EOF*/

//         if(!strncasecmp(buf,host_key,strlen(host_key)))/*Host:*/
//         {
//             strcpy(host_hdr,buf);
//             continue;
//         }

//         if(!strncasecmp(buf,connection_key,strlen(connection_key))
//                 &&!strncasecmp(buf,proxy_connection_key,strlen(proxy_connection_key))
//                 &&!strncasecmp(buf,user_agent_key,strlen(user_agent_key)))
//         {
//             strcat(other_hdr,buf);
//         }
//     }
//     if(strlen(host_hdr)==0)
//     {
//         sprintf(host_hdr,host_hdr_format,hostname);
//     }
//     sprintf(http_header,"%s%s%s%s%s%s%s",
//             request_hdr,
//             host_hdr,
//             conn_hdr,
//             prox_hdr,
//             user_agent_hdr,
//             other_hdr,
//             endof_hdr);

//     return ;
// }
// /*Connect to the end server*/
// inline int connect_endServer(char *hostname,int port,char *http_header){
//     char portStr[100];
//     sprintf(portStr,"%d",port);
//     return Open_clientfd(hostname,portStr);
// }

// /*parse the uri to get hostname,file path ,port*/
// void parse_uri(char *uri,char *hostname,char *path,int *port)
// {
//     *port = 80;
//     char* pos = strstr(uri,"//");

//     pos = pos!=NULL? pos+2:uri;

//     char*pos2 = strstr(pos,":");
//     if(pos2!=NULL)
//     {
//         *pos2 = '\0';
//         sscanf(pos,"%s",hostname);
//         sscanf(pos2+1,"%d%s",port,path);
//     }
//     else
//     {
//         pos2 = strstr(pos,"/");
//         if(pos2!=NULL)
//         {
//             *pos2 = '\0';
//             sscanf(pos,"%s",hostname);
//             *pos2 = '/';
//             sscanf(pos2,"%s",path);
//         }
//         else
//         {
//             sscanf(pos,"%s",hostname);
//         }
//     }
//     return;
// }

/*
 *  Proxylab
 *
 *  Auther: Tianyu Chen
 *  Andrew ID: tianyuc
 *  Email: tianyuc@andrew.cmu.edu
 *
 *  [Brief Description]: 
 *    This is a basic proxy, which acts as a server when connecting to a 
 *  client and acts as a client when connecting to remote web servers. And 
 *  it can deal with multiple concurrent requests. Finally, a cache with LRU
 *  rule is added to improve the function of this proxy.
 *
 *    The proxy works well on the following pages: 
 *      – http://www.cs.cmu.edu/˜213
 *      – http://csapp.cs.cmu.edu
 *      – http://www.cmu.edu
 *      – http://www.amazon.com
 *      - http://www.youtube.com
 *    
 *    Some features in this proxy:
 *      - Robust I/O  
 *      - Block SIGPIPE signals
 *      - LRU eviction rule
 *      - lock proper threads under concurrent requests
 *      
 */

#include <stdio.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including these long lines in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *accept_hdr = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
static const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";

#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *hostname, int *port);
void clienterror(int fd, char *cause, char *errnum, 
		         char *shortmsg, char *longmsg);
int powerten(int i);
void proxy_init(void);
void init_cache(void);
void *thread(void *vargp);
static void update_use(int *cache_use, int current, int len);
static int load_cache(char *tag, char *response);

static void save_cache(char *tag, char *response);
static void request_hdr(char *buf, char *buf2ser, char *hostname);

struct cache_line
{
    int valid;
    char *tag;
    char *block;
};

struct cache_set
{
    struct cache_line *line;
    int *use;
};

struct cache
{
    struct cache_set *set;
};

static struct cache cache;

// global variables
sem_t mutex;
static int set_num, line_num;

int main(int argc, char **argv) 
{
    signal(SIGPIPE, SIG_IGN); // ignore sigpipe

    int listenfd;
    int *connfd;

    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    pthread_t tid;

    /* Check command line args */
    if (argc != 2) {
	    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(1);
    }

    proxy_init();
    listenfd = Open_listenfd(argv[1]);
    while (1) {
	    clientlen = sizeof(clientaddr);
        connfd = malloc(sizeof(int));
	    *connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); 
        printf("Accepted connection from (%s, %s)\n", hostname, port);
//#include <string.h>
        pthread_create(&tid, NULL, thread, connfd);
    }
    return 0;
}

/*
 * proxy_init
 * initialize the whole proxy including cache
 */
void proxy_init()
{
    sem_init(&mutex, 0, 1);
    set_num = 1;
    line_num = 10;
    init_cache();
}

/*
 * init_cache 
 * initialize the cache, malloc space for the cache
 */
void init_cache()
{
    int i, j;
    cache.set = malloc(sizeof (struct cache_set) * set_num);
    for (i = 0; i < set_num; i++)
    {
        cache.set[i].line = malloc(sizeof(struct cache_line) * line_num);
        cache.set[i].use = malloc(sizeof(int) * line_num);
       for (j = 0; j < line_num; j++)
       {
           cache.set[i].use[j] = j;
           cache.set[i].line[j].valid = 0;
           cache.set[i].line[j].tag = malloc(MAXLINE);
           cache.set[i].line[j].block = malloc(MAX_OBJECT_SIZE);
       } 
    }
}

/* 
 * update_use - record cache usage condition for eviction
 * record the recent use of cache store in the array 'use', LRU rule
 * Least Reacently Used : 
 */
static void update_use(int *cache_use, int current, int len)
{
    int i, j;
    for(i = 0; i < len; i++)
    {
        if(cache_use[i] == current) {
             break;
        }
    }
    for(j = i; j > 0; j--)
    {
        cache_use[j] = cache_use[j - 1];
    }                               
//#include <string.h>
    cache_use[0] = current;
}

/*
 * load_cache - load data from cache
 * search desired cache and buffer the data in response
 */
static int load_cache(char *tag, char *response) 
{
    int index, i;
    index = 0;
    for (i = 0; i < line_num; i++) {
        if(cache.set[index].line[i].valid == 1 && 
          (strcmp(cache.set[index].line[i].tag, tag) == 0))
        {
            P(&mutex);
            update_use(cache.set[index].use, i, line_num);
            V(&mutex);
            strcpy(response, cache.set[index].line[i].block);
            break;
        }
    }
    if (i == line_num) {
        return 0;
    }
    else {
        return 1;
    }
}

/* 
 * save_cache - save data from server in cache
 * copy response and tag into cache 
 */ 
static void save_cache(char *tag, char *response)
{
    int index, eviction;
    index = 0;
    eviction = cache.set[index].use[line_num - 1];
    strcpy(cache.set[index].line[eviction].tag, tag);
    strcpy(cache.set[index].line[eviction].block, response);
//#include <string.h>
    if (cache.set[index].line[eviction].valid == 0) {
        cache.set[index].line[eviction].valid = 1;
    }
    update_use(cache.set[index].use, eviction, line_num);;
}

/*
 * doit - handle one HTTP request/response transaction
 */

/* $begin doit */
void doit(int fd) 
{
    int serverfd, len, object_len;

    int *port;
    char port2[10];
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char cache_buf[MAX_OBJECT_SIZE];// store cache data stuff
    char filename[MAXLINE];         // client request filename
    char hostname[MAXBUF];          // client request hostname
    char buf2ser[MAXLINE];          // proxy to server
    char ser_response[MAXLINE];     // server to proxy
    rio_t rio, rio_ser;             // rio: between client and proxy
                                    // rio_ser: between proxy and server
    port = malloc(sizeof(int));
    *port = 80;                      // default port 80

    memset(buf2ser, 0, sizeof(buf2ser)); 
    memset(filename, 0, sizeof(filename)); 
    memset(hostname, 0, sizeof(hostname)); 
    memset(ser_response, 0, sizeof(ser_response));
    memset(uri, 0, sizeof(uri));
    memset(method, 0, sizeof(method));
    memset(buf, 0, sizeof(buf));
    memset(version, 0, sizeof(version));
    memset(cache_buf, 0, sizeof(cache_buf)); 

    // step1: obtain request from client and parse the request
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  
        return;
    printf("request from client: %s\n", buf);

    // parse request into method, uri, version
    sscanf(buf, "%s %s %s", method, uri, version);       
    
    // check HTTP version, if 1.1, change it into 1.0
    if (!strcasecmp(version, "HTTP/1.1")) {
        strcpy(version, "HTTP/1.0");
    }

    // we only need GET method
    if (strcasecmp(method, "GET")) {     
        clienterror(fd, method, "501", "Not Implemented",
                    "This proxy does not implement this method");
        return;
    }               
    read_requesthdrs(&rio);

    /* Parse URI from GET request */
    parse_uri(uri, hostname, port);       
    strcpy(filename, uri);
    sprintf(buf2ser, "%s %s %s\r\n", method, filename, version);
    printf("proxy to server: %s\n", buf2ser);

    // request header
    request_hdr(buf, buf2ser, hostname);
    
    // check cache first
    if (load_cache(uri, cache_buf) == 1) {
        printf("Hit!\n");
        if (rio_writen(fd, cache_buf, sizeof(cache_buf)) < 0) {
            fprintf(stderr, "Error: cache load!\n");
            return;
        }
        memset(cache_buf, 0, sizeof(cache_buf));
    }
    else {   
    // if cache miss then forward the request to server
    // step2 : from proxy to server
        sprintf(port2, "%d", *port);
        if((serverfd = open_clientfd(hostname, port2)) < 0)
        {
            fprintf(stderr, "open server fd error\n");
            return;
        }

        Rio_readinitb(&rio_ser, serverfd);

        // send request to server
        Rio_writen(serverfd, buf2ser, strlen(buf2ser));

        // step3: recieve the response from the server and save data in cache
        memset(cache_buf, 0, sizeof(cache_buf));
        object_len = 0;

        while ((len = rio_readnb(&rio_ser, ser_response, 
                sizeof(ser_response))) > 0) {

            Rio_writen(fd, ser_response, len);

            strcat(cache_buf, ser_response);
            object_len += len;
            memset(ser_response, 0, sizeof(ser_response)); 
        }    
        if (object_len <= MAX_OBJECT_SIZE)
        {
            P(&mutex);
            save_cache(uri, cache_buf);
            V(&mutex);
        }
        close(serverfd);
    }
}
/* $end doit */

/*
 * request_hdr - request header
 * if the request does not contain header, add request header
 */
static void request_hdr(char *buf, char *buf2ser, char *hostname)
{
    if(strcmp(buf, "Host"))
    {
          strcat(buf2ser, "Host: ");
          strcat(buf2ser, hostname);
          strcat(buf2ser, "\r\n");
    }
    if(strcmp(buf, "Accept:")) {
        strcat(buf2ser, accept_hdr);
    }
    if(strcmp(buf, "Accept-Encoding:")) {
        strcat(buf2ser, accept_encoding_hdr);
    }
    if(strcmp(buf, "User-Agent:")) {
        strcat(buf2ser, user_agent_hdr);
    }
    if(strcmp(buf, "Proxy-Connection:")) {
        strcat(buf2ser, "Proxy-Connection: close\r\n");
    }
    if(strcmp(buf, "Connection:")) {
        strcat(buf2ser, "Connection: close\r\n");
    }
    memset(buf, 0, sizeof(buf));
    strcat(buf2ser, "\r\n");
}

/*
 * thread - thread funciton
 *
 */
void *thread(void *vargp)
{
    int connfd = *((int *)vargp);
    pthread_detach(pthread_self());
    free(vargp);
    doit(connfd);
    close(connfd);
    return NULL;
}

/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp) 
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    while(strcmp(buf, "\r\n")) {      
	Rio_readlineb(rp, buf, MAXLINE);
	printf("%s", buf);
    }
    return;
}
/* $end read_requesthdrs */

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
int parse_uri(char *uri, char *hostname, int *port) 
{
    // in this lab all requests are static 

    char tmp[MAXLINE];          // holds local copy of uri
    char *buf;                  // ptr that traverses uri
    char *endbuf;               // ptr to end of the cmdline string
    int port_tmp[10];
    int i, j;                   // loop
    char num[2];                // store port value

    buf = tmp;
    for (i = 0; i < 10; i++) {
        port_tmp[i] = 0;
    }
    (void) strncpy(buf, uri, MAXLINE);
    endbuf = buf + strlen(buf);
    buf += 7;                   // 'http://' has 7 characters
    while (buf < endbuf) {
    // take host name out
        if (buf >= endbuf) {
            strcpy(uri, "");
            strcat(hostname, " ");
            // no other character found
            break;
        }
        if (*buf == ':') {  // if port number exists
            buf++;
            *port = 0;
            i = 0;
            while (*buf != '/') {
                num[0] = *buf;
                num[1] = '\0';
                port_tmp[i] = atoi(num);
                buf++;
                i++;
            }
            j = 0;
            while (i > 0) {
                *port += port_tmp[j] * powerten(i - 1);
                j++;
                i--;
            }
        }
        if (*buf != '/') {

            sprintf(hostname, "%s%c", hostname, *buf);
        }
        else { // host name done
            strcat(hostname, "\0");
            strcpy(uri, buf);
            break;
        }
        buf++;
    }
    return 1;
}
/* $end parse_uri */

/*
 * powerten - return ten to the power of i
 */
int powerten(int i) {
    int num = 1;
    while (i > 0) {
        num *= 10;
        i--;
    }
    return num;
}

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
/* $end clienterror */