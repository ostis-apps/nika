# Project Structure

## kb

Place for the knowledge base of your app. Put your *.scs* files here.

## problem-solver

Place for the problem solver of your app. Put your agents here.

### C++ agents

Some tips:

- Store your modules with c++ agents in *problem-solver/cxx*;

- After updating c++ code you will need to rebuild problem-solver. Just run:

```sh
./scripts/build_problem_solver.sh
```

For a full rebuild with the deleting of the *bin* and *build* folders run:

```sh
./scripts/build_problem_solver.sh -f
```

For building the project in release mode run:

```sh
./scripts/build_problem_solver.sh -r
```

To build tests run:

```sh
./scripts/build_problem_solver.sh -t
```

For a full rebuild with build tests and the deleting of the *bin* and *build* folders run:

```sh
./scripts/build_problem_solver.sh -f -t
```

- Add an action deactivation check by using a function `ActionUtils::isActionDeactivated()` from the common module. Identifiers of deactivated actions are stored in `kb/non_subject_domain_concepts/action_deactivated.scs`. Example:

```cpp
#include "utils/ActionUtils.hpp"

sc_result MyModule::InitializeImpl()
{
  ScMemoryContext ctx(sc_access_lvl_make_min, "MyModule");
  if (ActionUtils::isActionDeactivated(&m_memoryCtx, Keynodes::action_of_my_agent))
  {
    SC_LOG_ERROR("My agent action is deactivated")
  }
  else
  {
    ...
  }
  return SC_RESULT_OK;
}
```

### Logging

You can change log mode and level in nika.ini configuration file.

## sc-web

- After updating js code you will need to rebuild sc-web. Just run:

```sh
./scripts/build_sc_web.sh
```

## interface

Place for your interface modules.

## scripts

Place for scripts of your app.

### build_problem_solver.sh [-f, --full, -t, --tests, -r, --release, --help]

Build the problem-solver of your app. Use an argument *-f* or *--full* for a complete rebuild of the problem-solver with the deleting of the *bin* and *build* folders.

### run_interface.sh

Run the interface of your app.

### install_project.sh

Install or update the ostis-web-platform.

### install_subsystems.sh

Building a problem solver and a knowledge base of subsystems.
