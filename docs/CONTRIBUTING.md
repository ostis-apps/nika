This page describes rules to create Pull Requests.  

### Commits message format

Commit naming rules are presented [here](https://github.com/angular/angular/blob/master/CONTRIBUTING.md#commit-message-format)

We use next commit scopes:

 - kb - knowledge base updates
 - solver - Problem Solver updates(C++ and Python agents)
 - client - web-client updates

_**Update this docs via pull request for add new scope.**_

_**You can do not use scope. For example `feat: ...`, `fix: ...`**_

Each commit should have not much differences excluding cases, with:

  - CodeStyle changes;
  - Renames;
  - Code formatting;

**Do atomic commits for each changes.** For example if you rename some members in `ClassX` and `ClassY`, then do two commits:
```
fix(<scope>): rename members in ClassX according to codestyle
fix(<scope>): rename members in ClassY according to codestyle
```

**Do not mix codestyle changes and any logical fixes in one commit**

_**All commit, that not applies to this rules, should be split by this rules. Another way they will be rejected with Pull request.**_

### Pull Request Preparation

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
 - Author merge PR to base branch
 - Author test work at base branch
