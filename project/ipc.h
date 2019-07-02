#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#pragma comment(lib, "user32.lib")

#define BUF_SIZE 256
TCHAR szName[]=TEXT("MyFileMappingObject");
TCHAR szMsg[]=TEXT("Message from first process.");

unsigned int __stdcall create_file_ipc(void *data)
{
	printf("in create_file_ipc\n");
	HANDLE hMapFile;
	LPCTSTR pBuf;
	
	hMapFile = CreateFileMapping(
	    INVALID_HANDLE_VALUE,    // use paging file
	    NULL,                    // default security
	    PAGE_READWRITE,          // read/write access
	    0,                       // maximum object size (high-order DWORD)
	    BUF_SIZE,                // maximum object size (low-order DWORD)
	    szName);                 // name of mapping object
	
	if (hMapFile == NULL) {
	    printf("Could not create file mapping object\n");
	    return 1;
	}
	pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
	    FILE_MAP_ALL_ACCESS, // read/write permission
	    0,
	    0,
	    BUF_SIZE);
	
	if (pBuf == NULL) {
		printf("Could not map view of file\n");
	    CloseHandle(hMapFile);
	
	    return 1;
	}
	
	
	CopyMemory((PVOID)pBuf, szMsg, (_tcslen(szMsg) * sizeof(TCHAR)));
	_getch();
	
	UnmapViewOfFile(pBuf);
	
	CloseHandle(hMapFile);
	
	printf("Done init shared mem\n");
	return 0;
}

unsigned int __stdcall read_file_ipc(void *data)
{
	printf("in read_file_ipc\n");
	HANDLE hMapFile;
   	LPCTSTR pBuf;

	   hMapFile = OpenFileMapping(
	                   FILE_MAP_ALL_ACCESS,   // read/write access
	                   FALSE,                 // do not inherit the name
	                   szName);               // name of mapping object
	
	   if (hMapFile == NULL)
	   {
	      _tprintf(TEXT("Could not open file mapping object (%d).\n"),
	             GetLastError());
	      return 1;
	   }
	
	   pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
	               FILE_MAP_ALL_ACCESS,  // read/write permission
	               0,
	               0,
	               BUF_SIZE);
	
	   if (pBuf == NULL)
	   {
	      printf("Could not map view of file (%d).\n"));
	
	      CloseHandle(hMapFile);
	
	      return 1;
	   }
	
	   printf("Process2\n");
	
	   UnmapViewOfFile(pBuf);

	   CloseHandle(hMapFile);
	
	   return 0;
}
