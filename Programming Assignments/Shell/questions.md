1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice for this application as it prevents buffer overflows. It allows the program to deny commands that are too long since we will limit the characters read.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  We use malloc() instead of allocating a fixed-size array because malloc allows us to dynamically change the size of the array. With a fixed-size array, we are prone to either extensive memory usage beyond what is required.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming leading and trailing whitespaces is necessary because we only want to parse the part of the command that we should be processing. Without this, there could be problems with the size of the command or it could be evaluated incorrectly.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: One example is redirecting an output into a file. The problem with this would be file formatting and permissions. Another example is input redirection. The challenge of this would be properly opening the file and error handling in case there is something missing. The last example is handling output appending. We would have to change the way to detect certain commands like instead of `>`, we have `>>`.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  The key differences between redirection and piping is that redirection deals with sending input or output to or from files. Piping deals with connecting mul;tiple commands instead of dealing w ith files.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep them seperate as it prevents us from mixing up command output with error messages which would help us debug certain issues.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  We should capture these separately from each other. The shell could offer options to merge STDOUT and STDERR so that it will make it easier for some users to see both the output and errors.