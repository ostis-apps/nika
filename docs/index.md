# Documentation

Welcome to [NIKA](https://github.com/ostis-apps/nika) documentation! 

## What is NIKA?  

NIKA is an Intelligent Knowledge-driven Assistant that operates on the basis of the OSTIS Technology. It is a dialog system that can access the advantages of ostis-systems. For more information, ask NIKA: "What's NIKA?".

Table of contents:

- [Quick Start](quick_start.md) - *get up and running with NIKA quickly* 
- **Build Instructions** - *guides for setting up and compiling NIKA*
    - [Quick Start](build/quick_start.md) - *get NIKA running quickly with minimal setup*
    - [Docker](build/docker_build.md) - *build, run, and deploy NIKA using Docker containers*
    - [Build System](build/build_system.md) - *understand the underlying build system components*
    - [CMake Flags](build/cmake_flags.md) - *configure the build process using available CMake options*
- **Development** - *resources for contributing to the NIKA project*
    - [Git workflow](dev/git-workflow.md) - *follow our git workflow for effective collaboration*
    - [Pull Request](dev/pr.md) - *guidelines for creating and submitting pull requests*
    - [Codestyle](dev/codestyle.md) - *adhere to the project's coding style conventions*
- **Agents** - *description of agents within the NIKA system*
    - [Non-atomic action interpretation agent](agents/nonAtomicActionInterpretationAgent.md) - *interprets non-atomic actions*
    - [Message reply agent](agents/messageReplyAgent.md) - *generates appropriate responses to user messages*
    - [Standard message reply agent](agents/standardMessageReplyAgent.md) - *provides message replies*
    - [Message topic classification agent](agents/messageTopicClassificationAgent.md) - *identifies the topic of user messages using Wit.ai*
    - [Alternative message topic classification agent](agents/alternativeMessageTopicClassificationAgent.md) - *approach to message topic classification using logic rules*
    - [Phrase generation agent](agents/phraseGenerationAgent.md) - *creates natural language phrases*
    - [Change interface color agent](agents/changeInterfaceColorAgent.md) - *dynamically adjusts the user interface color*
    - [Find word in set by first letter agent](agents/findWordInSetByFirstLetter.md) - *searches for words within a defined set based on their initial letter*
    - [Weather agent](agents/weatherAgent.md) - *retrieves and presents weather information*
- **Patterns** - *documentation of knowledge representation patterns*
    - [Knowledge base patterns](patterns/kb-patterns.md) - *reusable patterns for structuring the knowledge base*
- **Subsystems** - *information about the internal components of NIKA*
    - [scl-machine](subsystems/scl-machine.md) - *details on the `scl-machine` subsystem*
- [License](https://github.com/ostis-apps/nika/blob/main/LICENSE)
- [Changelog](changelog.md)
