1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation calls waitpid and like before,, it will wait for the children to finish running before the parent starts. If it is not called, then it could result in leaks.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() as they are not infinite and leaving them open could cause a large increase of resource usage.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd is built as a built-in function rather than an external command so that it can still modify the shell's directory. If cd were to be implemented as an external command, then it would have no effect on parent processes.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

Instead of using an array that is already defined, we would have to dynamically allocate memory for the command list. The trade-off we would have to consider is ensuring correct memory management as we would need to free all allocated memory.