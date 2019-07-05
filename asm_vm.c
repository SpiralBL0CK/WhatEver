#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define int long long // work with 64bit target

int *text,*old_text,*stack; //text sgmnt and dump text sgmnt stack of execution
int *pc,*bp,*sp,ax,cycle;
char *data; //data sgmnt
int token; //current token parsed;
char *src,*old_src; //ptr to source code string;
int poolsize; //default size of text/data/stack
int line; //line nr


enum { LEA,IMM,JMP,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
       OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
	    OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT};



void next()
{
	token = *src++;
	return;
}

void expression(int level)
{

}

void program()
{
	next();
	while(token>0){
		printf("token is %c\n",token);
		next();
	}
}


int eval(){
	int op,*tmp;
	while(1){
		op = *pc++;

		if(op == IMM) {ax = *pc++;} //load value in ax
		else if (op == LC) {ax = *(char*)ax;} //load char in ax  
		else if (op == LI) {ax = *(int*)ax;} //load int in ax  
		else if (op == SC) {ax = *(char*)*sp++ = ax;} //store char to addr,save addr on top stack,save value in ax  
		else if (op == SI) {*(int*)*sp++ = ax;} //same for si only int data type
		else if (op == PUSH) {*--sp = ax;} //push value of ax on the stacl
		else if (op == JMP) {pc = (int *)*pc;} //JMP <addr> will unconditionally set the value PC register to <addr>.
		else if (op == JZ)   {pc = ax ? pc + 1 : (int *)*pc;}                   // jump if ax is zero
		else if (op == JNZ)  {pc = ax ? (int *)*pc : pc + 1;}                   // jump if ax is not zero
		else if (op == CALL) {*--sp = (int)(pc+1);pc = (int*)*pc;}//call subroutine  save pc to return than jump to code+1
		//else if (op == RET) {pc = (int *)*sp++;} //increment sp such that it will go back to execution point
		else if (op == ENT) {*--sp = (int)bp; sp = sp  - *pc++;} //make new stack frame
		else if (op == ADJ) {sp = sp + *pc++;} // add esp,<size>
		else if (op == LEV) {sp = bp; bp = (int*)*sp++;pc=(int *)*sp++;}//restore call frame from pc
		//here is how stack looks when calling stack frame
		else if (op == LEA) {ax = (int)(bp + *pc++);} //since out ADD is weak we use LEA TO fetch arguments such that arg+4 => sub(arg1,arg2,arc3) => arg1 will be selected
		/*
		sub_function(arg1, arg2, arg3);

		|    ....       | high address
		+---------------+
		| arg: 1        |    new_bp + 4
		+---------------+
		| arg: 2        |    new_bp + 3
		+---------------+
		| arg: 3        |    new_bp + 2
		+---------------+
		|return address |    new_bp + 1
		+---------------+
		| old BP        | <- new BP
		+---------------+
		| local var 1   |    new_bp - 1
		+---------------+
		| local var 2   |    new_bp - 2
		+---------------+
		|    ....       |  low address

		 */
		else if (op == OR) ax = *sp++ | ax; //Each operator has two arguments: the first one is stored on the top of the stack while the second is stored in AX
		/* After the calculation is done, the argument on the stack will be poped out and the result will be stored in AX */
		else if (op == XOR) ax = *sp++ ^ ax;
        else if (op == AND) ax = *sp++ & ax;
        else if (op == EQ)  ax = *sp++ == ax;
        else if (op == NE)  ax = *sp++ != ax;
        else if (op == LT)  ax = *sp++ < ax;
        else if (op == LE)  ax = *sp++ <= ax;
        else if (op == GT)  ax = *sp++ >  ax;
        else if (op == GE)  ax = *sp++ >= ax;
        else if (op == SHL) ax = *sp++ << ax;
        else if (op == SHR) ax = *sp++ >> ax;
        else if (op == ADD) ax = *sp++ + ax;
        else if (op == SUB) ax = *sp++ - ax;
        else if (op == MUL) ax = *sp++ * ax;
        else if (op == DIV) ax = *sp++ / ax;
        else if (op == MOD) ax = *sp++ % ax;


        else if (op == EXIT) { printf("exit(%d)", *sp); return *sp;}
        else if (op == OPEN) { ax = open((char *)sp[1], sp[0]); }
        else if (op == CLOS) { ax = close(*sp);}
        else if (op == READ) { ax = read(sp[2], (char *)sp[1], *sp); }
        else if (op == PRTF) { tmp = sp + pc[1]; ax = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]); }
        else if (op == MALC) { ax = (int)malloc(*sp);}
        else if (op == MSET) { ax = (int)memset((char *)sp[2], sp[1], *sp);}
		else if (op == MCMP) { ax = memcmp((char *)sp[2], (char *)sp[1], *sp);}
		else {
    		printf("unknown instruction:%d\n", op);
    		return -1;
		}
	}
	return 0;
}

int main(int argc,char **argv)
{
	int i,fd;
	argc--;
	argv++;
	poolsize = 256 * 1024; // arbitrary size
	line=1;
	if((fd=open(*argv,0))<0) 
	{
		printf("could not open(%s)\n",*argv);
		return -1;
	}
	if(!(src = old_src = malloc(poolsize)))
	{
		printf("could not malloc(%d) for source area\n", poolsize);
		return -1;
	}
	if((i=read(fd,src,poolsize-1))<0)
	{
		printf("read() returned %d\n",i);
		return -1;
	}
	src[i] = 0; //EOF CHAR
	close(fd);

	close(fd);

	if(!(text = old_text = malloc(poolsize)))
	{
		printf("could not malloc(%d) for text area\n",poolsize);
		return -1;
	}

	if(!(data = malloc(poolsize)))
	{
		printf("could not malloc(%d) for data area\n",poolsize);
		return -1;
	}
	if(!(stack = malloc(poolsize)))
	{
		printf("could not malloc(%d) for stack area\n",poolsize);
		return -1;
	}

	memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);

	bp = sp = (int *)((int)stack+poolsize);
	ax = 0;

	i = 0;
	text[i++] = IMM;
    text[i++] = 10;
    text[i++] = PUSH;
    text[i++] = IMM;
    text[i++] = 20;
    text[i++] = ADD;
    text[i++] = PUSH;
    text[i++] = EXIT;

	pc = text;
	program();
	return eval();
}

