# Filesystem Syscalls

## Examples

Help

```sh
./bin/01-fs_syscalls --help
```

Copy a file

```sh
./bin/01-fs_syscalls --input Makefile --output Makefile_copy --copy
```

Copy the file reversed

```sh
./bin/01-fs_syscalls --input Makefile --output Makefile_copy --reverse
```

Ls like command on . directory

```sh
./bin/01-fs_syscalls --input . --list
```

Ls like command on a file

```sh
./bin/01-fs_syscalls --input Makefile --list
```

Copy a file using custom buffered mode

```sh
./bin/01-fs_syscalls --input Makefile --output Makefile_copy --copy --buffered
```
