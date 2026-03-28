# Документация

Добро пожаловать в документацию [NIKA](https://github.com/ostis-apps/nika)!

## Что такое NIKA?

NIKA - это интеллектуальный ассистент, управляемый знаниями, который работает на основе Технологии OSTIS. Это диалоговая система, которая использует преимущества ostis-систем. Для получения дополнительной информации спросите NIKA: "Что такое NIKA?".

Содержание:

- [Быстрый старт](quick_start.md) - *быстро начните работу с NIKA*

<details markdown="1">
<summary><strong>Инструкции по сборке</strong> - <em>руководства по настройке и компиляции NIKA</em></summary>

- [Быстрый старт](build/quick_start.md) - *быстро соберите NIKA с минимальной настройкой*
- [Docker](build/docker_build.md) - *сборка, запуск и развертывание NIKA с использованием контейнеров Docker*
- [Система сборки](build/build_system.md) - *понимание компонентов базовой системы сборки*
- [Флаги CMake](build/cmake_flags.md) - *настройка процесса сборки с использованием доступных опций CMake*

</details>

<details markdown="1">
<summary><strong>Разработка</strong> - <em>ресурсы для участия в проекте NIKA</em></summary>

- [Git-процесс](dev/git-workflow.md) - *следуйте нашему git-процессу для эффективного сотрудничества*
- [Pull Request](dev/pr.md) - *рекомендации по созданию и отправке pull request'ов*
- [Стиль кода](dev/codestyle.md) - *придерживайтесь соглашений о стиле кодирования проекта*

</details>

<details markdown="1">
<summary><strong>Агенты</strong> - <em>описание агентов в системе NIKA</em></summary>

- [Агент интерпретации неатомарных действий](agents/base/nonAtomicActionInterpretationAgent.md) - *интерпретирует неатомарные действия*
- [Агент ответа на сообщения](agents/base/messageReplyAgent.md) - *генерирует соответствующие ответы на сообщения пользователя*
- [Стандартный агент ответа на сообщения](agents/base/standardMessageReplyAgent.md) - *предоставляет ответы на сообщения*
- [Агент классификации темы сообщения](agents/classification/messageTopicClassificationAgent.md) - *определяет тему сообщений пользователя при помощи Wit.ai*
- [Альтернативный агент классификации темы сообщения](agents/classification/alternativeMessageTopicClassificationAgent.md) - *подход к классификации темы сообщений пользователя при помощи логических правил*
- [Агент генерации фраз](agents/base/phraseGenerationAgent.md) - *создает фразы на естественном языке*
- [Агент изменения цвета компонента интерфейса](agents/question_types/changeInterfaceColorAgent.md) - *динамически настраивает цвет пользовательского интерфейса*
- [Агент поиска слова во множестве по первой букве](agents/question_types/findWordInSetByFirstLetter.md) - *ищет слова в определенном множестве на основе их начальной буквы*
- [Агент прогноза погоды](agents/question_types/weatherAgent.md) - *получает и представляет информацию о погоде*
- [Агент добавления события в Google Календарь](agents/question_types/google/addCalendarEvent.ru.md) - *создает событие в календаре*
- [Агент удаления события из Google Календаря](agents/question_types/google/deleteCalendarEvent.ru.md) - *удаляет событие в календаре*
- [Агент обновления события в Google Календаре](agents/question_types/google/updateCalendarEvent.ru.md) - *обновляет событие в календаре*
- [Агент отправки почты](agents/question_types/google/sendMailSMTP.ru.md) - *отправляет письмо по протоколу SMTP*

</details>

<details markdown="1">
<summary><strong>Шаблоны</strong> - <em>документация паттернов представления знаний</em></summary>

- [Шаблоны базы знаний](patterns/kb-patterns.md) - *повторно используемые шаблоны для структурирования базы знаний*

</details>

<details markdown="1">
<summary><strong>Гайды</strong></summary>

- [Гайд обучения](guide/training_guide.md)

</details>

<details markdown="1">
<summary><strong>Подсистемы</strong> - <em>информация о внутренних компонентах NIKA</em></summary>

- [scl-machine](subsystems/scl-machine.md) - *подробности о подсистеме `scl-машина`*

</details>

- [Лицензия](https://github.com/ostis-apps/nika/blob/main/LICENSE)
- [Список изменений](changelog.md)