User interface component (pop-up window) for creating a class instance

This component is used to create an instance of the entity class.

**Phrase for calling the component**

`Создай экземпляр класса`

**Action Class:**

`action_create_class_instance`

### Description

<img src="../images/messageClassInstancePopup1.png"></img>

This state of the component is a form, by filling in which data about the system class identifier, about the class identifier in Russian and about the name of the intent in the wit.ai for further processing by the appropriate agent. To move to the next state of the component and save the form data, click the "Next" button.

<img src="../images/messageClassInstancePopup2.png"></img>

This state of the component is a form, by filling in which data is stored about the system identifier of the phrase template, about the identifier of the phrase template in Russian and about phrases for further processing by the appropriate agent. In this form, the chip component (compact elements representing input data) is used to add an arbitrary number of phrases. To save the form data and create structures, click the "Save" button.

### Component implementation language

TypeScript

### Result

Possible results:

* `Экземпляр класса создан` - constructs have been successfully created.
* `Вы ввели существующий в базе знаний системный идентификатор, либо не ввели основной идентификатор или определение.`- the data entered by the user already exists in the knowledge base or user has not entered all the data.
* `Дейстиве прервано пользователем.` - the user has closed the component.