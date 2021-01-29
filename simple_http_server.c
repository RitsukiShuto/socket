// Created by RitsukiShuto on 2021/01/25.
// simple_tcp_server.c
//
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(int argc, char *argv[]){
    int s0, s1;
    struct sockaddr_in addr, clnt;
    int l_clnt = sizeof(clnt), m;
    const int reuse = 1;
    char str_in[4096];
    char content[4096];
    char str_out[4096];

    memset(str_in, '\0', sizeof(str_in));   // itr_inをNULL文字で初期化

    /* 受付ソケットを生成 */
    if((s0 = sockert(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("soket");
        return 1;
    }

    setsockpt(s0, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));     // 受付ソケットを再利用可能にする

    /* 受付ソケット設定 ポート番号:8080 */
    addr.sin_family = AF_INET;      // IPv4
    addr.sin_port = htons(8080);     // htons: バイトオーダをネット用に交換

    /* ソケットにアドレスを割り当てる */
    if((bind(s0, (struct sockaddr *) & addr, sizeof(addr))) < 0){
        perror("bind");
        close(s0);
        return 1;
    }

    /* 接続待ち */
    if(listen(s0, 10) < 0){
        perror("listen");
        close(s0);
        return 1;
    }

    /* 接続受付 */
    while(1){
        if((s1 = acept(s0, (struct sockaddr *) &clnt,&l_clnt)) < 0){
            perror("accept");
            close(s0);
            return 0;
        }
    

        /* httpリクエストを送信 */
        m = read(s1, str_in, sizeof(str_in));

        /* HTMLで中身を記述 */
        snprintf(content, sizeof(content),
"<html><body>\r\n\
あなたの送ったHTTPリクエストは以下のとおりです．\r\n
<pre>%s</pre>\r\n
</body></html>", str_in);

        /* ヘッダと中身で応答メッセージ */
        snprintf(str_out, sizeof(str_out),
"HTTP/1.0 200 OK\r\n
Content-Length: %d\r\n
Content-Type: text/html\r\n
\r\n
%s", strlen(content) + 1, content);

        /* 応答メッセージを送信 */
        write(s1, str_out, strlen(str_out) + 1);
        close(s1);  // 接続を切断
    }

    close(s0);  // ソケットを閉じる

    return 0;
}