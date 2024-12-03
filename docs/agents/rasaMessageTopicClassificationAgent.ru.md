# Агент классификации темы сообщения

Данный агент выполняет классификацию сообщений по теме (приветствие, про условия лабораторной работы, про сущность и т.д.).
Агент классифицирует сообщение с помощью Rasa.

**Класс действий:**

`action_rasa_message_topic_classification`

**Параметры:**

1. `messageAddr` -- элемент класса `concept_message`.

**Используемые библиотеки:**

* [Rasa](https://rasa.com/) - официальный сайт Rasa с документацией.

**Особенности Rasa**

Использование классификатора Rasa абсолютно независимо от Интернет соединения, как и его обучение и тестирование. Rasa не располагает заранее разработанным интерфейсом для обучения классификатора, обучать его можно через .yml файлы. Модуль Rasa содержит в себе немало других модулей, от которых она зависима, поэтому для локального хранения классификатора необходимо заранее выделить место.  

Формат json ответа от классификатора:

```json
{"text":"Что такое Ника?","intent":{"name":"concept_message_about_entity","confidence":1.0},"entities":[{"entity":"entity","start":10,"end":14,"confidence_entity":0.9997850060462952,"value":"Ника","extractor":"DIETClassifier"}],"text_tokens":[[0,3],[4,9],[10,14]],"intent_ranking":[{"name":"ask_definition","confidence":1.0},{"name":"goodbye","confidence":2.69290423293711e-10},{"name":"greet","confidence":1.639179937618085e-11}]}
```

**Комментарий:**

* Входное сообщение должно содержать текстовый файл с текстом на русском языке;
* Выделяемая сущность должна быть формализованна в базе знаний.

### Пример

Пример входной структуры:

<img src="../images/rasaMessageTopicClassificationAgentInput.png"></img>

Пример выходной структуры:
<img src="../images/rasaMessageTopicClassificationAgentOutput.png"></img>

Пример структуры, необходимой для классификации сообщения по теме:

<img src="../images/rasaMessageTopicClassificationAgentIntentFormalization.png"></img>

Пример структуры, необходимой для получения сущностей сообщения:

<img src="../images/rasaMessageTopicClassificationAgentEntityFormalization.png"></img>

**Текущие классифицируемые сообщения:**

1. `concept_message_about_entity`

### Язык реализации агента
C++

### Результат

Возможные результаты:

* `SC_RESULT_OK` - сообщение успешно классифицировано (или произошла пустая классификация) или действие не принадлежит action_rasa_message_topic_classification.
* `SC_RESULT_ERROR`- внутренняя ошибка.
