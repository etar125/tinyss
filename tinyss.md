## Tiny SS 24/5
**TinySS** - скриптовый язык программирования, предназначенный для быстрого написания расширений.

### Переменные
Переменная может иметь любое значение (но желательно числовое).  
Чтобы объявить переменную - `define ИМЯ ($)значение`.  
`$` означает, что дальше идёт имя переменной.  
Также можно удалить переменную - `del ИМЯ`.  

Чтобы соединить две переменные - `stradd ИМЯ1 ИМЯ2`.  
Чтобы получить часть - `substr ИМЯ1 $начало $длина`. (результат записывается в `ИМЯ1`)  
Чтобы встроить одну переменную в другую - `strins ИМЯ1 ИМЯ2 $где`.  

Арифметические операции - `op имя + / - / * / /`.  

Проверка - `if/elif имя1 (n)e/l/g/ge/le $имя2`.  
`n` -> `not`.  
Требует `end`.  
Если никакие условия не были выполнены - `else`.
```
if ONE e TWO
    ; false
    end
elif ONE nl TWO
    ; false
    end
else
    ; данный код будет вызван
    end
```

### Метки
Чтобы вызвать метку - `call ИМЯ`, а чтобы вернуться - `ret`.  
Чтобы перейти к метке - `goto ИМЯ`.  

Также есть глобальные функции: `gcall НАЗВАНИЕ`.  
А чтобы передать им аргумент - `gpushb $имя`. (`pushb` -> `push back`)  

При вызове другой функции