#include <iostream> // Подключение библиотеки для ввода и вывода
#include <fstream> // Подключение библиотеки для работы с файлами
#include <string> // Подключение библиотеки для работы со строками
#include <thread> // используется для работы с многозадачностью и создания потоков.
#include <vector> // Подключение библиотеки для работы с векторами
#include <unistd.h> // Подключение библиотеки для доступа к системным функциям
#include <ctime> // Подключение библиотеки для работы со временем
#include <pthread.h> //используется для работы с потоками с использованием библиотеки Pthreads.

using namespace std;

// Функция для записи в файл
void writeToOwnFile(int threadID) {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    time_t currentTime = time(nullptr);
    string fileName = to_string(pid) + "_" + to_string(threadID) + ".txt";

    int fileSize = rand() % 901 + 100;// Генерируем случайное количество символов от 100 до 1000

    ofstream file(fileName, ios::out | ios::binary); // Открытие файла для записи в бинарном режиме ios::out - это флаг, указывающий, что мы хотим открыть файл для записи (output). ios::binary - это флаг, указывающий, что мы открываем файл в бинарном режиме. ofstream (Output File Stream) - это класс, который используется для записи данных в файл.
    if (file.is_open()) {
        string data(fileSize, 'A');  // Генерируем строку символов
        file.write(data.c_str(), fileSize);
        file.close();
    } else {
        cerr << "Thread " << threadID << " Error: Unable to create file " << fileName << endl;
    }

    cout << "Thread ID: " << threadID << ", PID: " << pid << ", PPID: " << ppid
         << ", Time: " << ctime(&currentTime)
         << "File Name: " << fileName << ", Characters Written: " << fileSize << endl;
}

// Функция для чтения из файла
void readFromOwnFile(int threadID) {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    time_t currentTime = time(nullptr);

    string fileName = to_string(pid) + "_" + to_string(threadID - 1) + ".txt";

    ifstream file(fileName, ios::in | ios::binary); // Открытие файла для чтения в бинарном режиме ifstream (Input File Stream) - это класс, предназначенный для чтения данных из файла.
    if (file.is_open()) {
        file.seekg(0, ios::end);
        int fileSize = file.tellg();
        file.close();

        cout << "Thread ID: " << threadID << ", PID: " << pid << ", PPID: " << ppid
             << ", Time: " << ctime(&currentTime)
             << "File Name: " << fileName << ", Characters Read: " << fileSize << endl;
    } else {
        cerr << "Thread " << threadID << " Error: Unable to open file " << fileName << endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <number of threads>" << endl;
        return 1;
    }

    int numThreads = stoi(argv[1]); // Получение количества потоков из аргументов командной строки
    if (numThreads % 2 != 0) { // Проверка на четное количество потоков
        cerr << "Number of threads must be even." << endl;
        return 1;
    }

    vector<thread> threads; // Создание вектора для хранения потоков
    for (int i = 1; i <= numThreads; ++i) {
        if (i % 2 == 1) {
            threads.emplace_back(writeToOwnFile, i); // Создание потока для записи
        } else {
            threads.emplace_back(readFromOwnFile, i); // Создание потока для чтения
        }//emplace_back - добавляет оригинал элемента (не копию) в вектор 
     }

    for (auto &thread : threads) {
        thread.join(); // Ожидание завершения работы каждого потока
    }

    return 0;
}
