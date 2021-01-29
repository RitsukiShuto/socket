// Created by RitsukiShuto on 2021/01/29.
// chat_a.c
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <readline/readline.h>
#include <readline/history.h>
int main(int argc, char *argv[])
{
    struct sockaddr_in serv;
    int s0;
    char data[101];
    char outdata[101];
    char *str;
    int l_data, l_serv = sizeof(serv), pid;

    memset(data, '\0', sizeof(data));       // dataをNULL文字で埋めて初期化
    s0 = socket(AF_INET, SOCK_STREAM, 0);   // ソケットを生成

    /*サーバ情報設定 localhost ポート番号9999*/
    serv.sin_family = AF_INET;      // IPv4
    serv.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);

    /*サーバへ接続要求*/
    if (connect(s0, (struct sockaddr *)&serv, l_serv) == -1)
    {
        perror("connect");
    }

    pid = fork();

    if (pid == 0)
    {
        /*child*/
        while (1)
        {
            /*ソケットからデータ取得*/
            l_data = read(s0, data, sizeof(data));
            if (l_data < 0)
            {
                puts("\e[30m"); // 文字色を黒に戻す
                return 1;
            }

            printf("\e[31m%s\e[34m\n", data);       // 取得データを赤文字で表示
            /* "bye bye"で接続を切断 */
            if (strncmp(data, "a> bye bye", 7) == 0)
            {
                close(s0);

                /* 親プロセスを終了させる */
                kill(getppid(), SIGTERM);
                puts("\e[30m");     // 文字色を黒に戻す
                return 1;
            }
        }
    }
    else
    {
        /* parent */
        while (1)
        {
            /* 子プロセスが死んだ場合は終了する */
            if (kill(pid, 0) != 0)
            {
                puts("\e[30m");     // 文字色を黒に戻す
                free(str);
                close(s0);
                return 1;
            }
            /* 自分のコマンドラインから読み込み，青色で表示する */
            str = readline("\e[34m");
            snprintf(outdata, sizeof(outdata), "b> %s", str);

            /* ソケットに書き込めない場合は終了する */
            if (write(s0, outdata, strlen(outdata) + 1) < 0)
            {
                puts("\e[30m");     // 文字色を黒に戻す
                free(str);
                close(s0);
                kill(pid, SIGTERM);
                return 1;
            }

            /* "bye bye"ならば接続を切断 */
            if (strncmp(str, "bye bye", 7) == 0)
            {
                puts("\e[30m");     // 文字色を黒に戻す
                free(str);
                kill(pid, SIGTERM);
                break;
            }
            free(str);
        }
    }
    puts("\e[30m"); // 文字色を黒に戻す
    close(s0);
    return 0;
}