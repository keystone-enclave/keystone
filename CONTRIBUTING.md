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
(1) All documents are up-to-date and agree with the current code
(2) All tests succeed

1. `master` must always be *healthy*
2. No direct commits allowed to `master`. `master` can be updated only by PR-ing healthy `dev`.
3. `dev` can be unhealthy, but should be healthy before being merged into `master`
Ideally `dev` should not stay unhealthy more than a few days.

### Rules for Submodules (`sdk` and `runtime`)

For `sdk` and `runtime`, `<submodule>:master` means the `master` branch of each submodule.
For `riscv-pk` use `keystone` instead of `master` as the master branch.
This is because those repositories are the mirrors of some upstream repos.
We want to keep `master` clean so that we can pull the upstream changes into `master`.

1. The submodules in `keystone:master` and `keystone:dev` should always point to the commits on `<submodule>:master`.
2. `master` of submodules should be always healthy (it compiles well and passes some basic tests).
3. The submodules can also have `dev-<name of task>` branch for each open task.
These kind of branches should be deleted right after merge

# How to Release

### Version Number

A version number consists of two positive integers: `major` and `minor`. For example, `1.14` means that the major is 1 and the minor is 14.
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

There are a few things that needs to be done before we bring Keystone into real world.

## List of Projects

**Hardware** (Requirements are marked *)

| Name | Type | Assigned | Description |
|:--------|:-----|:---:|:------------|
| *Silicon root of trust | development |  | Currently, Keystone only implements software-based root of trust simulated via early-stage bootloader (e.g., ZSBL). This lacks hardware-based protection of the keys and the certificate. OpenTitan is a potential open-source project that can be integrated with Keystone. |
| *I/O protection | development | | SoCs needs to also enforce the memory isolation for peripheral devices. This can be done by RISC-V IOPMP standard, which is still WIP. Some companies have already came up with non-standard IOPMP on their chip. |
| Interrupt Controller | development |  |Keystone doesn't have ability to allow enclaves to receive their own interrupts. This can be implemented on PLIC or CLIC interrupt controller. |
| Crypto Accelerator | research, development | Gui Andrade| Cryptographic accelerators may speed up secure booting, measurement, and attestation. Also, this could potentially make software-based memory encryption practical (ongoing research by Gui Andrade) |
| PMP Limitation | research |  | The limited number of PMP entries (e.g., 16) limits the number of concurrent enclaves, as well as some of research extensions (e.g., Elasticlave). A number of solutions have been proposed (e.g., sPMP of Penglai Enclave) as well. Faster hardware implementations or a better specification shall be explored. |

**Software**  (Requirements are marked *)

| Name | Type | Assigned | Description |
|:--------|:-----|:---:|:------------|
| System Call (`fork()`) | research, development| Dayeol Lee | `fork()` is one of the most tricky system calls to implement in TEEs. We are currently exploring secure, verified cloning of an enclave via snapshotting and post-init measurement (ongoing research by Dayeol Lee). This will improve start-up latency and resource utilization of numerous workloads (e.g., FaaS). |
| *System Calls (I/O) | development |  | More I/O system calls must be handled in the runtime in order to support more legacy applications. |
| Dynamic Loading | research, development | Cathy Lu | Currently, enclaves can only be statically initialized and should be completely loaded before the beginning of the execution. To support dynamic libraries in Keystone, we need to re-design how applications are loaded and also how the enclave verify the libraries after initialization. |
|More off-the-shelf microkernels| development | | We are hoping to support more off-the-shelf microkernels other than seL4 so that one can easily port their specific application into Keystone enclave. |
| *Concurrent Multithreading | research, development | Stephan Kaminsky | Enclaves can have multiple threads, but they can't run in parallel because we have no mechanism for an enclave to be aware of multiple harts. |
| Embedded Device TEE | research, development | Alex Thomas | Embedded or IoT devices often do not have MMU or S-mode. Our security monitor interface needs to be general enough to support these chips. |
| Toolings | development | | You can run unmodified application on Keystone, but you still need a bunch of tools like signature generator or edge-call development tools. |
| Interop with TEE Open-Source frameworks | development | | Asylo, OP-TEE |
