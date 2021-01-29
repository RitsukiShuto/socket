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

    s0 = socket(AF_INET, SOCK_STREAM, 0); // ��t�\�P�b�g�𐶐�

    /* ��t�\�P�b�g�ݒ� �|�[�g�ԍ� 9999 */
    addr.sin_family = AF_INET;   // IPv4
    addr.sin_port = htons(9999); // htons:�o�C�g�I�[�_�[���l�b�g�p�ɕϊ�
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s0, (struct sockaddr *)&addr, sizeof(addr));   // �\�P�b�g�ɖ��O��t����
    listen(s0, 10);                                     // �ڑ��҂�
    s1 = accept(s0, (struct sockaddr *)&clnt, &l_clnt); // �ڑ���t
    pid = fork();                                       // �ڑ���

    if (pid == 0)
    {

        /* child */
        while (1)
        {
            /* �\�P�b�g����f�[�^�擾 */
            l_data = read(s1, data, sizeof(data));
            if (l_data < 0)
            {
                puts("\e[30m"); // �����F�����ɖ߂�
                return 1;
            }

            printf("\e[34m%s\e[31m\n", data); // �擾�f�[�^��F�ŕ\��

            /* "bye bye"�Ȃ�ڑ���ؒf */
            if (strncmp(data, "b> bye bye", 7) == 0)
            {
                puts("\e[30m");
                close(s1);
                close(s0);
                kill(getppid(), SIGTERM); // �e�v���Z�X���I��
                puts("\e[30m");           // �����F�����ɖ߂�
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
                puts("\e[30m"); // �����F�����ɖ߂�
                free(str);
                close(s1);
                close(s0);
                return 1;
            }

            /* �R�}���h���C������ǂݍ��݁C�ԐF�ŕ\������ */
            str = readline("\e[31m");
            snprintf(outdata, sizeof(outdata), "a> %s", str);

            /* �\�P�b�g�ɏ������߂Ȃ��ꍇ�͏I������ */
            if (write(s1, outdata, strlen(outdata) + 1) < 0)
            {
                puts("\e[30m"); // �����F�����ɖ߂�
                free(str);
                close(s1);
                close(s0);
                kill(pid, SIGTERM);
                return 1;
            }

            /* "bye bye"�Ȃ�ڑ���ؒf */
            if (strncmp(str, "bye bye", 7) == 0)
            {
                puts("\e[30m"); // �����F�����ɖ߂�
                close(s1);
                free(str);
                kill(pid, SIGTERM);
                break;
            }

            free(str);
        }
    }

    /* ��t�\�P�b�g����� */
    puts("\e[30m"); // �����F�����ɖ߂�
    close(s0);

    return 0;
}