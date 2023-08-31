# Style Guide

## C++ Styles

In general, [Google's coding standard](https://google.github.io/styleguide/cppguide.html) is used, and we strongly encourage to read it.

Below are our specific (but not all!) exceptions to the Google's coding standard:

- All code should conform to C++17 standard.
- We use `.cpp` and `.hpp` files, not `.cc` and `.h` (`.c` and `.h` are used for C code), in UTF-8 encoding.
- File names are lowercase with underscores, like `sc_memory.cpp`.
- We use `#pragma once` instead of the `#define` Guard in header files.
- Includes are sorted and grouped by directory, there should be newlines between different directories.
- Order of directories in includes: "current_dir/current_file.hpp", other includes from the same dir, includes from other dirs sorted by name.

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

- We ARE using C++ exceptions.
- We are using all features of C++17.

Naming and formatting

- We ALWAYS use two spaces indent and don't use tabs. *If you see old formated file, then change format of it in separated commit.*
- We don't have hardcoded line width, but keep it reasonable to fit on the screen.
- Doxygen-style comments can be used.
- Underscores are allowed only in prefixes for member variables and namespace names, like `int m_keynodeAddr; namespace sc_utils`.
- Don't use `using namespace std` or other namepsaces globaly. You can use them localy in test cpp files or functions.
- Use right-to-left order for variables/params: `ScAddr const & addr` (reference to the const `ScAddr`).
- In one line `if`, `for`, `while` we do not use brackets. If one line `for` or `while` is combined with one line `if`, do use brackets for cycle.

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

- Spaces before and after `:` in `foreach` loop
- Space after the keyword in conditions and loops. Space after `;` in `for` loop.
- Space between binary operators: `x = y * y + z * z`.
- Space after double dash `x = isValid ? 2 : 1;`.
- We use `using` keyword instead of `typedef`.
- Compile-time constants must be named in camelCase, starting with a lower-case `k`, e.g. `kCompileTimeConstant` and marked as `constexpr` when possible.
- Values of enum classes must be named in CamelCase, e.g. `enum class Color { Red, Green, LightBlue };`.
- Macros and C-style enums must be named in UPPER_CASE, and enum values must be prefixed with a capitalized enum name.

    Note that macros complicate debugging, and old-style enums have dangerous implicit conversions to integers, and tend to clutter
    containing namespaces. Avoid them when possible - use `const` or `constexpr` instead of macros, and enum classes instead of enums.

**Code should compile without warnings! Just deprecated warning could be present for a short time. But if you see them, then try to fix it in a separate commit.**

## ClangFormat

Most of our coding style is specified in a configuration file for [ClangFormat](http://clang.llvm.org/docs/ClangFormat.html).
To automatically format a file, install `clang-format` and run:

```sh
clang-format -i file.cpp file.hpp other_file.cpp
```

To automatically format all C/C++ files, run:

```sh
./scripts/clang/format_code.sh
```

## Formatting Example/Guide/Reference

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

## Tips and Hints

- If you see outdated code which can be improved - DO IT NOW (but in the separate pull request).
- Your code should work at least on linux platform.
- Your code should compile well with gcc 4.8+ and clang 3.5+
- Try to avoid using any new 3party library if it is not fully tested and supported on supported platforms.
- Cover your code with unit tests.
- If you don't have enough time to make it right, leave a `// TODO(DeveloperName): need to fix it` comment.

### Some useful macros

- macroses to check compilation platform

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

- Check code in runtime for Debug builds:

```cpp
SC_ASSERT(expr, ("message"));
SC_ASSERT(expr, ()); // no message
```

- Check if code compiles in Debug mode:

```cpp
#if SC_DEBUG_MODE
 // compiles in debug
#endif
```

- Check code in runtime for any type of build. Prefer `SC_CHECK_...` to `SC_CHECK`:

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

### Logging

- `SC_LOG_DEBUG(msg)` - print message in Debug builds. Prefix: `[Debug]`
- `SC_LOG_INFO(msg)`
- `SC_LOG_WARNING(msg)`
- `SC_LOG_ERROR(msg)`
- `SC_LOG_INFO_COLOR(msg, color)` - print colored info message. Look color constants in `ScConsole::Color`

### Declare your own exceptions

```cpp
class MyException final : public ScException
{
public:
 explicit MyException(std::string const & msg) : ScException("My: " + msg) {}
};
```

### Throw exceptions

Throw exceptions with `SC_THROW_EXCEPTION(exceptionName, message)`:

```cpp
SC_THROW_EXCEPTION(MyException, "my message")
```

### Exception for a non implemented parts of code

```cpp
SC_NOT_IMPLEMENTED("message")
```

## Python Styles

Python code should be checked on:

- PEP8 code compliance.
- No commented out code.
- Names of classes, functions and variables should reflect the purpose.
- Use snake_case.
- Don't create new files for functions of the same class.
- Code compliance the Python style.
- Decompose tasks. One pull request should not contain many files.
- Cannot copy existing code.
- Follow the DRY principle.
  
### Self-check

The `black` formatter is used to format the Python code.

To show recommended changes:

```sh
black --diff --color --line-length 120 path/to/your/files
```

To reformat the code:

```sh
black --line-length 120 path/to/your/files
```

The `pylint` utilities are used to verify the code.

Use `local_ci.sh` script for self-check. This script allows you to run most of the checks that are present in CI locally.
Use `-a` argument to run all the checks. Also you can run separate checks with `-b`, `-p` (to run black and pylint  checks respectively) or a combination of them.
