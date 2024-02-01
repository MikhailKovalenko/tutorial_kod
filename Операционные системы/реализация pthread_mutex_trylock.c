#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <pthread.h>

pthread_mutex_t my_sync;  // Глобальная переменная для мьютекса

pthread_t kidA, kidB;  // Глобальные переменные для идентификаторов потоков
int anykey;  // Глобальная переменная для хранения результата попытки блокировки мьютекса

// Функция для попытки блокировки мьютекса с таймаутом
int lab_pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    int ret;
    struct timespec timeoutTime;

    // Получаем текущее время
    clock_gettime(CLOCK_REALTIME, &timeoutTime);
    // Добавляем полсекунды к текущему времени
    timeoutTime.tv_nsec += 500000000;

    // Пытаемся заблокировать мьютекс с таймаутом
    ret = pthread_mutex_timedlock(mutex, &timeoutTime);

    // Проверяем результат попытки блокировки мьютекса
    if (ret == 0) {
        printf("Мьютекс свободен\n");
        return 0;
    } else if (ret == ETIMEDOUT) {
        printf("Мьютекс занят\n");
        return 1;
    } else {
        perror("mutex: ");
        return -1;
    }
}

// Функция для выполнения действий в потоке A
void *streamA(void *arg)
{
    while (1)
    {
        sleep(1);
        printf("Поток A пытается заблокировать мьютекс\n");
        pthread_mutex_lock(&my_sync);
        printf("Поток A блокирует мьютекс\n");
        sleep(1.5);
        printf("Поток A продолжает блокировать мьютекс\n");
        pthread_mutex_unlock(&my_sync);
        printf("Поток A разблокировал мьютекс\n");
    }
}

// Функция для выполнения действий в потоке B
void *streamB(void *arg)
{
    while (1)
    {
        sleep(3);
        printf("Поток B пытается заблокировать мьютекс\n");
        anykey = lab_pthread_mutex_trylock(&my_sync);
        if (anykey == 0)
        {
            printf("Поток B может заблокировать мьютекс. Вернулся: %d\n", anykey);
            pthread_mutex_unlock(&my_sync);
        }
        else if (anykey == 1)
            printf("Поток B будет заблокирован. Вернулся: %d\n", anykey);
        else if (anykey == -1)
            printf("Ошибка. Вернулся: %d\n", anykey);
        printf("Поток B заканчивается.\n");
    }
}

int main()
{
    pthread_attr_t attr;

    // Инициализация атрибутов потока и мьютекса
    pthread_attr_init(&attr);
    pthread_mutex_init(&my_sync, NULL);
    
    // Создание и запуск потоков
    pthread_create(&kidA, &attr, streamA, NULL);
    pthread_create(&kidB, &attr, streamB, NULL);

    // Ожидание завершения потоков
    pthread_join(kidA, NULL);
    pthread_join(kidB, NULL);

    return 0;
}
