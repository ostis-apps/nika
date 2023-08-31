# Weather forecast agent

This is an agent added as an example to show how to add new python agents to the system.  

To test you can use message like `Какая температура в Минске?` or `Какая температура в Бресте в Беларуси?`, you can specify a country for city names that have the same name in different countries.

**Action class:**

`action_show_weather`

**Parameters:**

1. `messageAddr` - an element of `concept_message` and `concept_message_about_weather`;

**Workflow:**

* The agent extracts an entity from the message, then by [geocoding API](https://geocode.maps.co) gets longitude and latitude of the entity and send request about the entity coordinates using an open [weather API](https://open-meteo.com). 
* After receiving a response the agent processes it and generates a phrase including weather in the entity.

The agent rule:
<img src="../images/lr_message_about_weather.png"></img>

The agent template phrase:
<img src="../images/concept_phrase_about_weather.png"></img>

### Example

Example of an input structure:

<img src="../images/ShowWeatherAgentExample.png"></img>

### Result

Possible result codes:

* `SC_RESULT_OK` - agent successfully found elements and generate phrase;
* `SC_RESULT_ERROR`- `messageAddr` or `entity` is not found.
