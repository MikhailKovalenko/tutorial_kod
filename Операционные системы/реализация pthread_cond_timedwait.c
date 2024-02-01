#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <pthread.h>
#include <csignal>
#include <signal.h>

// Глобальные переменные
pthread_mutex_t my_sync;   // Мьютекс для синхронизации потоков
pthread_cond_t rx;         // Условная переменная для сигнализации потокам
pthread_t kidA, kidB;       // Идентификаторы потоков
pthread_attr_t attr;        // Атрибуты потока
pthread_t thred_unlock;     // Поток для ожидания таймера

int t; // Время ожидания в миллисекундах

// Структура для передачи аргументов в поток mywait
typedef struct someArgs_tag {
    unsigned int timetowait;  // Время ожидания в миллисекундах
    int out;                   // Результат ожидания (1 - успех, 0 - неудача)
} someArgs_t;

// Функция для ожидания заданного времени
void *mywait(void *args) {
    someArgs_t *arg = (someArgs_t *)args;
    int timetowait = arg->timetowait;
    usleep(timetowait);
    arg->out = 1;  // Установка флага успеха
    pthread_cond_signal(&rx);  // Сигнализация основному потоку
    printf("Конец потока mywait\n");
}

// Обертка для функции pthread_cond_timedwait
int lab_pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, unsigned int timetowait) {
    someArgs_t timetowait_s;
    timetowait_s.timetowait = timetowait;
    pthread_create(&thred_unlock, &attr, mywait, (void *)&timetowait_s);  // Создание потока для ожидания
    int VALUE = pthread_cond_wait(cond, mutex);  // Ожидание сигнала

    // Обработка результата
    if (VALUE != 0) {
        pthread_join(thred_unlock, NULL);
        return -1;  // Ошибка ожидания
    } else if (timetowait_s.out == 0) {
        pthread_join(thred_unlock, NULL);
        return 0;   // Прошло время, но сигнал не пришел
    } else if (timetowait_s.out == 1){
        pthread_join(thred_unlock, NULL);
        return 1;   // Успешное ожидание
    } else {
        pthread_join(thred_unlock, NULL);
        return -1;  // Неопределенная ошибка
    }
}

// Поток B, который отправляет сигнал через условную переменную
void *streamB(void *) {
    usleep(1000000); // 1 секунда
    pthread_cond_signal(&rx);  // Отправка сигнала потоку A
    printf("Конец потока B\n");
}

// Поток A, который ожидает сигнала от потока B
void *streamA(void *) {
    int res;
    res = lab_pthread_cond_timedwait(&rx, &my_sync, t);  // Ожидание сигнала с таймером

    // Вывод результата ожидания
    if (res == 0){
        printf("Поток B успел воспользоваться мьютексом. Вернулось: %d\n", res);
    } else if (res == 1){
        printf("Поток B НЕ успел воспользоваться мьютексом. Вернулось: %d\n", res);
    } else {
        printf("cond_error:\n");
    }
    printf("Конец потока A\n");
}

int main(int argc, char **argv) {
    // Проверка количества входных параметров
    if (argc != 2) {
        fprintf(stderr,"Usage: %s <number of time>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    t = atoi(argv[1]);  // Получение времени ожидания из аргументов командной строки
    pthread_mutex_init(&my_sync, NULL);   // Инициализация мьютекса
    pthread_cond_init(&rx, NULL);         // Инициализация условной переменной
    pthread_attr_init(&attr);             // Инициализация атрибутов потока

    // Создание потоков A и B
    pthread_create(&kidA, &attr, streamA, NULL);
    pthread_create(&kidB, &attr, streamB, NULL);

    // Ожидание завершения потоков
    pthread_join(kidA, NULL);
    pthread_join(kidB, NULL);

    return 0;
}
