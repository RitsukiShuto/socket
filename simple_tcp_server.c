#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main (int argc, char *argv[ ]){
    int s0;
    struct sockaddr_in addr, clnt;
    int s1, l_clnt = sizeof(clnt);

    /*��t�\�P�b�g����*/
    s0 = socket(AF_INET, SOCK_STREAM, 0);

    /*��t�\�P�b�g�ݒ� �|�[�g�ԍ� 9999*/
    addr.sin_family = AF_INET;/*IPv4*/
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s0, (struct sockaddr *)&addr, sizeof(addr));   // �\�P�b�g�Ƀ|�[�g���o�C���h����
    listen(s0, 10);     // �ڑ��҂�
    s1 = accept(s0, (struct sockaddr *)&clnt, &l_clnt); // �ڑ���t

    write(s1, "hoge hoge", 9);      // �e�X�g���M

    close(s1);
    close(s0);

    return 0;
}