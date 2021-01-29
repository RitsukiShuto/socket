#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[ ]){
    struct sockaddr_in serv;
    int s0;
    char data[101];
    int l_data, l_serv = sizeof(serv);

    memset(data, '\0', sizeof(data));   // データをNULLで埋めて初期化
    s0 = socket (AF_INET, SOCK_STREAM, 0);  // ソケット生成

    /*サーバ情報設定 localhost ポート番号9999*/
    serv.sin_family = AF_INET;  // IPv4
    serv.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);

    /*サーバへ接続要求*/
    if (connect(s0, (struct sockaddr *)&serv, l_serv) == -1){
        perror("connect");
    }

    l_data = read(s0, data, sizeof(data));      // ソケットからデータ取得
    printf("%s\n", data);       // 取得データ表示

    close(s0);

    return 0;
}