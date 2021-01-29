#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main (int argc, char *argv[ ]){
    int s0;
    struct sockaddr_in addr, clnt;
    int s1, l_clnt = sizeof(clnt);

    /*受付ソケット生成*/
    s0 = socket(AF_INET, SOCK_STREAM, 0);

    /*受付ソケット設定 ポート番号 9999*/
    addr.sin_family = AF_INET;/*IPv4*/
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s0, (struct sockaddr *)&addr, sizeof(addr));   // ソケットにポートをバインドする
    listen(s0, 10);     // 接続待ち
    s1 = accept(s0, (struct sockaddr *)&clnt, &l_clnt); // 接続受付

    write(s1, "hoge hoge", 9);      // テスト送信

    close(s1);
    close(s0);

    return 0;
}