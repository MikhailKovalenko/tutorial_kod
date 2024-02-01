#include <stdio.h>      // Стандартная библиотека ввода-вывода
#include <stdlib.h>     // Стандартная библиотека общего назначения
#include <time.h>       // Библиотека для работы с временем
#include <sys/types.h>  // Типы данных для системных вызовов
#include <sys/wait.h>   // Заголовочный файл для функций ожидания завершения дочернего процесса
#include <sys/errno.h>  // Системные ошибки

int main(int argc, char *argv[]) {
    if (argc !=2){// проверка количества вводимых параметров программы с консоли
        fprintf(stderr,"Usage: %s <size of bufer>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    // Инициализация генератора случайных чисел
    srand(time(NULL));
    long int t;
    long int N = atoi(argv[1]);  // Получение размера N из аргументов командной строки
    int fd[2], nread = 1;

    // Создание канала (pipe) для обмена данными между процессами
    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid_t child_pid;
    int status = 0;

    int buf_size = N;
    long int *buf = (long int*)malloc(sizeof(long int) * buf_size);  // Выделение памяти под буфер данных для отправляемых данных
    long int *buf_end = (long int*)malloc(sizeof(long int) * buf_size);  // Выделение памяти под буфер для принимаемых данных

    // Инициализация буфера данных
    for (long int i = 2; i < buf_size; i++) {
        buf[i] = i;
    }

    // Создание дочернего процесса
    if ((child_pid = fork()) < 0) {
        perror("fork failed");
        exit(2);
    }

    // Ветвь, выполняемая дочерним процессом
    if (child_pid == 0) {
        close(fd[1]);  // Закрытие дескриптора канала для записи

        int count_read = 0;
        // Чтение данных из канала
        while (nread != 0) {
            nread = read(fd[0], &(buf_end[count_read]), 1024);
            count_read = count_read + nread / sizeof(long int);
        }

        close(fd[0]);  // Закрытие дескриптора канала для чтения

        // Замер времени завершения передачи данных
        struct timespec end;
        clock_gettime(CLOCK_REALTIME, &end);

        int begin_tv_nsec = buf_end[0];
        int begin_tv_sec = buf_end[1];
        t = 1000000000 * (end.tv_sec - begin_tv_sec) + (end.tv_nsec - begin_tv_nsec);

        double dt = (double)t / 1000000.0;  // Преобразование времени в миллисекунды

        // Вывод информации о завершении передачи
        printf("The transfer is over. Transmission time - %f мс\n\n", dt);
        printf("The transmission begins. Transmitted number of bytes: %li\n", N * sizeof(long int));
        exit(0);  // Завершение дочернего процесса
    } else {  // Ветвь, выполняемая родительским процессом
        // Замер времени начала передачи данных
        struct timespec begin;
        clock_gettime(CLOCK_REALTIME, &begin);
        buf[0] = begin.tv_nsec;
        buf[1] = begin.tv_sec;

        close(fd[0]);  // Закрытие дескриптора канала для чтения

        write(fd[1], buf, buf_size * sizeof(long int)); // Запись данных в канал
        close(fd[1]);  // Закрытие дескриптора канала для записи

        while ((wait(&status)) > 0); // Ожидание завершения дочернего процесса

        // Освобождение выделенной памяти
        free(buf);
        free(buf_end);

        exit(0);  // Завершение родительского процесса
    }
}

