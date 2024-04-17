# Агент создания класса сообщений и шаблонов фраз

Данный агент создает класс сообщения, шаблоны фраз и правило логического ответа.

**Класс действий:**

`action_create_message_class_and_phrase_template`

**Параметры:**

1. `messageAddr` -- элемент класса `concept_message`.
2. `formLinkAddr` -- ссылка на информацию из всплывающего компонента.

### Пример

**Пример входной структуры:**

<img src="../images/createMessageClassAndPhraseTemplateAgentInput.png"></img>

**Примеры выходной конструкции:**

Вывод об успешном создании структур.

<img src="../images/createMessageClassAndPhraseTemplateAgentOutput1.png"></img>

Вывод ошибки о недопустимых параметрах.

<img src="../images/createMessageClassAndPhraseTemplateAgentOutput2.png"></img>

Вывод ошибки о существовании параметров.

<img src="../images/createMessageClassAndPhraseTemplateAgentOutput3.png"></img>

Вывод для уведомления о завершении действия пользователем.

<img src="../images/createMessageClassAndPhraseTemplateAgentOutput4.png"></img>

Результат работы агента зависит от данных, полученных от компонента. Если полученные данные верны, структуры будут созданы. В противном случае отобразится соответствующее сообщение об ошибке.

### Язык реализации агента
C++

### Результат

Возможные результаты:

* `SC_RESULT_OK` - конструкции (класс сообщения, шаблоны фраз и правило логического ответа) были успешно созданы или formLinkAddr содержит неверные данные.
* `SC_RESULT_ERROR`- внутренняя ошибка.
