# My Shell

A custom command-line shell written in C, built to learn the fundamentals of operating systems, processes, and inter-machine communication over SSH.

> *"Focus beats talent when talent doesn't focus."*

---

## About

My Shell is a command-line shell written in C that replicates core features of shells like `bash` and `zsh`, with additional custom commands to control services on a Raspberry Pi over SSH.

---

## Features

- Execute external commands (`ls`, `cat`, `grep`, etc.)
- Directory navigation with `cd`
- Pipe support (`ls | grep .c`)
- Current directory displayed on every prompt
- Built-in commands to manage a Raspberry Pi service over SSH

---

## Requirements

- Linux or WSL (Windows Subsystem for Linux)
- GCC installed
- SSH access configured to your Raspberry Pi using the alias `raspberry` in `~/.ssh/config`
- Key-based SSH authentication (passwordless) recommended

### Setting up the SSH alias

Add the following to `~/.ssh/config`:

```
Host raspberry
    HostName 192.168.1.XXX
    User your_username
```

### Setting up passwordless SSH (optional)

Add to `~/.bashrc` to load your key automatically on startup:

```bash
if [ -z "$SSH_AUTH_SOCK" ]; then
    eval "$(ssh-agent -s)" > /dev/null
    ssh-add ~/.ssh/id_ed25519 2>/dev/null
fi
```

---

## Build and run

```bash
gcc main.c -o myshell
./myshell
```

---

## Available commands

### Built-ins

| Command | Description |
|---|---|
| `cd <dir>` | Change the current directory |
| `exit` | Exit the shell |
| `help` | Display the list of available commands |

### ChatBot control (Raspberry Pi)

| Command | Description |
|---|---|
| `start chatbot` | Start the housebot service |
| `restart chatbot` | Restart the housebot service |
| `status chatbot` | Show the housebot service status |
| `stop chatbot` | Stop the housebot service |

### Pipes

```bash
ls | grep .c
cat file.txt | grep "word"
```

---

## Project structure

```
my-shell/
├── main.c          # Main loop — ties all modules together
├── shell.h         # Global header with shared #includes and #defines
├── builtin.c       # Built-in commands (cd, exit, help, chatbot control)
├── builtin.h
├── executor.c      # External command execution and pipe handling
├── executor.h
├── parser.c        # Input parsing and pipe detection
├── parser.h
├── raspberry.c     # SSH commands for the Raspberry Pi
├── raspberry.h
├── ui.c            # Banner, prompt, and input reading
├── ui.h
└── Makefile        # Automated build
```

Each module has a single responsibility — `raspberry.c` only knows about SSH, `parser.c` only knows about splitting input, and so on. This makes the codebase easier to read, maintain, and extend.

To build the project:

```bash
make
```

To clean compiled files:

```bash
make clean
```

See `FUNCTIONS.md` for a detailed explanation of every function, and `STRUCTURE.md` for the full code of each module and the Makefile.

---

## How it works

The shell follows the classic **REPL** cycle (Read → Evaluate → Print → Loop):

1. **Read** — reads user input via `fgets`
2. **Parse** — splits the input into tokens and detects pipes
3. **Execute** — decides between built-in, pipe, or external command
4. **Loop** — returns to the prompt

To run external commands, it uses the standard Unix pattern of `fork()` + `execvp()` + `wait()`:
- `fork()` creates a child process
- `execvp()` replaces the child with the target program
- `wait()` makes the parent wait for the child to finish
