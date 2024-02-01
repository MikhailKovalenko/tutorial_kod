#include <iostream> //библиотека используется для ввода и вывода данных в консоль.
#include <unistd.h> //библиотека предоставляет набор функций для работы с системными вызовами, такими как fork(), exec().
#include <sys/types.h> //библиотека содержит определения различных типов данных, используемых в системных вызовах, такие как pid_t.
#include <sys/wait.h> //библиотека предоставляет функции для ожидания завершения дочерних процессов с использованием waitpid().
#include <ctime> //библиотека используется для работы с временем и датой.
#include <cerrno> //библиотека содержит объявления для кодов ошибок, которые могут возникнуть при вызове системных функций.
#include <vector> //библиотека предоставляет контейнерный класс vector, который используется для хранения последовательности pid_t.

using namespace std;

void logProcessInfo_start(int child_pid, int processNum, const char* message) {//где * это указатель в c++
    time_t currentTime;//объявляет переменную currentTime с типом time_t, который используется для представления времени в секундах.
    time(&currentTime);//получаем текущее время Unix timestamp
    cout << "PID " << child_pid << " PPID " << getpid() << " Process " << processNum << ": " << message << " at " << ctime(&currentTime);//ctime() используется для преобразования времени, представленного в формате Unix timestamp (хранящегося в переменной currentTime), в строку, представляющую дату и время в удобочитаемом формате.
}

void logProcessInfo_end(int pid, const char* message) {
    time_t currentTime;
    time(&currentTime);
    cout << "PID " << pid << " PPID " << getpid() << ": " << message << " at " << ctime(&currentTime);
}


int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <process_1> <process_2> <process_3> <process_4>" << endl;
        return 1;
    }

    vector<int> processOrder;// создается вектор, в который добавляются номера выбранных процессов.
    for (int i = 1; i <= 4; i++) {
        int processNum = atoi(argv[i]);// перевод входного параметра строки в число
        if (processNum < 1 || processNum > 4) {// проверка на корректность ввода
            cerr << "Invalid process number: " << processNum << endl;
            return 1;
        }
        processOrder.push_back(processNum);//добавление номера процесса в вектор
    }

    vector<pid_t> child_pids;//Создается вектор для хранения PID дочерних процессов.

    for (int i : processOrder) {
        pid_t child_pid = fork();//создание процесса с сохранением его PID в переменную child_pid
        if (child_pid == 0) {
            // Этот код выполняется в дочернем процессе
            switch (i) {
            case 1:
                {
                    char *file = "/usr/bin/echo";
                    char *const args[] = {"/usr/bin/echo", "Hello execv!",NULL};
                    execv(file,args);
                }
                break;
            case 2:
                {
                    char *file = "/usr/bin/echo";
                    char *arg1 = "Hello execl!";
                    execl(file, file, arg1, NULL);
                }
                break;
            case 3:
                {
                    char *file = "echo";
                    char *const args[] = {"/usr/bin/echo","Hello execvp!",NULL};

                    execvp(file,args);
                }
                break;
            case 4:
                {
                    char *file = "echo";
                    char *arg1 = "Hello execlp!";
                    execlp(file, file, arg1, NULL);
                }
                break;
            default:
                cerr << "Invalid process number: " << i << endl;
                exit(1);
            }
            // В случае ошибки вызова exec, выводим сообщение об ошибке
            perror("Error");
            exit(1);
        } else if (child_pid < 0) {
            perror("Fork failed");
            return 1;
        } else {
            child_pids.push_back(child_pid);//добавление PID созданного процесса в вектор процессов
            switch (i) {
            case 1:
                logProcessInfo_start(child_pid, i, "Using execv()");
                break;
            case 2:
                logProcessInfo_start(child_pid, i, "Using execl()");
                break;
            case 3:
                logProcessInfo_start(child_pid, i, "Using execvp()");
                break;
            case 4:
                logProcessInfo_start(child_pid, i, "Using execlp()");
                break;
            }
        }
    }

    for (pid_t pid : child_pids) {//завершение созданных процессов
        int status;
        waitpid(pid, &status, 0);//Для каждого PID дочернего процесса вызывается waitpid(), чтобы дождаться завершения процесса.
        logProcessInfo_end(pid, "Exited");
    }

    return 0;
}
