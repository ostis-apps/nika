# Documentation

Welcome to [NIKA](https://github.com/ostis-apps/nika) documentation!

## What is NIKA?

NIKA is an Intelligent Knowledge-driven Assistant that operates on the basis of the OSTIS Technology. It is a dialog system that can access the advantages of ostis-systems. For more information, ask NIKA: "What's NIKA?".

Table of contents:

- [Quick Start](quick_start.md) - *get up and running with NIKA quickly*

<details markdown="1">
<summary><strong>Build Instructions</strong> - <em>guides for setting up and compiling NIKA</em></summary>

- [Quick Start](build/quick_start.md) - *get NIKA running quickly with minimal setup*
- [Docker](build/docker_build.md) - *build, run, and deploy NIKA using Docker containers*
- [Build System](build/build_system.md) - *understand the underlying build system components*
- [CMake Flags](build/cmake_flags.md) - *configure the build process using available CMake options*

</details>

<details markdown="1">
<summary><strong>Development</strong> - <em>resources for contributing to the NIKA project</em></summary>

- [Git workflow](dev/git-workflow.md) - *follow our git workflow for effective collaboration*
- [Pull Request](dev/pr.md) - *guidelines for creating and submitting pull requests*
- [Codestyle](dev/codestyle.md) - *adhere to the project's coding style conventions*

</details>

<details markdown="1">
<summary><strong>Agents</strong> - <em>description of agents within the NIKA system</em></summary>

- [Non-atomic action interpretation agent](agents/base/nonAtomicActionInterpretationAgent.md) - *interprets non-atomic actions*
- [Message reply agent](agents/base/messageReplyAgent.md) - *generates appropriate responses to user messages*
- [Standard message reply agent](agents/base/standardMessageReplyAgent.md) - *provides message replies*
- [Message topic classification agent](agents/classification/messageTopicClassificationAgent.md) - *identifies the topic of user messages using Wit.ai*
- [Alternative message topic classification agent](agents/classification/alternativeMessageTopicClassificationAgent.md) - *approach to message topic classification using logic rules*
- [Phrase generation agent](agents/base/phraseGenerationAgent.md) - *creates natural language phrases*
- [Change interface color agent](agents/question_types/changeInterfaceColorAgent.md) - *dynamically adjusts the user interface color*
- [Find word in set by first letter agent](agents/question_types/findWordInSetByFirstLetter.md) - *searches for words within a defined set based on their initial letter*
- [Weather agent](agents/question_types/weatherAgent.md) - *retrieves and presents weather information*
- [Google Calendar Event Addition Agent](agents/question_types/google/addCalendarEvent.md) - *creates an event in the calendar*
- [Google Calendar Event Deleting Agent](agents/question_types/google/deleteCalendarEvent.md) - *deletes an event in the calendar*
- [Google Calendar Event Updating Agent](agents/question_types/google/updateCalendarEvent.md) - *updates an event in the calendar*
- [Send Mail Agent](agents/question_types/google/sendMailSMTP.md) - *sends a mail by SMTP protocol*

</details>

<details markdown="1">
<summary><strong>Patterns</strong> - <em>documentation of knowledge representation patterns</em></summary>

- [Knowledge base patterns](patterns/kb-patterns.md) - *reusable patterns for structuring the knowledge base*

</details>

<details markdown="1">
<summary><strong>Guides</strong></summary>

- [NIKA training guide](guide/training_guide.md)

</details>

<details markdown="1">
<summary><strong>Subsystems</strong> - <em>information about the internal components of NIKA</em></summary>

- [scl-machine](subsystems/scl-machine.md) - *details on the `scl-machine` subsystem*

</details>

- [License](https://github.com/ostis-apps/nika/blob/main/LICENSE)
- [Changelog](changelog.md)