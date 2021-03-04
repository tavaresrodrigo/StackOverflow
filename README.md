# StackOverflow

This repo was created to demonstrate the operation of a buffer overflow exploit, we are going to change a password variable within a vulnerable program which might illustrate for example how can an attacker grant access to an assumed program which requires authentication by overwriting the current/real password in the stack memory of a C program affecting **CONFIDENTIALITY AND INTEGRITY**. 

We will also impair the availability of the program generating a segmentation fault error which occurs when when a program attempts to access a memory location that it is not allowed to access, a violation in memory in most standards computers will trigger the OS kernel to perform a corrective action leading the program to be terminated, affecting the **AVAILABILITY** 

## A bit of memory 

Understanding stack-based overflow attacks involves at least a basic understanding of computer memory, let's review some very important concepts. 

### Stack memory: 

A stack is the memory which stores variables created by a function, it's temporary, when the program is finished, the memory of the variable will be automatically erased and the area freed up, the stack is managed by the CPU – so you don’t have to allocate or deallocate memory. The stack section mostly contains methods, local variable, and reference variables. 

![stack memory graph ](https://github.com/tavaresrodrigo/StackOverflow/blob/main/stack.png)
### Heap memory

The heap is the memory used by programming languages to store large block of memory, it's also known as dynamic memory. Unlike the stack this area of memory is managed by the programmer in C for example, variables are allocated and freed using functions like malloc() and free()

![stack memory vs heap memory comparison ](https://github.com/tavaresrodrigo/StackOverflow/blob/main/stackvsheap.png)
## What is stack overflow exploit?

When the execution stack grows beyond the memory that is reserved for it, you will cause a stack overflow as the stack will eventually consume more memory than is reserved for it and eventually overwrite other buffers in the same stack. 


## The exploit

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

### To reproduce this exploit you will


## References

https://owasp.org/www-community/vulnerabilities/Buffer_Overflow
https://blog.rapid7.com/2019/02/19/stack-based-buffer-overflow-attacks-what-you-need-to-know/