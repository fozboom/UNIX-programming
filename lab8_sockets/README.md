# Лабораторная работа №8: Сокеты. Взаимодействие процессов

Задача состоит в разработке многопоточного сервера и клиента, работающих по простому протоколу. Изучаемые системные вызовы включают `socket()`, `bind()`, `listen()`, `connect()`, `accept()` и другие, связанные с адресацией в домене AF_INET.

Протокол должен содержать следующие запросы:

- `ECHO` - эхо-запрос, возвращающий без изменений полученное от клиента;
- `QUIT` - запрос на завершение сеанса;
- `INFO` - запрос на получение общей информации о сервере;
- `CD` - изменить текущий каталог на сервере;
- `LIST` - вернуть список файловых объектов из текущего каталога.

Протокол может содержать дополнительные запросы по выбору студента, не выходящие за пределы корневого каталога сервера и не изменяющих файловую систему в его дереве.

Запросы клиенту отправляются на stdin. Ответы сервера и ошибки протокола выводятся на stdout. Ошибки системы выводятся на stderr. Подсказка клиента для ввода запросов - символ '>'. Сервер принимает номер порта, на котором будет слушать и выводит протокол работы в stdout.

Клиент помимо интерактивных запросов принимает запросы из файла. Файл с запросами указывается с использованием префикса '@'.

Примеры использования:

```shell
$ myserver port_no
Готов.
```


```shell
$ myclient server.domen
Вас приветсвует учебный сервер 'myserver'
> @file
> ECHO какой-то_текст
какой-то_текст
> LIST
dir1
dir2
file
> CD dir1
dir1> QUIT
BYE
$
```

```shell
> ECHO "произвольный текст"
произвольный текст
>
```

```shell
> QUIT
BYE
$
```

```shell
> LIST
dir1/
dir2/
file1
file2 --> dir2/file2
file3 -->> dir1/file
>
```


```shell
> CD dir2
dir2> LIST
file2
dir2> CD ../dir1
dir1> LIST
file --> /file1
dir1> CD ..
> CD ..
>
```