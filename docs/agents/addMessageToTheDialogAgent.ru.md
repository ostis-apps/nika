Данный агент добавляет сообщение в диалога.

**Класс действия:**

`action_add_message_to_the_dialog`

**Параметры:**

1. `message node` - элемент класса `concept_message` и `concept_atomic_message` или `concept_non_atomic_message`;
2. `dialog history` - элемент класса `concept_dialogue`, в котором будет сохранена история диалога.

**Ход работы агента:**

* Узел сообщения, полученный агентом, добавляется к последовательности сообщений в истории диалога.
* Если история диалогов пуста, то к первому добавленному сообщению будет добавлено отношение `rrel_1`. Последнее сообщение всегда имеет отношение `rel_last`. Между сообщениями существует отношение `nrel_message_sequence`, которое определяет последовательность сообщений в диалоге.

### Пример

Пример структуры ввода для добавления пользовательского сообщения в диалог:

<img src="../images/addMessageToTheDialogAgentInputUser.png"></img>
<img src="../images/addMessageToTheDialogAgentInputDialogUser.png"></img>

Пример структуры ввода для добавления сообщения системы в диалог:

<img src="../images/addMessageToTheDialogAgentInputNika.png"></img>
<img src="../images/addMessageToTheDialogAgentInputDialogNika.png"></img>

Пример полученной структуры:

<img src="../images/addMessageToTheDialogAgentOutputDialog.png"></img>

### Результат

Возможные результаты:
 
* `SC_RESULT_OK` - сообщение было добавлено в диалог.
* `SC_RESULT_ERROR_INVALID_PARAMS` - действие не содержит входящего сообщения.
* `SC_RESULT_ERROR`- внутренняя ошибка.
