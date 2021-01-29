// Created by RitsukiShuto on 2021/01/29.
// chat_a.c
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char *argv[])
{
    int s0;
    struct sockaddr_in addr, clnt;
    int s1, l_clnt = sizeof(clnt);
    char *str;
    char data[101];
    char outdata[101];
    int pid, l_data;

    s0 = socket(AF_INET, SOCK_STREAM, 0); // 受付ソケットを生成

    /* 受付ソケット設定 ポート番号 9999 */
    addr.sin_family = AF_INET;   // IPv4
    addr.sin_port = htons(9999); // htons:バイトオーダーをネット用に変換
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s0, (struct sockaddr *)&addr, sizeof(addr));   // ソケットに名前を付ける
    listen(s0, 10);                                     // 接続待ち
    s1 = accept(s0, (struct sockaddr *)&clnt, &l_clnt); // 接続受付
    pid = fork();                                       // 接続中

    if (pid == 0)
    {

        /* child */
        while (1)
        {
            /* ソケットからデータ取得 */
            l_data = read(s1, data, sizeof(data));
            if (l_data < 0)
            {
                puts("\e[30m"); // 文字色を黒に戻す
                return 1;
            }

            printf("\e[34m%s\e[31m\n", data); // 取得データを青色で表示

            /* "bye bye"なら接続を切断 */
            if (strncmp(data, "b> bye bye", 7) == 0)
            {
                puts("\e[30m");
                close(s1);
                close(s0);
                kill(getppid(), SIGTERM); // 親プロセスを終了
                puts("\e[30m");           // 文字色を黒に戻す
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
                puts("\e[30m"); // 文字色を黒に戻す
                free(str);
                close(s1);
                close(s0);
                return 1;
            }

            /* コマンドラインから読み込み，赤色で表示する */
            str = readline("\e[31m");
            snprintf(outdata, sizeof(outdata), "a> %s", str);

            /* ソケットに書き込めない場合は終了する */
            if (write(s1, outdata, strlen(outdata) + 1) < 0)
            {
                puts("\e[30m"); // 文字色を黒に戻す
                free(str);
                close(s1);
                close(s0);
                kill(pid, SIGTERM);
                return 1;
            }

            /* "bye bye"なら接続を切断 */
            if (strncmp(str, "bye bye", 7) == 0)
            {
                puts("\e[30m"); // 文字色を黒に戻す
                close(s1);
                free(str);
                kill(pid, SIGTERM);
                break;
            }

            free(str);
        }
    }

    /* 受付ソケットを閉じる */
    puts("\e[30m"); // 文字色を黒に戻す
    close(s0);

    return 0;
}