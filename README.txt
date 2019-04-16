Sila Inci
21602415

When the make file is run, you use ./bilshell to invoke my program then "bilshell" is seen in the terminal. When my shell is invoked, you have to either give an input a number from 1 to 4096 in order to run it in interactive mode or give an input like "1 file.txt" to run it in batch mode. 
The builtin shell commands also can bu used in my shell. And the batch mode runs all the commands then it terminates.
When you want to exit interactive mode, you have to give an integer input such as "1" or you can say "exit".
My producer M | consumer M piped command only works from 1 to 10000. If an input is more than that it simply can not run a such large integer and enters an infinite loop.

I have calculated the time using time command. Because "producer 100 | consumer 100" is not a builtin command, I have not been able to run "time producer 100 | consumer 100". I calculated the time of my shell while running a single command such as "producer 100 | consumer 100" and made my experiments. 

