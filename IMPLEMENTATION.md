# Implementation Guide

## Overview
This document provides detailed information about the implementation of each version of the custom shell.

## Version Progression

### 1.0-base: Foundation
**Purpose**: Basic shell structure  
**Key Features**:
- Command input loop
- Tokenization of user input
- Fork-exec process model
- Basic error handling

**Implementation Details**:
- `read_cmd()`: Reads user input from stdin
- `tokenize()`: Splits command into argument array
- `execute()`: Forks process and executes command using execvp()

**Files**:
- `src/main.c`: Main program loop
- `src/shell.c`: Utility functions
- `src/execute.c`: Command execution
- `include/shell.h`: Header definitions

---

### 1.1-builtins: Built-in Commands
**Purpose**: Add shell built-in commands  
**New Features**:
- `exit` - Terminates the shell
- `cd` - Changes working directory
- `help` - Displays help information
- `jobs` - Placeholder for job control

**Implementation Details**:
- `handle_builtin()`: Checks if command is built-in before forking
- Built-ins execute in the shell process (no fork)
- Uses `chdir()` system call for directory changes

**Key Code**:
```c
int handle_builtin(char** arglist) {
    if (strcmp(arglist[0], "cd") == 0) {
        chdir(arglist[1]);
        return 1; // Built-in handled
    }
    return 0; // Not a built-in
}
```

---

### 1.2-history: Command History
**Purpose**: Track and recall previous commands  
**New Features**:
- Stores last 20 commands
- `history` command to display past commands
- `!N` syntax to re-execute command number N

**Implementation Details**:
- Global array `char* history[HISTORY_SIZE]`
- `add_to_history()`: Adds commands to circular buffer
- Command recall using `strdup()` to recreate command string

**Data Structure**:
```c
char* history[HISTORY_SIZE];  // Array of command strings
int history_count = 0;         // Number of stored commands
```

---

### 1.3-readline: GNU Readline Integration
**Purpose**: Enhanced user input experience  
**New Features**:
- Tab completion for commands
- Arrow key navigation
- Emacs-style line editing
- Persistent command history across sessions

**Implementation Details**:
- Replaced `read_cmd()` with `readline()` library function
- `rl_bind_key('\t', rl_complete)`: Enables tab completion
- `add_history()`: Adds to readline's internal history

**Build Changes**:
- Added `-lreadline` linker flag in Makefile
- Requires libreadline-dev package

---

### 5-redirection: I/O Redirection & Piping
**Purpose**: File I/O and process piping  
**New Features**:
- Input redirection: `cmd < file`
- Output redirection: `cmd > file`
- Piping: `cmd1 | cmd2`

**Implementation Details**:

**Tokenization Update**:
```c
// Special handling for <, >, |
if (*p == '<' || *p == '>' || *p == '|') {
    arglist[argnum] = malloc(2);
    arglist[argnum][0] = *p;
    arglist[argnum][1] = '\0';
    argnum++;
    p++;
}
```

**Piping**:
```c
int fds[2];
pipe(fds);

if (fork() == 0) {
    dup2(fds[1], STDOUT_FILENO);  // Write end
    close(fds[0]); close(fds[1]);
    execvp(left[0], left);
}

if (fork() == 0) {
    dup2(fds[0], STDIN_FILENO);   // Read end
    close(fds[0]); close(fds[1]);
    execvp(right[0], right);
}
close(fds[0]); close(fds[1]);
```

**File Redirection**:
```c
int fd = open(filename, O_RDONLY);  // Input
dup2(fd, STDIN_FILENO);

int fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);  // Output
dup2(fd, STDOUT_FILENO);
```

---

### 6.0-multitasking: Background Processes
**Purpose**: Run commands in background  
**New Features**:
- Background execution: `cmd &`
- Process tracking and cleanup
- Enhanced `jobs` command
- Command chaining with `;`

**Implementation Details**:

**Job Structure**:
```c
typedef struct {
    int pid;
    char cmd[MAX_LEN];
} job_t;

job_t bg_jobs[MAX_JOBS];
int bg_count = 0;
```

**Background Detection**:
```c
if (cmd[len-1] == '&') {
    background = 1;
    cmd[len-1] = '\0';  // Remove &
}
```

**Job Cleanup (per iteration)**:
```c
for (int i = 0; i < bg_count; ) {
    pid_t finished = waitpid(bg_jobs[i].pid, &status, WNOHANG);
    if (finished > 0) {
        // Remove from job list
        for (int j = i; j < bg_count - 1; j++)
            bg_jobs[j] = bg_jobs[j + 1];
        bg_count--;
    } else {
        i++;
    }
}
```

**Command Chaining**:
```c
char* token = strtok(cmdline, ";");
while (token != NULL) {
    commands[n_cmds++] = token;
    token = strtok(NULL, ";");
}
```

---

### 7.0-if-else: Conditional Execution
**Purpose**: Control flow structures  
**New Features**:
- If-else-fi syntax
- Multi-line input
- Exit status-based branching

**Syntax**:
```bash
if test -f filename
then
    cat filename
else
    echo "File not found"
fi
```

**Implementation**:
```c
void handle_if_structure(char *first_line) {
    char *if_cmd = strdup(first_line + 3);  // Skip "if "
    
    // Read then and else blocks
    while (fgets(line, sizeof(line), stdin)) {
        if (strcmp(line, "then") == 0) continue;
        if (strcmp(line, "else") == 0) { in_else = 1; continue; }
        if (strcmp(line, "fi") == 0) break;
        
        if (!in_else)
            then_block[then_count++] = strdup(line);
        else
            else_block[else_count++] = strdup(line);
    }
    
    // Execute test command
    pid_t pid = fork();
    if (pid == 0) {
        execvp(if_args[0], if_args);
        exit(1);
    }
    waitpid(pid, &status, 0);
    int exit_code = WEXITSTATUS(status);
    
    // Execute chosen block
    char **chosen_block = (exit_code == 0) ? then_block : else_block;
    for (int i = 0; i < chosen_count; i++) {
        char **args = tokenize(chosen_block[i]);
        execute(args, 0);
    }
}
```

---

### 8.0-variables: Variable Management
**Purpose**: Shell variable support  
**New Features**:
- Variable assignment: `VAR=value`
- Variable expansion: `echo $VAR`
- `set` command to list variables

**Data Structure**:
```c
typedef struct shell_var {
    char *name;
    char *value;
    struct shell_var *next;
} shell_var_t;

shell_var_t *var_list = NULL;  // Linked list
```

**Variable Assignment**:
```c
char *eq_pos = strchr(arglist[0], '=');
if (eq_pos && eq_pos != arglist[0]) {
    *eq_pos = 0;  // Split at =
    set_variable(arglist[0], eq_pos + 1);
}
```

**Variable Expansion**:
```c
void expand_variables(char **args) {
    for (int i = 0; args[i]; i++) {
        if (args[i][0] == '$') {
            char *val = get_variable(args[i] + 1);
            if (val) {
                free(args[i]);
                args[i] = strdup(val);
            }
        }
    }
}
```

**Storage Functions**:
```c
void set_variable(const char *name, const char *value) {
    // Update if exists, otherwise add to front of list
    shell_var_t *new_var = malloc(sizeof(shell_var_t));
    new_var->name = strdup(name);
    new_var->value = strdup(value);
    new_var->next = var_list;
    var_list = new_var;
}

char* get_variable(const char *name) {
    shell_var_t *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0)
            return curr->value;
        curr = curr->next;
    }
    return NULL;
}
```

---

## Building and Testing

### Build Process
```bash
cd <version-folder>
make clean
make
./bin/myshell
```

### Testing Each Feature

**1.0-base**:
```bash
PUCIT> ls -la
PUCIT> pwd
PUCIT> echo "Hello World"
```

**1.1-builtins**:
```bash
PUCIT> help
PUCIT> cd /tmp
PUCIT> pwd
PUCIT> exit
```

**1.2-history**:
```bash
PUCIT> echo test1
PUCIT> echo test2
PUCIT> history
PUCIT> !1
```

**1.3-readline**:
```bash
PUCIT> ec<TAB>    # Auto-completes to echo
PUCIT> <UP>       # Previous command
```

**5-redirection**:
```bash
PUCIT> echo "Hello" > output.txt
PUCIT> cat < output.txt
PUCIT> ls | grep txt
```

**6.0-multitasking**:
```bash
PUCIT> sleep 10 &
PUCIT> jobs
PUCIT> echo A; echo B; echo C
```

**7.0-if-else**:
```bash
PUCIT> if test -f myfile
> then
> cat myfile
> else
> echo "Not found"
> fi
```

**8.0-variables**:
```bash
PUCIT> NAME=John
PUCIT> AGE=25
PUCIT> echo $NAME
PUCIT> echo My name is $NAME and I am $AGE
PUCIT> set
```

---

## Common Issues and Solutions

### Compilation Errors

**Issue**: `fatal error: readline/readline.h: No such file or directory`  
**Solution**: Install readline development library
```bash
sudo apt-get install libreadline-dev
```

**Issue**: `undefined reference to 'readline'`  
**Solution**: Ensure `-lreadline` is in LDFLAGS in Makefile

### Runtime Issues

**Issue**: Background processes become zombies  
**Solution**: Version 6.0+ includes automatic zombie cleanup using `WNOHANG`

**Issue**: Piping doesn't work correctly  
**Solution**: Ensure both file descriptors are closed in parent after forking

**Issue**: Variables not expanding  
**Solution**: Ensure `expand_variables()` is called before `execute()`

---

## Memory Management

### Memory Allocation Points
- `readline()` returns malloc'd string - must free
- `tokenize()` allocates argument array - must free each element
- `strdup()` used for history and variables - must free
- Job structure uses static array - no malloc needed

### Cleanup Checklist
```c
// After each command:
for (int i = 0; arglist[i] != NULL; i++)
    free(arglist[i]);
free(arglist);
free(cmdline);

// For history:
free(history[0]) before overwriting

// For variables:
Current implementation has memory leak on shell exit
Optimal: Add cleanup_variables() function
```

---

## Performance Considerations

- **History**: O(1) access by index, O(n) for search
- **Variables**: O(n) lookup in linked list  
  *Optimization*: Use hash table for O(1) average case
- **Job cleanup**: O(n) per command iteration  
  *Acceptable*: n is typically small (< 50)

---

## Future Enhancements

1. **Signal Handling**: Ctrl+C, Ctrl+Z support
2. **Job Control**: fg, bg commands
3. **Wildcards**: Glob pattern expansion
4. **Scripting**: Read commands from file
5. **Environment Variables**: Export/import support
6. **Aliases**: Command shortcuts
7. **Functions**: User-defined shell functions
8. **Arrays**: Array variable support
9. **Advanced Redirection**: Here documents (<<), append (>>)
10. **Subshells**: Command substitution $()

---

## References

- Advanced Programming in the UNIX Environment (Stevens & Rago)
- GNU Readline Library Documentation
- POSIX.1-2017 System Interfaces
- Linux man pages: fork(2), exec(3), pipe(2), dup2(2)
