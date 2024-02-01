import winrm
import socket
import json

# Создание сеанса WinRM
session = winrm.Session('address', auth=('user@domain', 'password'), transport='ntlm')

ps_script = '''
chcp 65001 # Установка кодовой страницы в UTF-8
# Получить текущее время
$date = Get-Date

# Определить начальную дату как текущее время минус 15 минут
$begindate = $date.AddMinutes(-15)

# Перевести даты в формат WMI
$wmibegindate = [System.Management.ManagementDateTimeConverter]::ToDMTFDateTime($begindate)
$wmienddate = [System.Management.ManagementDateTimeConverter]::ToDMTFDateTime($date)

$eventArray = @()

$journal = @(
    "Security", 
    "Application", 
    "System", 
    "Setup", 
    "Windows PowerShell"
    )

foreach ($log in $journal) {
    $events = Get-WmiObject -Class Win32_NTLogEvent -Filter "LogFile='$log' AND TimeGenerated >= '$($wmibegindate)' AND TimeGenerated <= '$($wmienddate)'"
    foreach ($event in $events) {
        $eventData = @{
            EventID           = $event.EventCode
            TimeCreated       = $event.TimeGenerated
            Logfile           = $event.Logfile
            Message 	      = $event.Message
        }
        if ($event.EventCode -eq 4624) {
            # Извлекаем информацию о сетевых адресах с помощью регулярных выражений
            $networkInfo = $event.Message -match "Тип входа:\s+(\d+)[\s\S]+Новый вход:[\s\S]+Имя учетной записи:\s+(\S+)\s+Домен учетной записи:\s+(\S+)[\s\S]+Имя процесса:\s+(.*\S)"
            
            if ($networkInfo) {
                $eventData | Add-Member -MemberType NoteProperty -Name "InputType" -Value $Matches[1]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountName" -Value $Matches[2]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountDomain" -Value $Matches[3]
                $eventData | Add-Member -MemberType NoteProperty -Name "ProcessName" -Value $Matches[4]
            }
        }

        if ($event.EventCode -eq 4672) {
            # Извлекаем информацию о сетевых адресах с помощью регулярных выражений
            $networkInfo = $event.Message -match "Имя учетной записи:\s+(\S+)\s+Домен учетной записи:\s+(\S+)"
            
            if ($networkInfo) {
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountName" -Value $Matches[1]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountDomain" -Value $Matches[2]
            }
        }

        if ($event.EventCode -eq 4688) {
            # Извлекаем информацию о сетевых адресах с помощью регулярных выражений
            $networkInfo = $event.Message -match "Имя учетной записи:\s+(\S+)\s+Домен учетной записи:\s+(\S+)[\s\S]+ИД нового процесса:\s+(\S+)\s+Имя нового процесса:\s+(\S+)[\s\S]+ИД процесса-создателя:\s+(\S+)\s+Имя процесса-создателя:\s+(\S+)\s+Командная строка процесса:\s+(.*\S)"
            
            if ($networkInfo) {
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountName" -Value $Matches[1]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountDomain" -Value $Matches[2]
                $eventData | Add-Member -MemberType NoteProperty -Name "PID" -Value $Matches[3]
                $eventData | Add-Member -MemberType NoteProperty -Name "ProcessName" -Value $Matches[4]
                $eventData | Add-Member -MemberType NoteProperty -Name "PPID" -Value $Matches[5]
                $eventData | Add-Member -MemberType NoteProperty -Name "ParentProcessName" -Value $Matches[6]
                $eventData | Add-Member -MemberType NoteProperty -Name "ProcessCommandLine" -Value $Matches[7]
            }
        }

        if ($event.EventCode -eq 4689) {
            # Извлекаем информацию о сетевых адресах с помощью регулярных выражений
            $networkInfo = $event.Message -match "Имя учетной записи:\s+(\S+)\s+Домен учетной записи:\s+(\S+)[\s\S]+Идентификатор процесса:\s+(\S+)\s+Имя процесса:\s+(\S+)"
            
            if ($networkInfo) {
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountName" -Value $Matches[1]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountDomain" -Value $Matches[2]
                $eventData | Add-Member -MemberType NoteProperty -Name "PID" -Value $Matches[3]
                $eventData | Add-Member -MemberType NoteProperty -Name "ProcessName" -Value $Matches[4]
            }
        }

        if ($event.EventCode -eq 4625) {
            # Извлекаем информацию о сетевых адресах с помощью регулярных выражений
            $networkInfo = $event.Message -match "Тип входа:\s+(\d+)[\s\S]+Учетная запись, которой не удалось выполнить вход:[\s\S]+Имя учетной записи:\s+(\S+)\s+Домен учетной записи:\s+(\S+)"
            
            if ($networkInfo) {
                $eventData | Add-Member -MemberType NoteProperty -Name "InputType" -Value $Matches[1]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountName" -Value $Matches[2]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountDomain" -Value $Matches[3]
            }

        }

        if ($event.EventCode -eq 4634) {
            # Извлекаем информацию о сетевых адресах с помощью регулярных выражений
            $networkInfo = $event.Message -match "Имя учетной записи:\s+(\S+)\s+Домен учетной записи:\s+(\S+)[\s\S]+Тип входа:\s+(\d+)"
            
            if ($networkInfo) {
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountName" -Value $Matches[1]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountDomain" -Value $Matches[2]
                $eventData | Add-Member -MemberType NoteProperty -Name "InputType" -Value $Matches[3]
            }
        }

        if ($event.EventCode -eq 4662) {
            # Извлекаем информацию о сетевых адресах с помощью регулярных выражений
            $networkInfo = $event.Message -match "Имя учетной записи:\s+(\S+)\s+Домен учетной записи:\s+(\S+)[\s\S]+Сервер объекта:\s+(\S+)\s+Тип объекта:\s+(.*)\s+Имя объекта:\s+(.*)[\s\S]+Маска доступа:\s+(.*)\s+Свойства:\s+.*\s+(.*\s+.*)"
            
            if ($networkInfo) {
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountName" -Value $Matches[1]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccountDomain" -Value $Matches[2]
                $eventData | Add-Member -MemberType NoteProperty -Name "ObjectServer" -Value $Matches[3]
                $eventData | Add-Member -MemberType NoteProperty -Name "ObjectType" -Value $Matches[4]
                $eventData | Add-Member -MemberType NoteProperty -Name "ObjectName" -Value $Matches[5]
                $eventData | Add-Member -MemberType NoteProperty -Name "AccessMask" -Value $Matches[6]
                $eventData | Add-Member -MemberType NoteProperty -Name "Property" -Value $Matches[7]
            }
        }

        if ($event.EventCode -eq 5152 -or $event.EventCode -eq 5156 -or $event.EventCode -eq 5157) {
            # Извлекаем информацию о сетевых адресах с помощью регулярных выражений
            $networkInfo = $event.Message -match "Адрес источника:\s+([0-9a-fA-F:\.]+)\s+Порт источника:\s+(\d+)\s+Адрес назначения:\s+([0-9a-fA-F:\.]+)\s+Порт назначения:\s+(\d+)"
            
            if ($networkInfo) {
                $eventData | Add-Member -MemberType NoteProperty -Name "SourceAddress" -Value $Matches[1]
                $eventData | Add-Member -MemberType NoteProperty -Name "SourcePort" -Value $Matches[2]
                $eventData | Add-Member -MemberType NoteProperty -Name "DestinationAddress" -Value $Matches[3]
                $eventData | Add-Member -MemberType NoteProperty -Name "DestinationPort" -Value $Matches[4]
            }
        }

        $eventArray += $eventData
    }
}

$eventArray | ConvertTo-Json

'''

# Запуск PowerShell скрипта
result = session.run_ps(ps_script)
print("end script PowerShell")

# Путь к UNIX-сокету
unix_socket_path = '/tmp/winrm_output.sock'


try:
    # Преобразуем текст в список JSON-объектов
    json_data = result.std_out.decode('utf-8', errors='ignore').replace("Active code page: 65001", "")
    json_objects = json.loads(json_data)
    print(f"Number of events {len(json_objects)}")

    # Создаем сокет и подключаемся к UNIX-сокету
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.connect(unix_socket_path)

    for event in json_objects:
        # Преобразуем каждый объект в JSON-строку и отправляем
        event_json = json.dumps(event, ensure_ascii=False)
        sock.sendall((event_json + "\n").encode('utf-8'))
    # Закрываем соединение
    sock.close()

except Exception as e:
    print(f"Ошибка при отправке данных: {str(e)}")