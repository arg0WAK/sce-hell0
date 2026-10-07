<p align="center">
  <img src="https://arg0wak.github.io/gist/images/sce-hell0/ODZBPYKJ9HBSSSZI.webp" width="300" alt="sce-hell0 Logo"/>
</p>

# sce-hell0

> One ELF to rule the chain. \
> _Package the app. Embed the payloads. Run the chain._

## Overview

`sce-hell0` packages ELF payloads into a single `orchestrator.elf`
and provides a simple HTTP interface for running them in sequence.

It can be deployed as a PlayStation 5 application or used as a standalone orchestration component. The project handles the application packaging workflow, embeds ELF payloads directly into the generated `orchestrator.elf`, and provides a local HTTP control plane for managing their execution order.

You can place the required application assets, dependencies, and payloads under the `app/` directory and let the build system handle the packaging and embedding process. Payloads can then be registered dynamically, assigned individual delays, and executed sequentially through a simple HTTP API.

The number of embedded payloads is configurable, allowing a single `orchestrator.elf` to carry an entire payload chain rather than requiring a separate ELF delivery mechanism for each stage.

The service is intentionally bound to the loopback interface and designed to operate as a short-lived orchestration layer rather than a persistent daemon.

## Key Features

- PlayStation 5 application packaging and installation workflow
- Build-time embedding of multiple ELF payloads into a single `orchestrator.elf`
- Configurable payload capacity
- Dynamic task queue management
- Ordered payload execution with per-task delays
- Process lifecycle management and `SceShellUI` lifecycle synchronization
- Thread-safe producer/consumer communication
- Loopback-only HTTP control plane
- Graceful self-termination after sequence completion

## Architecture

`sce-hell0` consists of a small HTTP server built on `libmicrohttpd` and a payload chain orchestrator running within the same ELF binary.

The orchestrator manages multiple independent ELF payloads and executes them in a defined order. Each task may specify an execution delay, while the sequence controller can coordinate execution around the lifecycle of `SceShellUI`.

```text
Client (HTML/JS)
  │
  │ HTTP (loopback)
  ▼
┌────────────────────────────────────────────────────────┐
│ on_request() router                                    │
│   ├── /add_task          ──► task_queue[] (mutex)      │
│   ├── /clear_tasks                                     │
│   └── /trigger_sequence  ──► pthread_create(...)       │
└────────────────────────────────────────────────────────┘
  │
  ▼
┌────────────────────────────────────────────────────────┐
│ sequence_thread()                                      │
│   1. Copy the queue                                    │
│   2. Restart ShellUI                                   │
│   3. Wait for restoration                              │
│   4. Execute payloads sequentially                     │
│   5. Self-terminate                                    │
└────────────────────────────────────────────────────────┘
```

At build time, payloads are collected from `/app/payloads` and embedded directly into `orchestrator.elf`. At runtime, the orchestrator exposes them through the task queue and executes them according to the requested sequence.

## Usage and Directory Structure

You can compile the service as an application or, if you prefer, run only the orchestration layer on your PlayStation 5 device.

The entire automation chain required to develop a WebKit autoloader is provided by `sce-hell0`. However, if you want to automate the execution process of the orchestration payload, you should know that you must host a kernel exploit chain under the `/app` folder. Then, in the final stage of this chain, you need to spawn an `elfldr.elf` at the kernel level, create a TCP socket to communicate with this payload, and spawn the `orchestrator.elf` file with a simple function on the specified port. For an example chain, check out the [arg0WAK/WAK1360-Relapse](https://github.com/arg0WAK/WAK1360-Relapse) repository.

### Client-Side and JavaScript API Usage

Once the orchestration service is running, you can communicate with the local HTTP API (default: `http://127.0.0.1:11110`) via `index.html`. The structure provided under the `app` folder offers ready-to-use JavaScript helpers (`helper.js`) for you to manage your payload sequence.

#### `helper.js`

To streamline your frontend development process, `helper.js` provides the following functions:

- **`log(msg, status)`**: Creates colored log entries on the screen (`info`, `warning`, `success`, `error`). Ideal for tracking process steps.
- **`sleep(ms)`**: Defines an asynchronous wait (delay) time.
- **`fetchWithTimeout(url, ms, options)`**: Sends HTTP requests (fetch) with timeout support to prevent potential hangs.
- **`checkFileExists(fileUrl)`**: Checks whether the specified file (e.g., `orchestrator.elf`) is accessible.

#### Example Workflow (`/app/index.html`)

### Project Structure and Configuration Guidelines

- **`main.c` Configuration**: Before compiling, you must edit the `main.c` file in the source code according to your project. Set the `TITLE_ID` for your application, and if you are using a different ELF Loader, update the `ELFLDR_PORT` (default: `9021`) according to the port of your loader.

- **`orchestrator.c` Configuration**: The maximum number of tasks (payloads) the `orchestrator.elf` file can hold is determined by the `MAX_TASKS` constant found in the `orchestrator.c` source code. The default value is **10**. If your chain requires more than 10 payloads, you must increase this value before compiling.

- **Application Icon (`assets/icon0.png`)**: To customize the application icon, you can replace the `icon0.png` file under this folder with your own custom icon.

- **`/install`**: Only your installer interface, `index.html`, should reside in this folder. You can reference the existing `index.html` in this folder for your personal development processes.

- **`/app`**: Only the dependencies of the application to be installed should be placed in this folder.

> **\*IMPORTANT:** Please **do not edit** the `cache.appcache` file located under this folder; it is managed by the build and caching mechanism. Define the directories you want to exclude during compilation under the `static const char *const excluded[] = {` variable located in `main.c`.

- **`/app/payloads`**: Host your `.elf` files expected to be included in the generated `orchestrator.elf` file here. **Note:** The contents of this folder are not kept in the AppCache during the build process; they are solely embedded directly into the `orchestrator.elf` file.

```bash
# Test builds
┌────────────────────────────────────────────────────────┐
│ /app/payloads                                          │
│   ├── sce-hell0-payload-1.elf                          │
│   ├── sce-hell0-payload-2.elf                          │
│   ├── sce-hell0-payload-3.elf                          │
│   ├── sce-hell0-payload-4.elf                          │
│   └── sce-hell0-payload-5.elf                          │
└────────────────────────────────────────────────────────┘
```

## Build Instructions

To compile the project, you must have the [ps5-payload-dev/sdk](https://github.com/ps5-payload-dev/sdk) installed and configured in your environment. You can install it to the SDK path specified in the `Makefile` or update the file according to your preference.

```Makefile
SDK    := /opt/ps5-payload-sdk
```

To compile along with an installer, use the following command:

```bash
make clean && make
```

Once the build process is complete, you need to send the resulting `installer.elf` file to your PS5 device over the network (e.g., using `Netcat`) to trigger the installer.

```bash
nc <PS5_IP> <ELFLDR_PORT> < installer.elf
```

If you only need the orchestration payload, you can use the `orchestrator.elf` file located under the `/install/payloads` folder after a successful build.

## API Endpoints

| Endpoint                 | Method | Description                                                           |
| ------------------------ | ------ | --------------------------------------------------------------------- |
| `/add_task?name=&delay=` | GET    | Adds a new task to the queue.                                         |
| `/clear_tasks`           | GET    | Clears the queue, including leftover tasks from a previous execution. |
| `/trigger_sequence`      | GET    | Starts executing the queued tasks sequentially.                       |

## Technical Details

### Concurrency Model

- The server runs with `MHD_USE_INTERNAL_POLLING_THREAD` and 2 worker threads.

- `task_queue` and `task_count` are protected by the `task_lock` mutex.

- The `kill_pending` atomic flag ensures that only one sequence can run at a time using `atomic_exchange`.

- As soon as `sequence_thread` starts, it copies the queue and clears the original; this prevents an interrupted execution from leaving the queue in an inconsistent state.

## Dependencies

- [ps5-payload-dev/sdk](https://github.com/ps5-payload-dev/sdk)

- [karlson2k/libmicrohttpd](https://github.com/karlson2k/libmicrohttpd) (Vendored static library on Unofficial Mirror)

## Scope

`sce-hell0` is intended for homebrew development and authorized research on user-owned or explicitly authorized systems.

The authors are not responsible for misuse or operation outside authorized environments.

No physical disc copies were harmed during the development of this project.

No discs were required, either.
