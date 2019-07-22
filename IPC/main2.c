#include "stacklib.h" 
#include <stdio.h> 
  
int main() 
{ 
    shstackget(1, 4, 10); 
    shstackget(3, 1, 6); 
    shstackpush(1, 5); 
    shstackpush(1, 9); 
    shstackpush(3, 'k'); 
    shstackpop(1); 
    shstackpush(3, 'b'); 
  
    return 0; 
} 