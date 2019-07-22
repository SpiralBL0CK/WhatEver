#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main(int argc,char**argv)
{
	int counter = 0;
	char op[256],arg1[50],arg2[50],result[50];
	FILE *fp1,*fp2;

	if(!(fp1 = fopen((char*)argv[1],"r")))
	{
		printf("could not open:[%s]",(char*)argv[1]);	
	}

	if(!(fp2 = fopen("output.txt","w")))
	{
		printf("could not open output file,probably doesnt exist");
	}

	while(!feof(fp1))
	{
		fscanf(fp1,"%s%s%s%s",op,arg1,arg2,result);
		//NEED TO IMPLEMENT SMTHING if one of argument == label,set cursor at beggining
		//DEBUG INFO FOR HELMPING TO SOLVE BUGS :)
		printf("%s\n",op);

		//NEED TO IMPLEMENT SMTHING if one of argument == label,set cursor at beggining
		//actuall algorithm

		if(strcmp(op,"start")==0)
		{
			fprintf(fp2,"\nglobal _%s",arg1);
      		fprintf(fp2,"%s","\n");
      		fprintf(fp2,"\%s","section .text");
			fprintf(fp2,"%s","\n");
			fprintf(fp2,"_%s:\n%3s\n",arg1,"xor rax,rax");
			continue;
		}
		if(strcmp(op,"new_stack")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
      		fprintf(fp2,"%s","\nmov $rsp,$rbp");
      		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
		}

		if(strcmp(op,"+")==0)
		{
			fprintf(fp2,"\nMOV R0,%s",arg1);
      		fprintf(fp2,"\nADD R0,%s",arg2);
      		fprintf(fp2,"\nMOV %s,R0",result);
			continue;
			
		}

		if(strcmp(op,"*")==0)
		{
        	fprintf(fp2,"\nMOV R0,%s",arg1);
        	fprintf(fp2,"\nMUL R0,%s",arg2);
       	 	fprintf(fp2,"\nMOV %s,R0",result);
        	continue;
        }

		if(strcmp(op,"*")==0)
    	{
			fprintf(fp2,"\nMOV R0,%s",arg1);
  			fprintf(fp2,"\nMUL R0,%s",arg2);
			fprintf(fp2,"\nMOV %s,R0",result);
   	 		continue;
		}
		if(strcmp(op,"-")==0)
		{
		fprintf(fp2,"\nMOV R0,%s",arg1);
		fprintf(fp2,"\nSUB R0,%s",arg2);
      	fprintf(fp2,"\nMOV %s,R0",result);
    	continue;
		}
		if(strcmp(op,"/")==0)
    	{
      		fprintf(fp2,"\nMOV R0,%s",arg1);
      		fprintf(fp2,"\nDIV R0,%s",arg2);
      		fprintf(fp2,"\nMOV %s,R0",result);
    		continue;
		}
		if(strcmp(op,"=")==0)
    	{
     		fprintf(fp2,"\nMOV R0,%s",arg1);
      		fprintf(fp2,"\nMOV %s,R0",result);
    		continue;
		}
		if(strcmp(op,"syscall")==0)
    	{
     		fprintf(fp2,"%s","\nSYSCALL\n");
    		continue;
		}
			/*NEED TO IMPLEMENT IT,COMMON IT EZ */
		if(strcmp(op,"delete_stack")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
      		fprintf(fp2,"%s","\nmov $rsp,$rbp");
      		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
			
		}
			/*NEED TO IMPLEMENT IT,COMMON IT EZ,just remember push on the stack addr of struct */
		if(strcmp(op,"push_structure")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
      		fprintf(fp2,"%s","\nmov $rsp,$rbp");
      		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
		}
			/*TAKE FROM REGISTER / STACK AND THAN APPLY OFFSET,STILL NEEDS TO BE IMPLEMENTED,DONT BE LAZY U HAVE TO DO IT TODAY  */
		if(strcmp(op,"access_structure_member")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
    		fprintf(fp2,"%s","\nmov $rsp,$rbp");
    		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
		}

		//Still needs to be implemented.....
		if(strcmp(op,"create_for_loop")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
    		fprintf(fp2,"%s","\nmov $rsp,$rbp");
    		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
		}

		//Still needs to be implemented.....
		if(strcmp(op,"create_function_with_callback")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
    		fprintf(fp2,"%s","\nmov $rsp,$rbp");
    		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
		}
		/* since we are here why not implement a fct for a web-sv with routing huh >??
		/  Yeah thats for shure to be implemented */
		if(strcmp(op,"create_web_server")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
    		fprintf(fp2,"%s","\nmov $rsp,$rbp");
    		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
		}

		//why not create a function to ping???
		if(strcmp(op,"ping_network")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
    		fprintf(fp2,"%s","\nmov $rsp,$rbp");
    		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
		}

		//create reverse-tcp shell
		if(strcmp(op,"ping_network")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
    		fprintf(fp2,"%s","\nmov $rsp,$rbp");
    		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
		}

		//create-shellcode-out-of-0day
		if(strcmp(op,"ping_network")==0)
		{
			fprintf(fp2,"%s","\npush $rbp");
    		fprintf(fp2,"%s","\nmov $rsp,$rbp");
    		fprintf(fp2,"\nsub $rsp,%s",arg1);
			continue;
		}
	}
	
	fclose(fp1);
	fclose(fp2);
	return 0;	
	
}

