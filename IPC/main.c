#include "stacklib.h" 
#include <stdio.h> 

int main()
{
    shstackget(1, 4, 10); 
    shstackget(3, 1, 6); 
    shstackpush(1, 1); 
    shstackpush(1, 2); 
    shstackpush(3, 'a'); 
    shstackpop(1); 
    shstackpush(3, '1'); 
  
    return 0; 
}