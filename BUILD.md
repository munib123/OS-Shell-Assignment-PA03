# Build Instructions

## Prerequisites

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential libreadline-dev
```

### Fedora/RHEL
```bash
sudo dnf install gcc make readline-devel
```

### macOS
```bash
brew install readline
```

## Building a Specific Version

```bash
# Navigate to version directory
cd 8.0-variables  # or any other version

# Clean previous builds (optional)
make clean

# Build
make

# Run
make run
# or
./bin/myshell
```

## Build All Versions

```bash
#!/bin/bash
for dir in 1.0-base 1.1-builtins 1.2-history 1.3-readline 5-redirection 6.0-multitasking 7.0-if-else 8.0-variables; do
    echo "Building $dir..."
    cd "$dir"
    make clean
    make
    cd ..
done
```

## Makefile Targets

- `make` or `make all` - Build the shell executable
- `make clean` - Remove object files and executable
- `make run` - Build and run the shell
- `make dirs` - Create obj/ and bin/ directories

## Troubleshooting

### Readline Library Not Found

**Error**:
```
fatal error: readline/readline.h: No such file or directory
```

**Solution**:
Install the readline development package (see Prerequisites)

### Linker Error

**Error**:
```
undefined reference to 'readline'
```

**Solution**:
Ensure your Makefile includes `-lreadline` in LDFLAGS:
```makefile
LDFLAGS = -lreadline
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)
```

### Permission Denied

**Error**:
```
bash: ./bin/myshell: Permission denied
```

**Solution**:
```bash
chmod +x bin/myshell
```

## Cross-Version Compatibility

Each version is self-contained and can be built independently. No dependencies between versions.

## Recommended Build Order for Learning

1. 1.0-base (simplest)
2. 1.1-builtins
3. 1.2-history
4. 1.3-readline
5. 5-redirection
6. 6.0-multitasking
7. 7.0-if-else
8. 8.0-variables (complete)
