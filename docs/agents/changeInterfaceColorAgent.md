# Interface component color changer agent

Is an agent that changes a color of an interface component.

To test you can use messages like `Измени цвет хедера на #001122`, `Поменяй цвет компонента футер на #CC1122` or `Измени цвет компонента основная часть на #00AAFF`.

**Action class:**

`action_change_interface`


**Parameters:**

1. `messageAddr` has `interface component` as `rrel_entity` and optionally `component color` as `rrel_color`.

**Workflow:**

* The agent finds component color by `nrel_component_color` relation and changes its color sc-link value.
* If the concrete color is not passed, random color is generated.
* The agent is called by logic rule with concrete color or with random color.

The concrete interface component color rule:
<img src="../images/lr_color_message.png"></img>

The random interface component color rule:
<img src="../images/lr_random_color_message.png"></img>

<img src="../images/interfaceComponentColorExample.png"></img>

### Example

Example of an input structure:

<img src="../images/changeInterfaceColorAgentInput.png"></img>

### Result

Possible result codes:

* `SC_RESULT_OK` - interface component color is changed successfully;
* `SC_RESULT_ERROR`- `message` or `interface component` is not found.
