# Version 5-redirection - I/O Redirection & Piping

## Description
Adds support for input/output redirection and inter-process piping.

## New Features
- Input redirection: `command < input_file`
- Output redirection: `command > output_file`
- Pipe support: `command1 | command2`
- Enhanced tokenizer to handle special characters

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
# Output redirection
PUCIT> echo "Hello World" > output.txt
PUCIT> cat output.txt
Hello World

# Input redirection
PUCIT> cat < output.txt
Hello World

# Piping
PUCIT> ls -la | grep txt
PUCIT> cat output.txt | wc -l
```

## Implementation Notes
- Tokenizer now treats `<`, `>`, `|` as separate tokens
- Uses `pipe()` system call to create pipe
- Uses `dup2()` to redirect file descriptors
- Uses `open()` for file redirection
- Parent process closes pipe ends after forking children

## Technical Details

**Piping**:
```c
int fds[2];
pipe(fds);
// Child 1: dup2(fds[1], STDOUT_FILENO)
// Child 2: dup2(fds[0], STDIN_FILENO)
```

**File Redirection**:
```c
// Input: dup2(open(file, O_RDONLY), STDIN_FILENO)
// Output: dup2(open(file, O_WRONLY|O_CREAT|O_TRUNC), STDOUT_FILENO)
```

## Files
- All files from 1.3-readline
- Modified `src/shell.c` - Enhanced tokenizer
- Modified `src/execute.c` - Added pipe and redirection logic
- Modified `include/shell.h` - Added fcntl.h include

## Previous Version
`1.3-readline` - GNU Readline integration

## Next Version
`6.0-multitasking` - Background processes and job control
