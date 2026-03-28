# Альтернативный агент классификации сообщений

Данный агент выполняет классификацию сообщений, используя [логические правила](../subsystems/scl-machine.md).

**Класс действий:**

`action_alternative_message_topic_classification`

**Параметры:**

1. `messageAddr` -- элемент класса `concept_message`.

### Пример

Пример входной структуры:

<img src="../images/alternativeMessageTopicClassificationAgentInput.png"></img>

Пример логической формулы для классификации сообщения:

<img src="../images/lr_greeting_message.png"></img>

Результат работы агента зависит от результатов логического вывода. Если какая-либо формула была применена успешно, то `messageAddr` имеет класс согласно формуле.
В противном случае, `messageAddr` принадлежит классу `concept_not_classified_by_intent_message`.

### Язык реализации агента
C++

### Результат

Возможные результаты:

* `SC_RESULT_OK` - сообщение успешно классифицировано (или произошла пустая классификация) или действие не принадлежит action_message_topic_classification.
* `SC_RESULT_ERROR`- внутренняя ошибка.
