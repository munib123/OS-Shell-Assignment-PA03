# Version 6.0-multitasking - Background Processes & Job Control

## Description
Adds support for background process execution, job tracking, and command chaining.

## New Features
- Background execution: `command &`
- Job tracking and management
- Automatic zombie process cleanup
- Command chaining with `;`
- Enhanced `jobs` built-in command

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
# Background processes
PUCIT> sleep 5 &
[1] PID: 12345  Command: sleep
PUCIT> jobs
[1] PID: 12345  Command: sleep

# Command chaining
PUCIT> echo First; echo Second; echo Third
First
Second
Third

# Combined features
PUCIT> sleep 10 & echo Done; ls
```

## Implementation Notes

**Job Structure**:
```c
typedef struct {
    int pid;
    char cmd[MAX_LEN];
} job_t;
```

- Jobs stored in array `job_t bg_jobs[MAX_JOBS]`
- Background detection by checking for `&` at end of command
- Zombie cleanup using `waitpid(..., WNOHANG)` each iteration
- Command chaining using `strtok()` with `;` delimiter

## Technical Details

**Background Detection**:
```c
if (cmd[len-1] == '&') {
    background = 1;
    cmd[len-1] = '\0';  // Remove &
}
```

**Zombie Cleanup**:
```c
waitpid(bg_jobs[i].pid, &status, WNOHANG);
// Remove from array if finished
```

## Files
- All files from 5-redirection
- Modified `src/main.c` - Added job cleanup loop and command chaining
- Modified `src/shell.c` - Added job array and enhanced jobs command
- Modified `src/execute.c` - Added background parameter
- Modified `include/shell.h` - Added job_t structure

## Previous Version
`5-redirection` - I/O redirection and piping

## Next Version
`7.0-if-else` - If-else control structures
