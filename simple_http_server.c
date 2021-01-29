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

    memset(str_in, '\0', sizeof(str_in));   // itr_in��NULL�����ŏ�����

    /* ��t�\�P�b�g�𐶐� */
    if((s0 = sockert(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("soket");
        return 1;
    }

    setsockpt(s0, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));     // ��t�\�P�b�g���ė��p�\�ɂ���

    /* ��t�\�P�b�g�ݒ� �|�[�g�ԍ�:8080 */
    addr.sin_family = AF_INET;      // IPv4
    addr.sin_port = htons(8080);     // htons: �o�C�g�I�[�_���l�b�g�p�Ɍ���

    /* �\�P�b�g�ɃA�h���X�����蓖�Ă� */
    if((bind(s0, (struct sockaddr *) & addr, sizeof(addr))) < 0){
        perror("bind");
        close(s0);
        return 1;
    }

    /* �ڑ��҂� */
    if(listen(s0, 10) < 0){
        perror("listen");
        close(s0);
        return 1;
    }

    /* �ڑ���t */
    while(1){
        if((s1 = acept(s0, (struct sockaddr *) &clnt,&l_clnt)) < 0){
            perror("accept");
            close(s0);
            return 0;
        }
    

        /* http���N�G�X�g�𑗐M */
        m = read(s1, str_in, sizeof(str_in));

        /* HTML�Œ��g���L�q */
        snprintf(content, sizeof(content),
"<html><body>\r\n\
���Ȃ��̑�����HTTP���N�G�X�g�͈ȉ��̂Ƃ���ł��D\r\n
<pre>%s</pre>\r\n
</body></html>", str_in);

        /* �w�b�_�ƒ��g�ŉ������b�Z�[�W */
        snprintf(str_out, sizeof(str_out),
"HTTP/1.0 200 OK\r\n
Content-Length: %d\r\n
Content-Type: text/html\r\n
\r\n
%s", strlen(content) + 1, content);

        /* �������b�Z�[�W�𑗐M */
        write(s1, str_out, strlen(str_out) + 1);
        close(s1);  // �ڑ���ؒf
    }

    close(s0);  // �\�P�b�g�����

    return 0;
}