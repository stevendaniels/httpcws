extern "C" {
#include <sys/types.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <wait.h>

#include <err.h>
#include <event.h>
#include <evhttp.h>

#define VERSION "1.0"
}
#include "ICTCLAS/ICTCLAS30.h"

/* 去除字符串两端的空格 */
void trim( char *str )
{
        char *copied, *tail = NULL;

        if ( str == NULL )
                return;

        for( copied = str; *str; str++ )
        {
                if ( *str != ' ' && *str != '\t' )
                {
                        *copied++ = *str;
                         tail = copied;
                }
                else
                {
                         if ( tail )
                                 *copied++ = *str;
                }
        }

        if ( tail )
             *tail = 0;
        else
             *copied = 0;

        return;
}

char *urldecode(char *in)
{
    char  *out;
    char   temp[32];
    char  *p, *q;
    int             value;

    out = (char *) malloc(strlen(in) + 1);

    p = in, q = out;
    while (*p != 0) {
        if (*p == '+') {
            *q++ = ' ';
        } else if (*p == '%') {
            temp[0] = *(p + 1);
            temp[1] = *(p + 2);
            temp[2] = 0;
            sscanf(temp, "%x", &value);

            *q++ = 0xff & value;

            p += 2;
        } else {
            *q++ = *p;
        }

        p++;
    }
    *q = '\0';

    return (char *) out;
}

static void show_help(void)
{
	char *b = "HTTPCWS v" VERSION " written by Zhang Yan (KingSoft Game)\n"
		  "\n"
		   "-l <ip_addr>  interface to listen on, default is 0.0.0.0\n"
		   "-p <num>      TCP port number to listen on (default: 1985)\n"
		   "-x <path>     Chinese dictionary path (example: /usr/local/httpcws/dict)\n"
		   "-t <second>   timeout for an http request (default: 120)\n"		   
		   "-d            run as a daemon\n"
		   "-h            print this help and exit\n"	   
		   "\n";
	fprintf(stderr, b, strlen(b));
}

/* 处理模块 */
void httpcws_handler(struct evhttp_request *req, void *arg)
{	
        struct evbuffer *buf;
        buf = evbuffer_new();
		
		/* 分析URL参数 */
		struct evkeyvalq httpcws_http_query;
		evhttp_parse_query(evhttp_request_uri(req), &httpcws_http_query);
		
		/* 接收POST表单信息 */
		const char *tcsql_input_postbuffer = (const char*) EVBUFFER_DATA(req->input_buffer);		
		
		/* 接收GET表单参数 */
		const char *httpcws_input_words = evhttp_find_header (&httpcws_http_query, "w");

		const char *httpcws_output_tmp = NULL;
		char *httpcws_output_words = "\0";
		if (tcsql_input_postbuffer != NULL) {
			char *decode_uri = evhttp_decode_uri(tcsql_input_postbuffer);
			httpcws_output_tmp = ICTCLAS_ParagraphProcess(decode_uri, 0);
			free(decode_uri);
			httpcws_output_words = strdup(httpcws_output_tmp);
			trim (httpcws_output_words);
		} else if (httpcws_input_words != NULL) {
			char *decode_uri = evhttp_decode_uri(httpcws_input_words);
			httpcws_output_tmp = ICTCLAS_ParagraphProcess(decode_uri, 0);
			free(decode_uri);
			httpcws_output_words = strdup(httpcws_output_tmp);
			trim (httpcws_output_words);
		} else {
			httpcws_output_words = strdup("");
		}
		
		/* 输出内容给客户端 */
		evhttp_add_header(req->output_headers, "Server", "HTTPCWS/1.0.0");
		evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=GB2312");
		evhttp_add_header(req->output_headers, "Connection", "close");
		evbuffer_add_printf(buf, "%s", httpcws_output_words);
        evhttp_send_reply(req, HTTP_OK, "OK", buf);
		
		free(httpcws_output_words);
		evhttp_clear_headers(&httpcws_http_query);
		evbuffer_free(buf);	
}

int main(int argc, char **argv)
{
	int c;
	/* 默认参数设置 */
	char *httpcws_settings_listen = "0.0.0.0";
	int httpcws_settings_port = 1985;
	char *httpcws_settings_datapath = NULL; /*中文词典数据库路径 */
	bool httpcws_settings_daemon = false;
	int httpcws_settings_timeout = 120; /* 单位：秒 */

    /* process arguments */
    while ((c = getopt(argc, argv, "l:p:x:t:dh:")) != -1) {
        switch (c) {
        case 'l':
            httpcws_settings_listen = strdup(optarg);
            break;
        case 'p':
            httpcws_settings_port = atoi(optarg);
            break;
        case 'x':
            httpcws_settings_datapath = strdup(optarg); /* 词库文件存储路径 */
            break;
        case 't':
            httpcws_settings_timeout = atoi(optarg);
            break;			
        case 'd':
            httpcws_settings_daemon = true;
            break;
		case 'h':			
        default:
            show_help();
            return 1;
        }
    }
	
	/* 判断是否加了必填参数 -x */
	if (httpcws_settings_datapath == NULL) {
		show_help();
		fprintf(stderr, "Attention: Please use the indispensable argument: -x <path>\n\n");		
		exit(1);
	}

	/* 初始化分词组件 */
	if(!ICTCLAS_Init(httpcws_settings_datapath))
	{
	printf("%s\n", httpcws_settings_datapath);
		fprintf(stderr, "ERROR: Count not open the Chinese dictionary!\n\n");		
		exit(1);
	}
	ICTCLAS_SetPOSmap(ICT_POS_MAP_SECOND);

	fprintf(stderr, "Loading Chinese dictionary 'httpcws_dict.txt' into memory, please waitting ......\n");	
	int nCount = ICTCLAS_ImportUserDict("dict/httpcws_dict.txt");
	ICTCLAS_SaveTheUsrDic();
	printf("OK! %d words has loaded into memory.\n\n", nCount);
	printf("HTTPCWS Server running on %s:%d\n", httpcws_settings_listen, httpcws_settings_port);
		
	process_restart:
	int fd[2];
	pipe(fd);
	
	//signal(SIGCHLD,SIG_IGN);
	
	/* 如果加了-d参数，以守护进程运行 */
	if (httpcws_settings_daemon == true){
		pid_t master_pid;
		master_pid = fork();
		if (master_pid < 0) {
			exit(EXIT_FAILURE);
		}
		if (master_pid > 0) {
        exit(EXIT_SUCCESS);
		}	
	
        pid_t pid;

        /* Fork off the parent process */       
        pid = fork();
        if (pid < 0) {
                exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
			/* 通过管道发送子进程的pid号给父进程 */
			char c_pid[10];
			sprintf(c_pid, "%d", getpid());
			close(fd[0]);
			write(fd[1], c_pid, strlen(c_pid));
			wait(NULL);			
		
			/* 请求处理部分 */
			struct evhttp *httpd;

			event_init();
			httpd = evhttp_start(httpcws_settings_listen, httpcws_settings_port);
			evhttp_set_timeout(httpd, httpcws_settings_timeout);

			/* Set a callback for requests to "/specific". */
			/* evhttp_set_cb(httpd, "/select", select_handler, NULL); */

			/* Set a callback for all other requests. */
			evhttp_set_gencb(httpd, httpcws_handler, NULL);

			event_dispatch();

			/* Not reached in this code as it is now. */
			evhttp_free(httpd);
			exit(EXIT_SUCCESS);
        }
	}
	
	sleep(3000); /* 间隔300秒重启一次子进程 */
	int c_n;
	char c_mpid[10];
	close(fd[1]);
	c_n = read(fd[0], c_mpid, 10);
	if (strcmp(c_mpid, "") != 0) {
		kill(atoi(c_mpid), SIGKILL);
		wait(NULL);
	}
	memset(c_mpid, '\0', 10);
	goto process_restart;

    return 0;
}





