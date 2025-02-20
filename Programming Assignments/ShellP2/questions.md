1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use `fork/execvp` instead of `execvp` so that our shell can continously run even if there is a seperate process. Calling `execvp` directly would replace the shell and we would not be able to run more commands afterwards.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() fails, it would return a -1 valueu and a perror would print fork. 

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() searches for the executables in directories listed in the PATH environemtn.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  We call wait() so that the the parent process doesn't continue without the child process occuring first. If we did not call this, the parent process would complete and immediately return with the child process still running.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() retrieves the exit code of the child process. This is important as it allows the shell to check if something executed successfully or failed.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My implementation of build_cmd_buff() handles quoted arguments with character by character parsing. The arguments inside of the quotes are treated as single tokens and this ensures that the command is executed and read as intended.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  In the previous assignment, we parsed commands using pipes. In this assignment, we no longer use pipes but instead use quotes. I changed my old code to handle cmd_buff instead of a list.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  The purpose of signals in a Linux system is to notify processes of events. This is different from other forms of IPC like pipes as they don't send actual data, but instead just tell a process that soemthing happened.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL is a kill signal which cannot be caughtm, blocked, or ignored. It is used to force a process to end. SIGTERM is the default termination signal send by the kill command. Other processes can catch and handle the signal which could allow them to save resources before exiting. SIGINT is the inteript from keyboard signal(CTRL + C). It is used to request a process to terminate.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, they pause and are moved to a stopped state. It can't be caught or ignored since it is supposed to stop a process immediately for for things like debugging.