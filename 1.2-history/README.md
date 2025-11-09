# Version 1.2-history - Command History

## Description
Adds command history tracking with recall capability.

## New Features
- Stores last 20 commands in memory
- `history` built-in command to display past commands
- `!N` syntax to re-execute command number N
- Circular buffer for history storage

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
PUCIT> echo test1
PUCIT> echo test2
PUCIT> ls -la
PUCIT> history
1  echo test1
2  echo test2
3  ls -la
PUCIT> !1
PUCIT> echo test1
test1
```

## Implementation Notes
- History stored in global array `char* history[HISTORY_SIZE]`
- Commands added after parsing but before execution
- History recall uses `strdup()` to recreate command
- Circular buffer when HISTORY_SIZE is exceeded

## Files
- All files from 1.1-builtins
- Modified `src/shell.c` - Added history array and functions
- Modified `src/main.c` - Added history recall logic
- Modified `include/shell.h` - Added HISTORY_SIZE define

## Previous Version
`1.1-builtins` - Built-in commands

## Next Version
`1.3-readline` - GNU Readline integration
