# Агент классификации темы сообщения

Данный агент выполняет классификацию сообщений по теме (приветствие, про условия лабораторной работы, про сущность и т.д.).
Кроме того, агент классифицирует сообщение по признакам (нейтральная, позитивная, отрицательная эмоциональная окраска) и получает сущности сообщения.
Агент классифицирует сообщение с помощью Wit.ai.

**Класс действий:**

`action_message_topic_classification`

**Параметры:**

1. `messageAddr` -- элемент класса `concept_message`.

**Используемые библиотеки:**

* [Wit.ai](https://wit.ai/) - для классификации сообщений и получения сущностей.

**Комментарий:**

* Входное сообщение должно содержать текстовый файл с текстом на русском языке;
* Выделяемая сущность должна быть формализованна в базе знаний.

### Пример

Пример входной структуры:

<img src="../images/messageTopicClassificationAgentInput.png"></img>

Пример выходной структуры:
<img src="../images/messageTopicClassificationAgentOutput.png"></img>

Пример структуры, необходимой для классификации сообщения по теме:

<img src="../images/messageTopicClassificationAgentIntentFormalization.png"></img>

Пример структуры, необходимой для классификации сообщения по признакам:

<img src="../images/messageTopicClassificationAgentTraitFormalization.png"></img>

Пример структуры, необходимой для получения сущностей сообщения:

<img src="../images/messageTopicClassificationAgentEntityFormalization.png"></img>

**Текущие классифицируемые сообщения:**

1. `concept_greeting_message`
2. `concept_message_about_entity`
3. `concept_message_about_subdividing`
4. `concept_message_about_study_system`
5. `concept_message_about_lab_work_condition`
6. `concept_message_about_lab_work_deadline`

### Язык реализации агента
C++

### Результат

Возможные результаты:

* `SC_RESULT_OK` - сообщение успешно классифицировано (или произошла пустая классификация) или действие не принадлежит action_message_topic_classification.
* `SC_RESULT_ERROR`- внутренняя ошибка.
