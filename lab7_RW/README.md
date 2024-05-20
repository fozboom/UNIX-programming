# Лабораторная работа No 7. Блокировки чтения/записи

Программа обеспечивает конкурентный доступ к совместно используемому файлу, используя блокировку чтения-записи. Изучаемые системные вызовы: `fcntl(F_GETLK, F_SETLK, F_SETLKW, F_UNLK)`.

Программа в режиме конкурентного доступа читает из и пишет в файл, содержащий записи фиксированного формата. Формат записей произвольный. Примерный формат записи:

```c
struct record_s {
    char name[80];     // Ф.И.О. студента
    char address[80];  // адрес проживания
    uint8_t semester;  // семестр
}
```

Файл должен содержать не менее 10 записей. Создается и наполняется с помощью любых средств.

Программа должна выполнять следующие операции:

`LST` ‒ отображение содержимого файла с последовательной нумерацией записей
`GET <num>` ‒ получение записи с порядковым номером Rec_No;
`MOD <num>` - модификация полей записи
`PUT` ‒ сохранение последней прочитанной и модифицированной записи по месту.

## Алгоритм конкурентного доступа к записи

```
REC <-- get(Rec_No)             // читаем запись

Again:
REC_SAV <-- REC                 // сохраним копию

... // делаем что-нибудь с записью и желаем ее сохранить

if (REC модифицирована) {
    lock(Rec_No)                // блокируем запись для модификации в файле
    REC_NEW <-- get(Rec_No)     // и перечитываем
    if (REC_NEW != REC_SAV) {   // кто-то изменил запись после получения ее нами
        unlock(Rec_No)          // освобождаем запись
        message                 // сообщаем, что запись кто-то изменил
        REC <-- REC_NEW         // повторим все с ее новым содержимым если нужно
        goto Again
    }
    put(REC, Rec_No)            // сохраняем новое содержимое
    unlock(Rec_No)              // освобождаем запись
}

```

Для отладки и тестирования используется не менее двух экземпляров программы.