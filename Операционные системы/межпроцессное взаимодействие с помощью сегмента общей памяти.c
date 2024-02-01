#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <ctime>

key_t sem_key = 2045; // Произвольный ключ для системного семафора
int semid; // семафор для синхронизации процессов

const size_t BITS_PER_BYTE = 8;
const size_t BYTES_PER_COPY = 1024 / BITS_PER_BYTE; // Число байтов для копирования


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <size of buffer>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    pid_t child_pid;
    long int *buf;
    int buf_size = atoi(argv[1]);
    int shmid;

    if ((semid = semget(sem_key, 1, 0666 | IPC_CREAT)) == -1) {//создание семафора
        perror("Ошибка создания системного семафора");
        exit(1);
    }

    semctl(semid, 0, SETVAL, 0);//инициализация начального значения для семафора

    // Создание нового процесса
    if ((child_pid = fork()) < 0) {
        printf("Ошибка создания процесса\n");
        exit(1);
    }

    if (child_pid != 0) { // Родительский процесс
        printf("Начинается передача данных. Передаваемое число байт: %lu\n", buf_size * sizeof(long int));

        struct timespec begin;
        clock_gettime(CLOCK_REALTIME, &begin);

        printf("\nРодитель создает общую память\n");

        // Создание или подключение к общей памяти
        if ((shmid = shmget(2044, buf_size * sizeof(long int), 0666 | IPC_CREAT)) == -1) {
            printf("Ошибка создания общей памяти\n");
            exit(1);
        }

        // Присоединение общей памяти к адресному пространству процесса
        buf = (long int *)shmat(shmid, 0, 0);

        if (buf == (long int *)-1) {
            printf("Ошибка при присоединении к общей памяти\n");
            exit(2);
        }

        printf("\nРодитель создал общую память\n");

        // Заполнение буфера данными
        for (long int i = 2; i < buf_size; i += BYTES_PER_COPY) {
            size_t copy_size = std::min(BYTES_PER_COPY, static_cast<size_t>(buf_size - i));
            if (i + copy_size <= buf_size) {
                memset(buf + i, i, copy_size);
                struct sembuf sem_post_buf = {0, 1, 0}; // Увеличиваем значение семафора на 1
                semop(semid, &sem_post_buf, 1); // Разблокируем семафор, чтобы разрешить родительскому процессу завершить запись
            }
        }

        printf("\nРодитель закончил запись\n");

        struct sembuf sem_wait_buf = {0, -1, 0}; // Уменьшаем значение семафора на 1
        semop(semid, &sem_wait_buf, 1);

        struct timespec end;
        clock_gettime(CLOCK_REALTIME, &end);

        // Измерение времени передачи данных
        long int t = 1000000000 * (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
        double dt = (double)t / 1000000.0;
        printf("\nПередача закончена, Время передачи: %f мс\n\n", dt);

        shmdt(buf);
        if (shmctl(shmid, IPC_RMID, NULL) == -1) exit(-1);
        // Уничтожаем системный семафор
        if (semctl(semid, 0, IPC_RMID) == -1) {
            perror("Ошибка удаления системного семафора");
            exit(1);
        }

        exit(0);
    } else if (child_pid == 0) { // Дочерний процесс
        // Подключение к общей памяти
        if ((shmid = shmget(2044, buf_size * sizeof(long int), 0)) == -1) {
            printf("Ошибка создания общей памяти\n");
            exit(1);
        }
        buf = (long int *)shmat(shmid, 0, 0);
        if (buf == (long int *)-1) {
            printf("Ошибка при присоединении к общей памяти\n");
            exit(2);
        }

        printf("\nДочерний присоединился к сегменту общей памяти\n");

        // Читаем из общего сегмента памяти
        for (size_t i = 0; i < buf_size; i += BYTES_PER_COPY) {
            struct sembuf sem_wait_buf = {0, -1, 0}; // Уменьшаем значение семафора на 1
            semop(semid, &sem_wait_buf, 1);
            size_t copy_size = std::min(BYTES_PER_COPY, buf_size - i);
            if (i + copy_size <= buf_size) {
                memcpy(buf + i, buf + i, copy_size);
            }
        }

        printf("\nДочерний закончил читать общую память\n");

        struct sembuf sem_post_buf = {0, 1, 0}; // Увеличиваем значение семафора на 1
        semop(semid, &sem_post_buf, 1); // Разблокируем семафор, чтобы разрешить родительскому процессу завершить запись

        exit(0);
    }
    return 0;
}
