From-scratch implementations of an assortment of Unix programs. Written in C. Note that these are college-assignment level, minimal implementations, mainly for demonstrating how these utilities are implemented in Unix. Please DO NOT expect them to conform to POSIX standards.

### shell.c

A "from-scratch" implementation of a Bash-like shell.
- Implementation of shell in-built commands (like cd)
- Supports input/output redirection (using '<' and '>' syntax) like in Bash
- uses the GNU readline library to provide features of auto-completion
- Can exec arbitrary executables inside the shell Eg; (./a.out)

### ls.c

Implementation of list (ls) with support for the flags -l, -a, -R
