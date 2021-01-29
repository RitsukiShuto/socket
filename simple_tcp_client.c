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

    memset(data, '\0', sizeof(data));   // �f�[�^��NULL�Ŗ��߂ď�����
    s0 = socket (AF_INET, SOCK_STREAM, 0);  // �\�P�b�g����

    /*�T�[�o���ݒ� localhost �|�[�g�ԍ�9999*/
    serv.sin_family = AF_INET;  // IPv4
    serv.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);

    /*�T�[�o�֐ڑ��v��*/
    if (connect(s0, (struct sockaddr *)&serv, l_serv) == -1){
        perror("connect");
    }

    l_data = read(s0, data, sizeof(data));      // �\�P�b�g����f�[�^�擾
    printf("%s\n", data);       // �擾�f�[�^�\��

    close(s0);

    return 0;
}