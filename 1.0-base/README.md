# Version 1.0-base - Basic Shell

## Description
This is the foundational version of the custom shell with basic command parsing and execution capabilities.

## Features
- Basic command input loop
- Command tokenization
- Fork-exec process model
- Simple error handling

## Building

```bash
make clean   # Clean previous builds
make         # Build the shell
make run     # Build and run
```

## Running

```bash
./bin/myshell
```

## Testing

```bash
PUCIT> ls -la
PUCIT> pwd
PUCIT> echo "Hello World"
PUCIT> whoami
```

## Files
- `src/main.c` - Main program loop
- `src/shell.c` - Shell utilities (read_cmd, tokenize)
- `src/execute.c` - Command execution
- `include/shell.h` - Header definitions
- `Makefile` - Build configuration

## Limitations
- No built-in commands
- No history
- No tab completion
- No I/O redirection

## Next Version
See `1.1-builtins` for built-in command support.
