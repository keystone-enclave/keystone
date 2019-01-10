# How to Contribute

### Submodules

You are recommended to *fork* the submodules to your own GitHub account, modify the code, 
and send Pull Request (PR) into `master` branch. The admin will review the code before merging.

### `keystone` repo

For the `keystone` main repo, you should PR for all of the changes of submodules first, and request PR into `keystone:dev`.
The admin will review the code and merge all of the submodules and then merge `keystone`.

### General Guideline

Please try to write a full description of your modification when you PR or commit.
Cite related issues if possible.

Please update the documentation if needed.

# Terminology

**Snapshot** - A certain version on any branch, having a unique commit hash. A snapshot is 1-1 mapped to a commit hash.

**Healthy Snapshot** - If a snapshot is not broken and works well with the up-to-date documentation, the snapshot is told to be *healthy*. However, it does not mean that the branch has no bug or security hole. There's no such snapshot.
Healthiness is different from *stability*. See [How To Release](# How to Release) to see what is a *stable* version.

**Healthy Branch** - If the latest snapshot of a branch is healthy, the branch is told to be healthy.

# Branches

### Name of the Branches

`dev`: contains the latest snapshot of the project. All changes first go to this branch.
`master`: contains the latest "healthy" snapshot pulled from `dev`

Optionally, we can occasionally have `dev-<name of task>` branches for a specific feature or a subproject.
For example, if we want to integrate `CMA`, we need to create a branch named something like `dev-cma`,
and work on the branch until everything has been done.
These branches should be deleted after merge.

### Maintaining Healthiness

The snapshot is healty if:
(1) All documents are up to date and agree with the current code
(2) All tests succeed
(3) All submodules points to `master` of themselves

1. `master` must always be *healthy*
2. No direct commits allowed to `master`. `master` can be updated only by merging healthy `dev`.
3. `dev` can be unhealthy, but should be healthy before being merged into `master`
Ideally `dev` should not stay unhealthy more than a few days.

### Rules for Submodules

1. Submodules in `keystone:master` should always point to commits on `<submodule>:master`.
2. Submodules in `keystone:dev` should point to commits on `<submodule>:master` before being merged into `master`.
3. `master` of submodules should be always healthy (it compiles well and passes some basic tests).
4. submodules can also have `dev-<name of task>` branch for each open task. 
These kind of branches should be deleted right after merge

# How to Release

### Version Number

A version number consists of `major` and `minor`. For example, `1.14` means that the major is 1 and the minor is 14.
The numbers increase on each *major release* or *minor release* respectively. 

### Releasing

"Releasing" is just giving a version number to the latest snapshot of the project.

The admins are in charge of the releases.

Major is usually used to indicate a big change in the product, and minor is usually used for minor updates.

We use roadmap-driven major and schedule-driven minor.
For example, major increases only when we have some major changes (which is very rare).
We do minor release at the end of every month (or every two month).

To release:
(1) Write a changelog.
(2) Create a tag with the next ersion number at the latest `master`
(3) Create a document version at `readthedocs.org`

### Stable and Latest Versions

For now, there is only one *latest* version. We don't support stable versions at this moment.

# Call for Contribution

We have several specific TODOs that need contribution.
Any other contributions are very welcomed.

1. Better programming model for the enclaves in SDK (something like Google Asylo, Intel SGX SDK or even better)
2. Integration with hardware memory encryption engine
3. Better runtime supporting more system calls and dynamic library support
4. More use-case applications

For potential collaboration or large suggestions, please feel free to create an issue or contact us (keystone-leads@googlegroups.com).

