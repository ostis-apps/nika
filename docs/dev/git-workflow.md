# Git worflow

**We use repo forks**

## Initialize

Use ssh for cloning repo:

```sh
git clone git@github.com:ostis-apps/nika.git
```

## Workflow

This project uses [Git-Flow](https://www.gitkraken.com/learn/git/git-flow),
[git-flow tool](https://github.com/nvie/gitflow) can be used.

The Git flow branches that we are interested in are the following branches :

* `develop` (long lived) - latest development work, deploys to a dev environment
* `feature/*`(short lived) - new functional, docs, build, CI, test development work
* `fix/*`(short lived) - fixes of functional, docs, build, CI, test development work
* `release/*` (short lived) - release candidate, bug fixes for a release, deploys to a test environment
* `main` (long lived) - last release, deploys to a production environment
* `hotfix/*` (short lived) - urgent fixes to production
