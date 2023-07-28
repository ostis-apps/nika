#  CodeStyle

## C++ Styles

В качестве основы используется [Google's coding standard](https://google.github.io/styleguide/cppguide.html). Советуем Вам с ним ознакомиться.

Ниже можно увидеть некоторые наши особенности по стилистике кода, которых нет в Google's coding standard:

- Весь код должен соответствовать стандарту C++17.
- Мы используем файлы `.cpp` и `.hpp`, а не `.cc` и `.h` (`.c` и `.h` используются для языка C) в кодировке UTF-8.
- Имена файлов пишутся в нижнем регистре с подчеркиванием, например `sc_memory.cpp `.
- Мы используем `#pragma once` вместо `#define` в заголовочных файлах.
- Includes сортируются и группируются по каталогам, между разными каталогами должны быть пустые строки.
- Порядок каталогов в includes: `current_dir/current_file.hpp`, другие файлы из того же каталога, файлы из других каталогов, сортируются по имени.

```cpp
#include "../test.hpp"

#include "hash/hmac.h"
#include "hash/sha256.h"

#include "sc-memory/cpp/sc_wait.hpp"
#include "sc-memory/cpp/kpm/sc_agent.hpp"

#include <curl/curl.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
```

- Мы используем исключения C++.
- Мы используем все возможности C++17.

Именование и форматирование

- Мы ВСЕГДА используем отступ в два пробела и не используем табуляцию. *Если вы видите старый отформатированный файл, то измените его формат в отдельном коммите.*
- У нас нет жестко заданной ширины строки кода, но желательно чтобы она соответствовала размеру экрана.
- Можно использовать комментарии в стиле Doxygen.
- Подчеркивания разрешены только в префиксах для переменных-членов и названиях пространств имен, например: `int m_keynodeAddr; namespace sc_utils`.
- Не используйте `using namespace std` или другие пространства имен глобально. Вы можете использовать их локально в тестовых cpp-файлах или функциях.
- Используйте порядок справа налево для переменных/параметров: `ScAddr const & addr` (ссылка на константу `ScAddr`).
- Для однострочных условий `if`, `for`, `while` мы не используем скобки. Если одна строка `for` или  `while` объединена с одной строкой `if`, используйте скобки для обозначения цикла.

```cpp
for (ScAddr const & addr : listOfAddr)
  ctx.EraseElement(addr);

...

for (ScAddr const & addr : listOfAddr)
{
  if (addr.IsValid())
    return true;
}
```

- Пробелы до и после `:` в цикле `foreach`
- Пробел после ключевого слова в условиях и циклах. Пробел после `;` в цикле `for`.
- Пробел между бинарными операторами: `x = y * y + z * z`.
- Пробел после оператора присваивания `x = isValid ? 2 : 1;`.
- Мы используем ключевое слово `using` вместо `typedef`.
- Статические константы должны быть названы в camelCase, начиная со строчной буквы "k", например `kCompileTimeConstant`, и по возможности помечены как `constexpr`.
- Значения в перечислениях должны быть названы в CamelCase, например `enum class Color { Red, Green, LightBlue };`.
- Макросы и enums в стиле C должны быть названы в UPPER_CASE, а к значениям перечислений должен быть добавлен префикс имени enum с заглавной буквы.
    Обратите внимание, что макросы усложняют отладку, а enums старого стиля содержат опасные неявные преобразования в целые числа и, как правило, загромождают содержащие их пространства имен. Избегайте их, когда это возможно - используйте `const` или `constexpr` вместо макросов и перечисления вместо enums.

**Код должен компилироваться без предупреждений! Иногда предупреждение может присутствовать в течение короткого времени. Но если Вы их видите, то попробуйте исправить в отдельном коммите.**

## ClangFormat

Большая часть нашей стилистики кода указана в файле конфигурации для [ClangFormat](http://clang.llvm.org/docs/ClangFormat.html).
Чтобы автоматически отформатировать файл, установите `clang-format` и запустите:

    clang-format -i file.cpp file.hpp other_file.cpp

Чтобы отформатировать все C/C++ файлы проекта автоматически, используйте следующий скрипт: 

    ./scripts/clang/format_code.sh

## Примеры форматирования

```cpp
#pragma once

#include "sc_addr.hpp"

uint16_t constexpr kBufferSize = 255;

// C-style enums are ALL_CAPS. But remember that C++11 enum classes are preferred.
enum Type
{
  TYPE_INTEGER,
  TYPE_FLOAT,
  TYPE_STRING
};

using TMyTypeStartsWithCapitalTLetter = double;

class ComplexClass
{
public:
  Complex(double rePart, double imPart) : m_re(rePart), m_im(imPart) {}
  double Modulus() const
  {
    double const rere = m_re * m_re;
    double const imim = m_im * m_im;
    return sqrt(rere + imim);
  }
  double OneLineMethod() const { return m_re; }
private:
  // We use m_ prefix for member variables.
  double m_re;
  double m_im;
};

namespace
{
void CamelCaseFunctionName(int lowerCamelCaseVar)
{
  static int counter = 0;
  counter += lowerCamelCaseVar;
}
}  // namespace

namespace lower_case
{
template <class TTemplateTypeStartsWithCapitalTLetter>
void SomeFoo(int a, int b,
             TTemplateTypeStartsWithCapitalTLetter /* We avoid compilation warnings. */)
{
  for (int i = 0; i < a; ++i)
  {
    // IMPORTANT! We DON'T use one-liners for if statements for easier debugging.
    // The following syntax is invalid: if (i < b) Bar(i);
    if (i < b)
      Bar(i);
    else
    {
      Bar(i);
      Bar(b);
      // Commented out the call.
      // Bar(c);
    }
  }
}
}  // namespace lower_case

// Switch formatting.
int Foo(int a)
{
  switch (a)
  {
  case 1:
    Bar(1);
    break;
  case 2:
  {
    Bar(2);
    break;
  }
  case 3:
  default:
    Bar(3);
    break;
  }
  return 0;
}

// Loops formatting.

if (condition)
  foo();
else
  bar();

if (condition)
{
  if (condition)
    foo();
  else
    bar();
}

for (size_t i = 0; i < size; ++i)
  foo(i);

while (true)
{
  if (condition)
    break;
}

// Space after the keyword.
if (condition)
{
}

for (size_t i = 0; i < 5; ++i)
{
}

while (condition)
{
}

switch (i)
{
}

// Space between operators, and don't use space between unary operator and expression.
x = 0;
x = -5;
++x;
x--;
x *= 5;
if (x && !y)
{
}
v = w * x + y / z;
v = w * (x + z);

// Space after double dash. And full sentences in comments.
```

## Советы и подсказки

- Если Вы видите устаревший код, который можно улучшить - СДЕЛАЙТЕ ЭТО СЕЙЧАС (но в отдельном PR).
- Ваш код должен работать, по крайней мере, на платформе Linux.
- Ваш код должен хорошо компилироваться с gcc 4.8+ и clang 3.5+.
- Старайтесь избегать использования какой-либо новой сторонней библиотеки, если она не полностью протестирована и не поддерживается на используемых платформах.
- Покройте свой код модульными тестами.
- Если у Вас недостаточно времени, чтобы все исправить, оставьте комментарий `// TODO(DeveloperName): need to fix it`.

### Некоторые полезные макросы

- Макросы для проверки платформы компиляции

```cpp
#if SC_IS_PLATFORM_WIN32
  // windows platform
#endif

#if SC_IS_PLATFORM_LINUX
 // linux platform
#endif
#if SC_IS_PLATFORM_MAC
 // OSX (mac) platform
#endif

#if SC_IS_PLATFORM_IOS
 // iOS platform
#endif

#if SC_IS_PLATFORM_ANDROID
 // android platform
#endif
```

- Проверка кода во время выполнения при Debug сборке:

```cpp
SC_ASSERT(expr, ("message"));
SC_ASSERT(expr, ()); // no message
```

- Проверка, скомпилирован ли код в режиме отладки:

```cpp
#if SC_DEBUG_MODE
 // compiles in debug
#endif
```

- Проверка кода во время выполнения для любой сборки. Лучше использовать `SC_CHECK_...`, чем `SC_CHECK`:

```cpp
SC_CHECK(expr, ("message"));
SC_CHECK(expr, ()); // no message

SC_CHECK_NOT(expr, ()); // equal to SC_CHECK(!expr, ());
SC_CHECK_EQUAL(a, b, ()); // equal to SC_CHECK(a == b, ());
SC_CHECK_NOT_EQUAL(a, b, ()); // equal to SC_CHECK(a != b, ());
SC_CHECK_GREAT(a, b, ()); // equal to SC_CHECK(a > b, ());
SC_CHECK_GREAT_EQ(a, b, ()); // equal to SC_CHECK(a >= b, ());
SC_CHECK_LESS(a, b, ()); // equal to SC_CHECK(a < b, ());
SC_CHECK_LESS_EQ(a, b, ()); // equal to SC_CHECK(a <= b, ());
```

### Логирование
  * `SC_LOG_DEBUG(msg)` - вывод сообщения в Debug сборке. Префикс: `[Debug]`
  * `SC_LOG_INFO(msg)`
  * `SC_LOG_WARNING(msg)`
  * `SC_LOG_ERROR(msg)`
  * `SC_LOG_INFO_COLOR(msg, color)` - Вывод цветного информационного сообщения. Посмотреть цветовые константы можно в `ScConsole::Color`

### Объявление своих собственных исключений

```cpp
class MyException final : public ScException
{
public:
	explicit MyException(std::string const & msg) : ScException("My: " + msg) {}
};
```

### Генерация исключений 

Генерация исключений с помощью `SC_THROW_EXCEPTION(exceptionName, message)`
```cpp
SC_THROW_EXCEPTION(MyException, "my message")
```

### Исключение для нереализованных частей кода
```cpp
SC_NOT_IMPLEMENTED("message")
```

## Python Styles
  
Код Python должен быть проверен на:

- Соответствие правилам PEP8.
- Отсутствие закомментированного кода.
- Подходящие названия классов, функций и переменных, которые должны отражать назначение.
- Использование snake_case.
- Отсутствие новых файлов для функций того же класса.
- Соответствие кода стилю Python.
- Декомпозицию задач. Один PR не должен содержать много файлов.
- Копирование существующего кода.
- Соблюдение DRY принципа.

### Самопроверка

`black` форматировщик используется для форматирования кода на Python.

Чтобы показать рекомендуемые изменения:
```sh
black --diff --color --line-length 120 path/to/your/files
```

Чтобы переформатировать код:

```sh
black --line-length 120 path/to/your/files
```
Для проверки кода используются утилиты `pylint`.

Используйте скрипт `local_ci.sh`  для самопроверки. Этот скрипт позволяет Вам выполнять большинство проверок, которые присутствуют в CI локально.
Используйте аргумент `-a` для выполнения всех проверок. Также Вы можете запускать отдельные проверки с помощью `-b`, `-p` (для запуска проверок black и pylint соответственно) или их комбинации.
