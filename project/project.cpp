/*This is the server */
#include "networking.h"
int main(int argc,char **argv)
{
   
    HANDLE handle_local_client = (HANDLE)_beginthreadex(0, 0,&check_local_client, 0, 0,0);
    WaitForSingleObject(handle_local_client, INFINITE);
    CloseHandle(handle_local_client);    

    return 0;
}

