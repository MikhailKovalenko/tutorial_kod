#include <pthread.h>    // Библиотека для работы с потоками
#include <stdio.h>      // Стандартная библиотека ввода-вывода
#include <unistd.h>     // Системные вызовы, включая usleep
#include <cstdlib>      // Общие функции стандартной библиотеки C

pthread_mutex_t my_sync;  // Мьютекс для синхронизации доступа к общим данным
pthread_cond_t rx;        // Условная переменная для сигнализации о событии
int buffer[10];           // Общий буфер данных
int beg = 0;              // Индекс начала буфера
int end = 0;              // Индекс конца буфера
int size = 10;            // Размер буфера

// Функция записи в буфер
void *write(void *arg) {
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};  // Массив для записи в буфер
    int cur = 0;  // Индекс текущего элемента массива для записи в буфер

    while (1) {
        long int n = 500000 + rand() % 1500000;  // Случайная задержка от 500 000 до 2 000 000 микросекунд (или от 0.5 до 2 секунд).
        usleep(n);  // Засыпание на случайное время, передается в микросекундах

        pthread_mutex_lock(&my_sync);  // Блокировка мьютекса перед заполнением буфера

        while ((end - beg) >= size) {
            pthread_cond_wait(&rx, &my_sync);  // Ожидание, если буфер полон
        }

        printf("Buffer before write operation:");  // Вывод состояния буфера перед операцией записи
        for (int i = beg; i < end; ++i) {
            int ind = i % size;
            printf(" %i", buffer[ind]);
        }
        printf("\n");

        int e = end % size;
        buffer[e] = arr[cur];  // Запись элемента в буфер
        printf("Writing <%d> to buffer\n", buffer[e]);  // Вывод записанного значения

        if (++cur == 10) {
            cur = 0;  // Сброс индекса массива при достижении конца
        }

        end++;  // Увеличение индекса конца буфера

        printf("Buffer after write operation:");  // Вывод состояния буфера после операции записи
        for (int i = beg; i < end; ++i) {
            int ind = i % size;// в данном случае начало и конец буфера не обнуляются и поэтому для корректного отображения и чтобы не было выхода за границы массива
            printf(" %i", buffer[ind]);
        }
        printf("\n\n");

        pthread_cond_signal(&rx);  // Сигнал ожидающим потокам о том, что буфер изменился
        pthread_mutex_unlock(&my_sync);  // Разблокировка мьютекса
    }
}

// Функция чтения из буфера
void *read(void *arg) {
    while (1) {
        long int n = 500000 + rand() % 1500000;  // Случайная задержка
        usleep(n);  // Засыпание на случайное время

        pthread_mutex_lock(&my_sync);  // Блокировка мьютекса перед чтением из буфера

        while (beg == end) {
            pthread_cond_wait(&rx, &my_sync);  // Ожидание, если буфер пуст
        }

        printf("Buffer before read operation:");  // Вывод состояния буфера перед операцией чтения
        for (int i = beg; i < end; ++i) {
            int ind = i % size;
            printf(" %i", buffer[ind]);
        }
        printf("\n");

        int b = beg % size;
        printf("Reading <%i> from buffer\n", buffer[b]);  // Вывод прочитанного значения
        beg++;  // Увеличение индекса начала буфера

        printf("Buffer after read operation:");  // Вывод состояния буфера после операции чтения
        for (int i = beg; i < end; ++i) {
            int ind = i % size;
            printf(" %i", buffer[ind]);
        }
        printf("\n\n");

        pthread_cond_signal(&rx);  // Сигнал ожидающим потокам о том, что буфер изменился
        pthread_mutex_unlock(&my_sync);  // Разблокировка мьютекса
    }
}

int main() {
    srand(time(NULL));  // Инициализация генератора случайных чисел
    pthread_t id1, id2;  // Идентификаторы потоков

    pthread_mutex_init(&my_sync, NULL);  // Инициализация мьютекса
    pthread_cond_init(&rx, NULL);  // Инициализация условной переменной

    pthread_create(&id1, NULL, write, NULL);  // Создание потока для записи
    pthread_create(&id2, NULL, read, NULL);   // Создание потока для чтения

    pthread_join(id1, NULL);  // Ожидание завершения потока записи
    pthread_join(id2, NULL);  // Ожидание завершения потока чтения

    return 0;
}
