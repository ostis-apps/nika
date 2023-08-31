# Pull Requests

This page describes rules to create Pull Requests.

## Commits message format

Commit naming rules are presented [here](https://github.com/angular/angular/blob/master/CONTRIBUTING.md#commit-message-format)

We use next commit scopes:

- kb - knowledge base updates
- solver - Problem Solver updates (C++ and Python agents)
- client - web-client updates

_**Update these docs via pull request for add new scope.**_

_**Omitting the scope is allowed. For example `feat: ...`, `fix: ...`**_

Each commit should have not many differences excluding cases, with:

- Codestyle changes;
- Renames;
- Code formatting;

**Do atomic commits for each change.** For example if you rename some members in `ClassX` and `ClassY`, then do two commits:

```txt
fix(<scope>): rename members in ClassX according to codestyle
fix(<scope>): rename members in ClassY according to codestyle
```

**Do not mix codestyle changes and any logic fixes in one commit**

_**All commits that do not adhere to these rules should be split. Otherwise, the PR will be rejected.**_

### Pull Request Preparation

 If it's needed:

- Read rules to create PR in documentation
- Update changelog
- Update documentation

### Pull Request creation

- Create PR on GitHub
- Check that CI checks were passed successfully
- Assign PR reviewers
- Send link to PR to project development chat

### Pull Request Review

- Reviewer should test code from PR if CI don't do it
- Reviewer submit review as set of conversations
- Author make review fixes at `Review fixes` commits
- Author re-request review
- Reviewer resolve conversations if they were fixed and approve PR

### After Pull Request Review

- Author can update commits history for remove `Review fixes` commits
- Author test work at base branch
- Author merge PR to base branch
