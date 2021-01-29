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

    memset(data, '\0', sizeof(data));       // data��NULL�����Ŗ��߂ď�����
    s0 = socket(AF_INET, SOCK_STREAM, 0);   // �\�P�b�g�𐶐�

    /*�T�[�o���ݒ� localhost �|�[�g�ԍ�9999*/
    serv.sin_family = AF_INET;      // IPv4
    serv.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);

    /*�T�[�o�֐ڑ��v��*/
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
            /*�\�P�b�g����f�[�^�擾*/
            l_data = read(s0, data, sizeof(data));
            if (l_data < 0)
            {
                puts("\e[30m"); // �����F�����ɖ߂�
                return 1;
            }

            printf("\e[31m%s\e[34m\n", data);       // �擾�f�[�^��ԕ����ŕ\��
            /* "bye bye"�Őڑ���ؒf */
            if (strncmp(data, "a> bye bye", 7) == 0)
            {
                close(s0);

                /* �e�v���Z�X���I�������� */
                kill(getppid(), SIGTERM);
                puts("\e[30m");     // �����F�����ɖ߂�
                return 1;
            }
        }
    }
    else
    {
        /* parent */
        while (1)
        {
            /* �q�v���Z�X�����񂾏ꍇ�͏I������ */
            if (kill(pid, 0) != 0)
            {
                puts("\e[30m");     // �����F�����ɖ߂�
                free(str);
                close(s0);
                return 1;
            }
            /* �����̃R�}���h���C������ǂݍ��݁C�F�ŕ\������ */
            str = readline("\e[34m");
            snprintf(outdata, sizeof(outdata), "b> %s", str);

            /* �\�P�b�g�ɏ������߂Ȃ��ꍇ�͏I������ */
            if (write(s0, outdata, strlen(outdata) + 1) < 0)
            {
                puts("\e[30m");     // �����F�����ɖ߂�
                free(str);
                close(s0);
                kill(pid, SIGTERM);
                return 1;
            }

            /* "bye bye"�Ȃ�ΐڑ���ؒf */
            if (strncmp(str, "bye bye", 7) == 0)
            {
                puts("\e[30m");     // �����F�����ɖ߂�
                free(str);
                kill(pid, SIGTERM);
                break;
            }
            free(str);
        }
    }
    puts("\e[30m"); // �����F�����ɖ߂�
    close(s0);
    return 0;
}