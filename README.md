# Custom Shell Implementation - OS Programming Assignment PA-03

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

A comprehensive custom Unix shell implementation developed as part of the Operating Systems course. This project demonstrates progressive shell features from basic command execution to advanced features like variables, control structures, and process management.

## 📋 Table of Contents
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [Building and Running](#building-and-running)
- [Author](#author)
- [License](#license)

## ✨ Features

### 1.0 Base Shell
- Basic command parsing and execution
- Simple prompt (`PUCIT> `)
- Fork-exec model for command execution

### 1.1 Built-in Commands
- `cd <directory>` - Change working directory
- `exit` - Exit the shell
- `help` - Display available commands
- `jobs` - Job control placeholder

### 1.2 Command History
- Store up to 20 recent commands
- `history` command to view past commands
- `!N` syntax to re-execute command number N

### 1.3 Readline Integration
- GNU Readline library support
- Tab completion for commands
- Advanced line editing capabilities
- Arrow keys for command navigation

### 5.0 I/O Redirection and Piping
- Input redirection: `command < input_file`
- Output redirection: `command > output_file`
- Pipe support: `command1 | command2`
- Proper file descriptor management

### 6.0 Multitasking
- Background process execution: `command &`
- Process tracking and management
- Automatic background job cleanup
- `jobs` command to list running background processes

### 7.0 Control Structures
- If-else conditional execution
- Multi-line command input
- Exit status-based branching
- Syntax:
  ```bash
  if <test_command>
  then
      <commands>
  else
      <commands>
  fi
  ```

### 8.0 Variables
- Variable assignment: `VAR=value`
- Variable expansion: `$VAR`
- `set` command to list all variables
- Dynamic variable storage with linked list

## 📁 Project Structure

```
.
├── 1.0-base/                  # Basic shell implementation
├── 1.1-builtins/             # Built-in commands
├── 1.2-history/              # Command history
├── 1.3-readline/             # Readline integration
├── 5-redirection/            # I/O redirection and pipes
├── 6.0-multitasking/         # Background processes
├── 7.0-if-else/              # If-else structures
└── 8.0-variables/            # Variable support

Each implementation contains:
├── src/
│   ├── main.c               # Main program loop
│   ├── shell.c             # Shell utilities
│   └── execute.c           # Command execution
├── include/
│   └── shell.h             # Header file
├── Makefile                # Build configuration
└── LICENSE                 # MIT License
```

## 🚀 Installation

### Prerequisites
- GCC compiler
- GNU Make
- GNU Readline library (for versions 1.3 onwards)

### Install Dependencies (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential libreadline-dev
```

### Clone Repository
```bash
git clone https://github.com/munib123/OS-Shell-Assignment-PA03.git
cd OS-Shell-Assignment-PA03
```

## 💻 Usage

### Building
Navigate to any implementation folder and build:
```bash
cd 8.0-variables  # or any other version
make
```

### Running
```bash
make run
# or directly
./bin/myshell
```

### Example Commands
```bash
PUCIT> ls -la
PUCIT> cd /tmp
PUCIT> echo "Hello World" > output.txt
PUCIT> cat output.txt
PUCIT> ls | grep txt
PUCIT> sleep 10 &
PUCIT> jobs
PUCIT> history
PUCIT> !1
PUCIT> NAME=John
PUCIT> echo $NAME
PUCIT> set
PUCIT> if test -f output.txt
> then
> cat output.txt
> else
> echo "File not found"
> fi
PUCIT> exit
```

### Cleaning
```bash
make clean
```

## 🔧 Implementation Details

### Architecture
- **Parser**: Tokenizes input into command arguments
- **Executor**: Handles command execution using fork-exec
- **Built-ins**: Directly implemented in shell process
- **I/O Redirection**: File descriptor manipulation with dup2
- **Process Management**: Job tracking with PID arrays
- **Variable Storage**: Linked list for dynamic variable management

### Key Files
- `main.c` - Main loop, readline integration, command chaining
- `shell.c` - Parsing, tokenization, built-in commands, control structures
- `execute.c` - Fork-exec implementation, piping, I/O redirection
- `shell.h` - Function declarations, constants, data structures

### Memory Management
- Proper malloc/free usage
- Command history with circular buffer
- Dynamic argument list allocation
- Background job cleanup

## 📚 Building and Running

Each version builds independently:

```bash
# Build specific version
cd 1.3-readline
make

# Run
make run

# Clean build artifacts
make clean
```

## 👨‍💻 Author

**Student ID**: BSDSF23M040  
**Course**: Operating Systems  
**Assignment**: PA-03  
**Institution**: PUCIT (Punjab University College of Information Technology)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- GNU Readline Library
- Unix System Programming concepts
- Operating Systems course instructors

## 📝 Notes

- Each version builds upon the previous one
- Version 8.0 contains all features combined
- The shell prompt is "PUCIT> " as per assignment requirements
- Maximum command length: 512 characters
- Maximum arguments per command: 10
- History size: 20 commands
- Maximum background jobs: 50

---

**⚠️ Educational Purpose**: This project is submitted as part of an academic assignment. Use responsibly.
