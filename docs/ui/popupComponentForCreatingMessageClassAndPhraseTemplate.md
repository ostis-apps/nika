# User interface component (pop-up window) for creating a message class and phrase templates

This component is used to create a class of messages and phrase templates.

**Phrase for calling the component**

`Создай класс сообщений и шаблоны фраз`

**Action Class:**

`action_message_class_and_phrase_template`

### Description

<img src="../images/messageClassAndPhraseTemplatePopup1.png"></img>

This state of the component is a form, by filling in which data about the system class identifier, about the class identifier in Russian and about the name of the intent in the wit.ai for further processing by the appropriate agent. To move to the next state of the component and save the form data, click the "Next" button.

<img src="../images/messageClassAndPhraseTemplatePopup2.png"></img>

This state of the component is a form, by filling in which data is stored about the system identifier of the phrase template, about the identifier of the phrase template in Russian and about phrases for further processing by the appropriate agent. In this form, the chip component (compact elements representing input data) is used to add an arbitrary number of phrases. To save the form data and create structures, click the "Save" button.

### Component implementation language

TypeScript

### Result

Possible results:

* `Теперь я умею отвечать на новый вопрос!` - constructs (message class, phrase templates, and logical response rule) have been successfully created.
* `Ошибка. Введённые вами системные индетификаторы либо wit.ai intent уже существуют.`- the data entered by the user already exists in the knowledge base.
* `Ошибка. Вы не ввели системные индетификаторы, wit.ai intent или фразы овтетов.` - the user has not entered all the data.
* `Дейстиве прервано пользователем.` - the user has closed the component.