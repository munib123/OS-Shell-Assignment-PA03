# Version 1.1-builtins - Built-in Commands

## Description
Adds support for shell built-in commands that execute within the shell process.

## New Features
- `exit` - Exit the shell
- `cd <directory>` - Change working directory
- `help` - Display help information
- `jobs` - Job control placeholder

## Building

```bash
make clean
make
make run
```

## Running

```bash
./bin/myshell
```

## Testing

```bash
PUCIT> help
PUCIT> cd /tmp
PUCIT> pwd
PUCIT> cd ..
PUCIT> exit
```

## Implementation Notes
- Built-in commands are checked before forking
- `chdir()` system call used for directory changes
- Built-ins return 1 to indicate they were handled

## Files
- All files from 1.0-base
- Modified `src/shell.c` - Added `handle_builtin()`
- Modified `src/main.c` - Checks built-ins before execute

## Previous Version
`1.0-base` - Basic shell

## Next Version
`1.2-history` - Command history support
