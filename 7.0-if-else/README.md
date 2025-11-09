# Version 7.0-if-else - Control Structures

## Description
Adds support for if-else-fi control flow structures based on command exit status.

## New Features
- If-else-fi syntax
- Multi-line command input
- Exit status-based branching
- Nested command execution

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
# Test if a file exists
PUCIT> if test -f myfile.txt
> then
> cat myfile.txt
> else
> echo "File not found"
> fi

# Test directory
PUCIT> if test -d /tmp
> then
> echo "Directory exists"
> fi

# Test with command
PUCIT> if grep -q "hello" file.txt
> then
> echo "Found hello"
> else
> echo "Not found"
> fi
```

## Syntax

```bash
if <test_command>
then
    <command1>
    <command2>
else
    <command3>
    <command4>
fi
```

## Implementation Notes

- `handle_if_structure()` reads multi-line input until `fi`
- Test command is executed and exit status checked
- Exit code 0 = success → execute then block
- Exit code non-zero = failure → execute else block
- Commands in chosen block are tokenized and executed

## Technical Details

**Structure Detection**:
```c
if (strncmp(cmdline, "if", 2) == 0) {
    handle_if_structure(cmdline);
}
```

**Exit Status Check**:
```c
waitpid(pid, &status, 0);
int exit_code = WEXITSTATUS(status);
char **chosen_block = (exit_code == 0) ? then_block : else_block;
```

## Files
- All files from 6.0-multitasking
- Modified `src/shell.c` - Added `handle_if_structure()`
- Modified `src/main.c` - Added if-else detection
- Modified `include/shell.h` - Added function declaration

## Limitations
- No elif support
- No nested if-else
- Commands must be simple (no redirection in blocks)

## Previous Version
`6.0-multitasking` - Background processes

## Next Version
`8.0-variables` - Variable support (FINAL)
