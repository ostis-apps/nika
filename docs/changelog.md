# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.2.1] - 04.06.2025

### Fixed

- Use DYLD_LIBRARY_PATH to run sc-machine on macOS
- Install common library into lib directory

## [0.2.0] - 03.06.2025

### Added

- Training Guide

### Changed

- Migrate to OSTIS Platform 0.10.0

## [0.1.0] - 01.09.2023

### Added

- Rules to answer if we don't know about asked entity
- Find entity by identifiers relation set, not only main idtf
- Common rule to answer "На что декомпозируется ...?" with any formalized entity
- Common rule to answer "Что такое ...?" with any formalized entity
- Rule to answer "Что умеет Ника?"
- Phrase pattern to get all entities from set $...{_set}
- Pattern to design knowledge base
- Alternative message classification agent using logic rules
- Change interface color agent
- Move to the latest platform
- Replace ostis-inference with scl-machine
- Find word in set by first letter agent
- Weather agent 
- Russian version for documentation

### Changed
- Message processing program tries to classify message with rules first, then with wit.ai
- Simplify platform prepare
- Reuse scripts from platform
- Rename script `run_scweb.sh` to `run_sc_web.sh`

### Removed
- Script `install_ostis.sh`
- Script `set_vars.sh` part for ci

## 12.09.2022

### Added

- Add excluding many entities with the same role in message classification agent
- Add README about project and installation
- Add clang format check
- Dockerize system
- Add questions and answers about laboratory work #1
- Add about page in UI
- Add UI loading at the starting and without the Internet
- Add message processing program
- Add SCg view of semantic equivalent
- Add dialog UI component
- Add phrase generation agent
- Add standard message reply agent
- Add topic classification agent
- Add message reply agent
- Train Wit.ai for message classification and excluding message entities
- Move to platform 0.7.0
- Add SD of intelligent studying systems
- Add SD of dialogues
- Add SD of messages
