From-scratch implementations of an assortment of Unix programs. Written in C. Note that these are college-assignment level, minimal implementations, mainly for demonstrating how these utilities are implemented in Unix. Please DO NOT expect them to conform to POSIX standards.

### shell.c

An implementation of a Bash-like shell.
- Implementation of shell in-built commands (like cd)
- Supports input/output redirection (using '<' and '>' syntax) like in Bash
- uses the GNU readline library to provide features of auto-completion
- Can exec arbitrary executables inside the shell Eg; (./a.out)

#### Possible improvements -
- Add support for piping (|)
- Add support for running background processes (&)

### ls.c

Implementation of list (ls) with support for the flags -l, -a, -R

### ps.c

Basic 'ps' implementation that reads the /proc/ directory and displays the processes in running (R) state.

### rm.c

remove - removes file by using the unlink() system call. rmv2.c can remove directories recursively.

### kill.c

send a signal to a process. Default signal is SIGINT.

### wc.c

Word Count - Counts number of lines/words/characters in a file.

### Compilation

The [GNU make](https://www.gnu.org/software/make/) utility can be used to compile these .c files. The Makefile provided includes goals to compile each program individually or all of them at once.

Eg: To compile shell.c into an executable -
```
make shell
```

To compile all programs at once simply -

```
make
```

To cleanup created executables -

```
make clean
```
