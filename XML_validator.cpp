// Chech_md5.cpp : Defines the entry point for the application.
//
#pragma comment(lib,"bufferoverflowU.lib")
#include "stdafx.h"
#include "resource.h"
#include <windowsx.h>
#include "WINBASE.H"
#include <WinUser.h>
#include <commdlg.h> //dialog
#include <commctrl.h>
//#define MAX_LOADSTRING 100
#include <SHLOBJ.H> //for setdir
#include <SHELLAPI.H>
#include <mmsystem.h>
#include "md5.h"
#include <WINCRYPT.H>
#include <STDIO.H>
#include "zip.h"
#include "unzip.h"
#include "archiving.h"

#define MAX_XML_FILES 1000

#define MAX_TAGS 30
#define MAX_TAG_LEN 64

//#define MAX_LOADSTRING 100
 #define PROJECT_WINDOW_WIDTH	600
#define PROJECT_WINDOW_HIGHT	400
#define SET_PATH_FOR_FOLDERS 507
#define FILE_DIR				6504
#define XML_CHECK_FINISHED  6505
#define XML_CLEAN_FINISHED 6506
#define FIX_XML_FINISHED 6507
//#define LOAD_XLSX 6505
#define   CHECK_ZIP 505
#define CLEAN_INFO 506
#define FIX_XML 508
int File_count=0;

 char TempPath[MAX_PATH];


int check_quote=0;

// Global Variables:
char ProgramPatch[MAX_PATH];
WCHAR FindZip[MAX_PATH];
WCHAR sU[MAX_PATH];;
WCHAR File_list[MAX_XML_FILES][MAX_PATH];
static WCHAR FindFileDirectory_[MAX_PATH]=L"\\";
static HWND hEdit, hEditSourceDirecrory,	hBrowseFolderButton,hLoadXLSX;
HWND hProgressBarWnd, hFixXMLButton, hRemoveImageButton_, hProgressBarWnd2,hOpenCompareWindowButton  , hWaitingWnd, hSelectExtension,hRenameButton, hTranslatedSuffix, hSkipEmptyFiles, hSkipEnglishStrings, hListBoxBU,hOkButton,hFindFilesButton_, hFinfInSubDirectoriesCheckButton,hAddButton,hOpenDBButton, hDeleteButton_, hCancelButton ,hEditDescription,hListBoxSourceLang,hListBoxTargetLang,hEditProjectName,hEditDatabaseName,	hListSourceFiles,hEditPMName,hEditEditSubject,hEditMCN;
RECT rt;
HFONT hUIFont;
HINSTANCE hInst;								// current instance
TCHAR szTitle[100]="XML-Checker";								// The title bar text
TCHAR szWindowClass[100]="XML-Checker";									// The title bar text
SYSTEMTIME sm;
// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hWnd=NULL;

int ver=116; 
char subver[8]=" ";


int Find_Directory3 (WCHAR* file);
int FindXML(WCHAR* file);
int Find_DirectoryForXML(WCHAR* file);


//int Pdf_files=0,Md5_files=0;
int Zip_files=0;
char UserName[64];
DWORD User_name_len=sizeof(UserName);
unsigned long __stdcall SendThread(void *Param);
unsigned long __stdcall Sender2(void *Param);
DWORD dwIdSend,dwIdSend2,dwIdClose ,dwCheckZip ,dwCleanZip;
HANDLE hThreadSend,hThreadSend2,hCheckXML,hCleanXML,hFixXML;;



int  send(char*);
int ValidateXML(WCHAR* file);
int CleanXML(WCHAR* file);
int FixXML(WCHAR* file);
//int CheckXMLtags(file))

unsigned long __stdcall SendThread(void *Param)
{
	
	send(UserName);	
	ExitThread(0);
   return( 0 ); 
}
#define TEXT_BUF_LEN 10000
char TexBuffer[TEXT_BUF_LEN];
int TexBuffer_pos=0;



unsigned long __stdcall CleanXMLThread(void *Param)
{
int i;
WCHAR str[500];
	int len=wcslen(sU);

memset(TexBuffer,0,TEXT_BUF_LEN);
 TexBuffer_pos=0;

		GetWindowTextW(hEditSourceDirecrory,sU, sizeof(sU)-1 );
		if(sU[1]>0)
				{
							File_count=0; 
							memset(File_list,0,MAX_XML_FILES*MAX_PATH*2);
							SendMessageW((HWND)hListSourceFiles, LB_RESETCONTENT,0,0);
							
							wcscpy(FindZip,sU);
							wcscat(FindZip,L"\\");
							wcscat(FindZip,L"*.xml");
							//Find_Directory3(FindZip);

							FindXML(FindZip);

							ShowWindow(hProgressBarWnd, SW_SHOWNORMAL);
							SendMessage( hProgressBarWnd, PBM_SETPOS,0,0);

							ShowWindow(hProgressBarWnd2, SW_SHOWNORMAL);
							SendMessage( hProgressBarWnd2, PBM_SETPOS,0,0);

							for(i=0;i<File_count;i++)
							{
								memset(FindZip,0,MAX_PATH*2);
								wcscpy(FindZip,sU);
								wcscat(FindZip,L"\\");
								wcscat(FindZip,File_list[i]);

								int res=CleanXML(FindZip);
								
								if(res==1)
									swprintf(str,L"OK  %s",&File_list[i]);	
								else
								if(res==0)
									swprintf(str,L"Error in %s",&File_list[i]);	
								else
								if(res==-1)
									swprintf(str,L"Skipped(wrong format) %s",&File_list[i]);	
							 	else
								if(res==-2)
									swprintf(str,L"Skipped(not xml) %s",&File_list[i]);	
									else
								if(res>=2)
									swprintf(str,L"Removed %d item(s)  %s",res-1,&File_list[i]);	
							 
		 
									SetWindowText(hEdit,TexBuffer);
									
									SendMessageW((hListSourceFiles), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)(LPCSTR)str);

 
								SendMessage( hProgressBarWnd2, PBM_SETPOS, (int)100*i/File_count, 0);
							  
							}
							ShowWindow(hProgressBarWnd, SW_HIDE);
							ShowWindow(hProgressBarWnd2, SW_HIDE);
					}
SendMessage(hWnd, WM_COMMAND,XML_CLEAN_FINISHED,0); 
ExitThread(0);
  // return( 0 );   
}

unsigned long __stdcall FixXMLThread(void *Param)
{
int i;
WCHAR str[500];
	int len=wcslen(sU);

memset(TexBuffer,0,TEXT_BUF_LEN);
 TexBuffer_pos=0;

		GetWindowTextW(hEditSourceDirecrory,sU, sizeof(sU)-1 );
		if(sU[1]>0)
				{
							File_count=0; 
							memset(File_list,0,MAX_XML_FILES*MAX_PATH*2);
							SendMessageW((HWND)hListSourceFiles, LB_RESETCONTENT,0,0);
							
							wcscpy(FindZip,sU);
							wcscat(FindZip,L"\\");
							wcscat(FindZip,L"*.xml");
							//Find_Directory3(FindZip);

							FindXML(FindZip);

							ShowWindow(hProgressBarWnd, SW_SHOWNORMAL);
							SendMessage( hProgressBarWnd, PBM_SETPOS,0,0);

							ShowWindow(hProgressBarWnd2, SW_SHOWNORMAL);
							SendMessage( hProgressBarWnd2, PBM_SETPOS,0,0);

							for(i=0;i<File_count;i++)
							{
								memset(FindZip,0,MAX_PATH*2);
								wcscpy(FindZip,sU);
								wcscat(FindZip,L"\\");
								wcscat(FindZip,File_list[i]);

								int res=FixXML(FindZip);
								
							 	if(res==1)
									swprintf(str,L"OK  %s",&File_list[i]);	
								else
								if(res==0)
									swprintf(str,L"Error in %s",&File_list[i]);	
								else
								if(res==-1)
									swprintf(str,L"Skipped(wrong format) %s",&File_list[i]);	
							 	else
								if(res==-2)
									swprintf(str,L"Skipped(not xml) %s",&File_list[i]);	
								else
								if(res>=2)
									swprintf(str,L"Fixed%d item(s) in %s",res-1,&File_list[i]); 	
							 
		 
									SetWindowText(hEdit,TexBuffer);
									
									SendMessageW((hListSourceFiles), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)(LPCSTR)str);

 
								SendMessage( hProgressBarWnd2, PBM_SETPOS, (int)100*i/File_count, 0);
							  
							}
							ShowWindow(hProgressBarWnd, SW_HIDE);
							ShowWindow(hProgressBarWnd2, SW_HIDE);
					}
SendMessage(hWnd, WM_COMMAND,FIX_XML_FINISHED,0); 
ExitThread(0);
  // return( 0 );   
}





unsigned long __stdcall ValidateXMLThread (void *Param)
{
int i;
WCHAR str[500];
	int len=wcslen(sU);

memset(TexBuffer,0,TEXT_BUF_LEN);
 TexBuffer_pos=0;

		GetWindowTextW(hEditSourceDirecrory,sU, sizeof(sU)-1 );
		if(sU[1]>0)
				{
							 File_count=0; 
							memset(File_list,0,MAX_XML_FILES*MAX_PATH*2);
							SendMessageW((HWND)hListSourceFiles, LB_RESETCONTENT,0,0);
							
							wcscpy(FindZip,sU);
							wcscat(FindZip,L"\\");
							wcscat(FindZip,L"*.xml");
							//Find_Directory3(FindZip);

							FindXML(FindZip);

							ShowWindow(hProgressBarWnd, SW_SHOWNORMAL);
							SendMessage( hProgressBarWnd, PBM_SETPOS,0,0);

							ShowWindow(hProgressBarWnd2, SW_SHOWNORMAL);
							SendMessage( hProgressBarWnd2, PBM_SETPOS,0,0);

							for(i=0;i<File_count;i++)
							{
								memset(FindZip,0,MAX_PATH*2);
								wcscpy(FindZip,sU);
								wcscat(FindZip,L"\\");
								wcscat(FindZip,File_list[i]);

								int res=ValidateXML(FindZip);
								
								if(res==1)
									swprintf(str,L"OK  %s",&File_list[i]);	
								else
								if(res==0)
									swprintf(str,L"Error in %s",&File_list[i]);	
								else
								if(res==-1)
									swprintf(str,L"Skipped(wrong format) %s",&File_list[i]);	
								else
								if(res==-2)
									swprintf(str,L"Skipped(not xml) %s",&File_list[i]);	
							
		 
									SetWindowText(hEdit,TexBuffer);
									
									SendMessageW((hListSourceFiles), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)(LPCSTR)str);

 
								SendMessage( hProgressBarWnd2, PBM_SETPOS, (int)100*i/File_count, 0);
							  
							}
							ShowWindow(hProgressBarWnd, SW_HIDE);
							ShowWindow(hProgressBarWnd2, SW_HIDE);
					}
SendMessage(hWnd, WM_COMMAND,XML_CHECK_FINISHED,0); 
ExitThread(0);
  // return( 0 );   
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	GetUserNameA( UserName,&User_name_len);
		 
		{
		hThreadSend2=CreateThread( NULL, 0,Sender2, NULL, 0, &dwIdSend2 );	
		
		Sleep(10);
		hThreadSend=CreateThread( NULL, 0,SendThread, NULL, 0, &dwIdSend );

			
		}
	// Initialize global strings
//	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadString(hInstance, IDC_CHECH_MD5, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_CHECH_MD5);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

int FixXML(WCHAR* file) 
{
int status=1;
	int s_file_len=wcslen (sU);
	
	char file_[MAX_PATH*2];
 	memset (file_,0,MAX_PATH*2*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, &file[s_file_len+1], wcslen(&file[s_file_len+1]),file_,MAX_PATH*2, NULL, NULL);
 
//	char str[500];
 
	DWORD	dwFileLength,n_bytes4,dwSourceFileBufferLength;
	HANDLE hFile;

	unsigned int i=0;
	char* Buffer=0;

		hFile=CreateFileW( file, GENERIC_READ|GENERIC_WRITE,
		   FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,OPEN_EXISTING, 0, NULL);
		 if (hFile==INVALID_HANDLE_VALUE)
		 {
				MessageBoxW( hWnd, L" file not found", L"Error", MB_ICONWARNING);
				return(0);
		 } 

		dwFileLength=GetFileSize (hFile, NULL) ;
		dwSourceFileBufferLength=dwFileLength+dwFileLength*2;
		Buffer=0;
		
		Buffer=(char*)new char[dwSourceFileBufferLength];
	
		if(Buffer==0)
			{
				MessageBoxW(hWnd, L"Cannot allocate memory for buffer. Please close unused programs and try again.", L"Warning", MB_ICONWARNING);			
				return(0);
			}

		memset(Buffer,0x00, dwSourceFileBufferLength);
		ReadFile(hFile, (LPVOID)Buffer, dwFileLength, &n_bytes4, NULL);
		//CloseHandle(hFile);

	
//-------------------------------------

//char tags[MAX_TAGS][MAX_TAG_LEN];
//memset(tags,0, MAX_TAGS*MAX_TAG_LEN );
		
//char temp_tags [MAX_TAG_LEN];


char Image[]="<string";


//int line=1;
//int current_tag=0;
//int column=0;
//int quote;
	if(Buffer[0]==(char)0xFF && Buffer[1]==(char)0xFE)
	{
		status=-1;
		goto end;
	}

	//------check --------------------
	for(i=0;i<(int)(dwFileLength);i++)
	{
		if(Buffer[i]==(char)'<' && Buffer[i+1]==(char)'?'&& Buffer[i+2]==(char)'x'&& Buffer[i+3]==(char)'m')
		break;
		if(Buffer[i]==(char)'<' && Buffer[i+1]==(char)'r'&& Buffer[i+2]==(char)'e'&& Buffer[i+3]==(char)'s')
		break;
		if(Buffer[i]==(char)'<' && Buffer[i+1]==(char)'s'&& Buffer[i+2]==(char)'t'&& Buffer[i+3]==(char)'r')
		break;
	}

	if(i>=dwFileLength)
		{
			status=-2;
			goto end;
		}
	//--------------------------
	status=1;

		for(i=0;i<(int)(dwFileLength);i++)// 
		{
		
			if(Buffer[i]=='<' && Buffer[i+1]=='!' && Buffer[i+2]=='-') //<!--
				while(Buffer[i]!='-'||Buffer[i+1]!='-'||Buffer[i+2]!='>') i++; //-->
			
			if(Buffer[i]=='<' && Buffer[i+1]=='!' && Buffer[i+2]=='['&& Buffer[i+3]=='C'&& Buffer[i+4]=='D'&& Buffer[i+5]=='A') //<![CDATA[
				while(Buffer[i]!=']'||Buffer[i+1]!=']'||Buffer[i+2]!='>') i++; //]]>
				
			 
					 
			if(Buffer[i]==Image[0]&&Buffer[i+1]==Image[1])
				if(!memcmp(&Buffer[i],Image, strlen(Image)))
				{	
					while(Buffer[i]!='>') i++;
					i++;

					for(i ;i<(int)(dwFileLength);i++)// 
					{
						if(Buffer[i]=='<'&&Buffer[i+1]=='/'&&Buffer[i+2]=='s')
							break;

						if((   Buffer[i]==0x27 && Buffer[i-1]!='\\' )
							||(Buffer[i]=='"'  && Buffer[i-1]!='\\'))
						{
						
							for(unsigned int j=dwSourceFileBufferLength-1;j>i;j--)// 
							Buffer[j]=Buffer[j-1];
							Buffer[i]='\\';
						status++;
						}
					}
				
				}
					SendMessage( hProgressBarWnd, PBM_SETPOS, (int)100*i/dwFileLength, 0);
		}

		SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		WriteFile(hFile, (LPVOID)Buffer, strlen(Buffer), &n_bytes4, NULL);
		SetEndOfFile( hFile ); 
		CloseHandle(hFile);

	
end:
	delete(Buffer);
	return status;

}









int CleanXML(WCHAR* file)
{
int status=1;
	int s_file_len=wcslen (sU);
	
	char file_[MAX_PATH*2];
 	memset (file_,0,MAX_PATH*2*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, &file[s_file_len+1], wcslen(&file[s_file_len+1]),file_,MAX_PATH*2, NULL, NULL);
 
//	char str[500];
 
	DWORD	dwFileLength,n_bytes4,dwSourceFileBufferLength;
	HANDLE hFile;

	unsigned int i=0;
	char* Buffer=0;

		hFile=CreateFileW( file, GENERIC_READ|GENERIC_WRITE,
		   FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,OPEN_EXISTING, 0, NULL);
		 if (hFile==INVALID_HANDLE_VALUE)
		 {
				MessageBoxW( hWnd, L" file not found", L"Error", MB_ICONWARNING);
				return(0);
		 } 

		dwFileLength=GetFileSize (hFile, NULL) ;
		dwSourceFileBufferLength=dwFileLength+dwFileLength/2;
		Buffer=0;
		
		Buffer=(char*)new char[dwSourceFileBufferLength];
	
		if(Buffer==0)
			{
				MessageBoxW(hWnd, L"Cannot allocate memory for buffer. Please close unused programs and try again.", L"Warning", MB_ICONWARNING);			
				return(0);
			}

		memset(Buffer,0x00, dwSourceFileBufferLength);
		ReadFile(hFile, (LPVOID)Buffer, strlen(Buffer), &n_bytes4, NULL);
		//CloseHandle(hFile);

	
//-------------------------------------

//char tags[MAX_TAGS][MAX_TAG_LEN];
//memset(tags,0, MAX_TAGS*MAX_TAG_LEN );
		
//char temp_tags [MAX_TAG_LEN];


char Image[]="image=\"";


//int line=1;
//int current_tag=0;
//int column=0;
//int quote;
	if(Buffer[0]==(char)0xFF && Buffer[1]==(char)0xFE)
	{
		status=-1;
		goto end;
	}

	//------check --------------------
	for(i=0;i<(int)(dwFileLength);i++)
	{
		if(Buffer[i]==(char)'<' && Buffer[i+1]==(char)'?'&& Buffer[i+2]==(char)'x'&& Buffer[i+3]==(char)'m')
		break;
		if(Buffer[i]==(char)'<' && Buffer[i+1]==(char)'r'&& Buffer[i+2]==(char)'e'&& Buffer[i+3]==(char)'s')
		break;
		if(Buffer[i]==(char)'<' && Buffer[i+1]==(char)'s'&& Buffer[i+2]==(char)'t'&& Buffer[i+3]==(char)'r')
		break;
	}

	if(i>=dwFileLength)
		{
			status=-2;
			goto end;
		}
	//--------------------------
	status=1;

		for(i=0;i<(int)(dwFileLength);i++)// 
		{
		
			if(Buffer[i]=='<' && Buffer[i+1]=='!' && Buffer[i+2]=='-') //<!--
				while(Buffer[i]!='-'||Buffer[i+1]!='-'||Buffer[i+2]!='>') i++; //-->
			
			if(Buffer[i]=='<' && Buffer[i+1]=='!' && Buffer[i+2]=='['&& Buffer[i+3]=='C'&& Buffer[i+4]=='D'&& Buffer[i+5]=='A') //<![CDATA[
				while(Buffer[i]!=']'||Buffer[i+1]!=']'||Buffer[i+2]!='>') i++; //]]>
				
			 
					 
			if(Buffer[i]==Image[0]&&Buffer[i+1]==Image[1])
				if(!memcmp(&Buffer[i],Image, strlen(Image)))
				{
					int start=i;
					i+=strlen(Image);
					while(Buffer[i]!='"') i++;
					i++;
					if(Buffer[i]=='>') start--;

					for(int j=start;j<(int)(dwFileLength);j++)// 
						Buffer[j]=Buffer[j+i-start];
						status++;
				}

		
			SendMessage( hProgressBarWnd, PBM_SETPOS, (int)100*i/dwFileLength, 0);
		}
  
		SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		WriteFile(hFile, (LPVOID)Buffer, strlen(Buffer), &n_bytes4, NULL);
		SetEndOfFile( hFile ); 
		CloseHandle(hFile);

	

 
end:
	delete(Buffer);
	return status;
}



int ValidateXML(WCHAR* file)
{
int status=1;
	int s_file_len=wcslen (sU);
	
	char file_[MAX_PATH*2];
 	memset (file_,0,MAX_PATH*2*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, &file[s_file_len+1], wcslen(&file[s_file_len+1]),file_,MAX_PATH*2, NULL, NULL);
 
	char str[500];
 
	DWORD	dwFileLength,n_bytes4,dwSourceFileBufferLength;
	HANDLE hFile;

	unsigned int i=0;
	char* Buffer=0;

		hFile=CreateFileW( file, GENERIC_READ,
		   FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,OPEN_EXISTING, 0, NULL);
		 if (hFile==INVALID_HANDLE_VALUE)
		 {
				MessageBoxW( hWnd, L" file not found", L"Error", MB_ICONWARNING);
				return(0);
		 } 

		dwFileLength=GetFileSize (hFile, NULL) ;
		dwSourceFileBufferLength=dwFileLength+dwFileLength/2;
		Buffer=0;
		
		Buffer=(char*)new char[dwSourceFileBufferLength];
	
		if(Buffer==0)
			{
				MessageBoxW(hWnd, L"Cannot allocate memory for buffer. Please close unused programs and try again.", L"Warning", MB_ICONWARNING);			
				return(0);
			}

		memset(Buffer,0x00, dwSourceFileBufferLength);
		ReadFile(hFile, (LPVOID)Buffer, dwFileLength, &n_bytes4, NULL);
		CloseHandle(hFile);

	
//-------------------------------------

char tags[MAX_TAGS][MAX_TAG_LEN];
memset(tags,0, MAX_TAGS*MAX_TAG_LEN );
		
char temp_tags [MAX_TAG_LEN];
char Ruby[6]="Ruby ";
char Kenten[8]="Kenten " ;

char Underline[]="Underline=\"true\" Underline";


char AutoSizingType[]="AutoSizingType=\"HeightOnly\" AutoSizingReferencePoint=\"TopCenterPoint\"  AutoSizingType=\"HeightOnly\" AutoSizingReferencePoint=\"TopCenterPoint\"";

char FontStyle[]="FontStyle FontStyle";
int line=1;
int current_tag=0;
int column=0;
int quote;
int tag_place=0;

	if(Buffer[0]==(char)0xFF && Buffer[1]==(char)0xFE)
	{
		status=-1;
		goto end;
	}

	//------check --------------------
	for(i=0;i<(int)(dwFileLength);i++)
	{
		if(Buffer[i]==(char)'<' && Buffer[i+1]==(char)'?'&& Buffer[i+2]==(char)'x'&& Buffer[i+3]==(char)'m')
		break;
		if(Buffer[i]==(char)'<' && Buffer[i+1]==(char)'r'&& Buffer[i+2]==(char)'e'&& Buffer[i+3]==(char)'s')
		break;
		if(Buffer[i]==(char)'<' && Buffer[i+1]==(char)'s'&& Buffer[i+2]==(char)'t'&& Buffer[i+3]==(char)'r')
		break;
	}

	if(i>=dwFileLength)
		{
			status=-2;
			goto end;
		}
	//--------------------------

		for(i=1;i<(int)(dwFileLength);i++)// 
		{
			if(Buffer[i]==0xA) { line++; column=i+1;}
			if(Buffer[i]=='"') quote++;	

			if(Buffer[i]=='<' && Buffer[i+1]=='!' && Buffer[i+2]=='-') //<!--
				while(Buffer[i]!='-'||Buffer[i+1]!='-'||Buffer[i+2]!='>') i++; //-->
			
			if(Buffer[i]=='<' && Buffer[i+1]=='!' && Buffer[i+2]=='['&& Buffer[i+3]=='C'&& Buffer[i+4]=='D'&& Buffer[i+5]=='A') //<![CDATA[
				for(i;i<(int)(dwFileLength);i++)//
				{
				if(Buffer[i]==0xA) { line++; column=0;}
				if(Buffer[i]==']'&&Buffer[i+1]==']'&&Buffer[i+2]=='>')
					break; //]]>
				}
				
			 //if(Buffer[i]=='<'&&Buffer[i+1]=='s'&&Buffer[i+2]=='t'&&Buffer[i+3]=='r'&&Buffer[i+4]=='i'&&Buffer[i+5]=='n'&&Buffer[i+6]=='n')
			//		tag_place=1;

			if(Buffer[i]=='&')
							if(Buffer[i+1]!='l'&&Buffer[i+1]!='g'&&Buffer[i+1]!='a'&&Buffer[i+1]!='q'&&Buffer[i+1]!='n'&&Buffer[i+1]!='c')
							{
								tag_place=i;
							 
								wsprintf(str,"XML Parsing Error: '&' character is not encoded.\r\nLocation: %s\r\nLine: %d\r\n\r\n" ,file_,line);	
				 				goto next;  
							}
					 
			if(Buffer[i]=='<'&&Buffer[i+1]!='/'&&Buffer[i+1]!='?')
			{
				 quote=0; 
				for(int j=i;j<(int)(dwFileLength);j++) 
				{
					if(Buffer[j]==0xA){ line++; column=j+1;}
				    if(Buffer[j]=='"') quote++;

					
					
					if(Buffer[j]==' '||Buffer[j]==0xd||Buffer[j]==0x9||Buffer[j]=='>'||Buffer[j]=='/')
					{
						if(j-i<MAX_TAG_LEN-1)
						{
							memset(temp_tags,0,MAX_TAG_LEN);
							memcpy(&temp_tags[1],&Buffer[i],j-i);
							temp_tags[0]='<';
							temp_tags[1]='/';
							temp_tags[strlen(temp_tags)]='>';
						}	
						else
							MessageBoxW(hWnd, L"Tag lenght>MAX_TAG_LEN", L"Warning", MB_ICONWARNING);	

						for(j;j<(int)(dwFileLength);j++) 
						{
							
							
							if(Buffer[j]==Ruby[0]) // Ruby
							 if(!memcmp(&Buffer[j],Ruby,strlen(Ruby)))
							{
							  wsprintf(str,"XML Parsing Error: not well-formed(Ruby).\r\nLocation: %s\r\nLine: %d\r\n\r\n" ,file_,line);	
				 				 goto next; 
							 } 
							 
							 if(Buffer[j]==Kenten[0]) // Kenten
							 if(!memcmp(&Buffer[j],Kenten,strlen(Kenten)))
							{
							  wsprintf(str,"XML Parsing Error: not well-formed(Kenten).\r\nLocation: %s\r\nLine: %d\r\n\r\n" ,file_,line);	
				 				 goto next; 
							 } 
							  
							 if(Buffer[j]==Underline[0]) // Underline
							 if(!memcmp(&Buffer[j],Underline,strlen(Underline)))
							{
							  wsprintf(str,"XML Parsing Error: dublicated attribute(Underline).\r\nLocation: %s\r\nLine: %d\r\n\r\n" ,file_,line);	
				 				 goto next; 
							 } 

							 if(Buffer[j]==AutoSizingType[0]) // AutoSizingType
							 if(!memcmp(&Buffer[j],AutoSizingType,strlen(AutoSizingType)))
							{
							  wsprintf(str,"XML Parsing Error: dublicated attribute(AutoSizingType).\r\nLocation: %s\r\nLine: %d\r\n\r\n" ,file_,line);	
				 				 goto next; 
							 } 

							  if(Buffer[j]==FontStyle[0]) // FontStyle FontStyle
							 if(!memcmp(&Buffer[j],FontStyle,strlen(FontStyle)))
							{
							  wsprintf(str,"XML Parsing Error: dublicated attribute(FontStyle).\r\nLocation: %s\r\nLine: %d\r\n\r\n" ,file_,line);	
				 				 goto next; 
							 } 

							 
							 
					 
							if(Buffer[j]==0xA) { line++; column=j+1;}

							if(Buffer[j]=='"') quote++;
							
							if(Buffer[j]=='/'&&Buffer[j+1]=='>')
							{  break; }

							
							
							if(Buffer[j]=='>')
							{
								tag_place=j;
									
								if(quote) 
								  if(quote%2>0) 	
								  {	
									  wsprintf(str,"XML Parsing Error: not well-formed (quotes).\r\nLocation: %s\r\nLine: %d\r\n\r\n" ,file_,line);	
				 					goto next; 
								  }
				 
								if(current_tag<MAX_TAGS-1)
								{
									strcpy(tags[current_tag],temp_tags);
									current_tag++;
								}
								else
									MessageBoxW(hWnd, L"Tag count>MAX_TAGS", L"Warning", MB_ICONWARNING);
								break;
							}
								
							if(Buffer[j]=='<'&&Buffer[j+1]=='/')
								{
									wsprintf(str,"XML Parsing Error: not well-formed. \r\nLocation: %s\r\nLine: %d, Column: %d\r\n\r\n" , file_,line,j-column);	
									int str_len=strlen(str);

									if(TexBuffer_pos+str_len<TEXT_BUF_LEN)
									{
										strcpy(&TexBuffer[TexBuffer_pos],str);
										TexBuffer_pos+=str_len;
										//SetWindowText(hEdit,TexBuffer);
									}
									else
										MessageBoxW(hWnd, L"TexBuffer_pos<TEXT_BUF_LEN", L"Warning", MB_ICONWARNING);
									status=0;
									goto end;
								}

								
						}
				
						i=j; break;
					}
				}
			}

			if(Buffer[i]=='<'&&Buffer[i+1]=='/')
			{
				 int j;

						 for( j=i;j>1;j--) 
						 {	
							 if(Buffer[j]=='>'&&Buffer[j-1]=='"')
								 break;
							 if(Buffer[j]=='>'&&Buffer[j-1]=='/')
								 break;
							 if(j==tag_place)  break;

								 //check for AMAX team
								if(check_quote)
								if(	Buffer[j]==0x27&&Buffer[j-1]!='\\')
								{
									wsprintf(str,"XML Parsing Error: Apostrophe not preceded by \\. \r\nLocation: %s\r\nLine: %d, Column: %d\r\n\r\n" , file_,line,j-column);	
									int str_len=strlen(str);

									if(TexBuffer_pos+str_len<TEXT_BUF_LEN)
									{
										strcpy(&TexBuffer[TexBuffer_pos],str);
										TexBuffer_pos+=str_len;
										//SetWindowText(hEdit,TexBuffer);
									}
									else
										MessageBoxW(hWnd, L"TexBuffer_pos<TEXT_BUF_LEN", L"Warning", MB_ICONWARNING);
									status=0;
									goto end;
								}//-------------------------------
								//check for AMAX team
								if(check_quote)
							 	if( Buffer[j]=='"'&&Buffer[j-1]!='\\')
								{
									 if( Buffer[j-1]=='>'||Buffer[j+1]=='<') continue;
									 if( Buffer[j+1]=='\\'&&Buffer[j+2]=='n' )  continue; //for arabic "\n\n"
									  if( Buffer[j-1]=='n'&&Buffer[j-2]=='\\' )  continue; //for arabic "\n\n"

									wsprintf(str,"XML Parsing Error: Quote not preceded by \\. \r\nLocation: %s\r\nLine: %d, Column: %d\r\n\r\n" , file_,line,j-column);	
									int str_len=strlen(str);

									if(TexBuffer_pos+str_len<TEXT_BUF_LEN)
									{
										strcpy(&TexBuffer[TexBuffer_pos],str);
										TexBuffer_pos+=str_len;
										//SetWindowText(hEdit,TexBuffer);
									}
									else
										MessageBoxW(hWnd, L"TexBuffer_pos<TEXT_BUF_LEN", L"Warning", MB_ICONWARNING);
									status=0;
									goto end;
								}//------------------------------- 
						 }
			
				 for( j=i;j<(int)(dwFileLength+dwFileLength/3);j++) 	
					if(Buffer[j]=='>'||Buffer[j]==0)
					{  
						if(j-i<MAX_TAG_LEN-1)
						{
							memset(temp_tags,0,MAX_TAG_LEN);
							memcpy(temp_tags,&Buffer[i],j-i+1 );
						}	
						else
						{	wsprintf(str,"XML Parsing Error: not well-formed.\r\nLocation: %s\r\nLine: %d\r\n\r\n" ,file_,line);	
							goto next;//MessageBoxW(hWnd, L"Tag lenght>MAX_TAG_LEN", L"Warning", MB_ICONWARNING);
						}
						
						if(!strcmp(tags[current_tag-1],temp_tags))//,j-i+1))
						{
							current_tag--;
						}
						else
						{
							current_tag--;
							wsprintf(str,"XML Parsing Error: mismatched tag. Expected: %s \r\nLocation: %s\r\nLine: %d, Column: %d\r\n\r\n" ,tags[current_tag],file_,line,j-column);	
next:
							int str_len=strlen(str);

							if(TexBuffer_pos+str_len<TEXT_BUF_LEN)
							{
								strcpy(&TexBuffer[TexBuffer_pos],str);
								TexBuffer_pos+=str_len;
								//SetWindowText(hEdit,TexBuffer);
							}
							else
								MessageBoxW(hWnd, L"TexBuffer_pos<TEXT_BUF_LEN", L"Warning", MB_ICONWARNING);
							
							status=0;
							goto end;
						}
						
						i=j; break;
					}

			}
			
			SendMessage( hProgressBarWnd, PBM_SETPOS, (int)100*i/dwFileLength, 0);
		}
  
	
	//	swprintf(str,L"OK %s",&file[len+1]);	
 	//else
	//	int len=wcslen(sU);
	//	swprintf(str,L"Wrong %s",&file[len+1]);	
		 
//	SendMessageW((hListSourceFiles), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)(LPCSTR)str);
		 //InvalidateRect( hListSourceFiles, NULL, TRUE ); 
end:
	delete(Buffer);
	return status;
}



char * md5_noerrors(char *data,int size)
{
 HCRYPTHASH hHash;
 HCRYPTPROV hProv;
 BYTE md5hash[16];
 DWORD md5hash_size,dwSize;
 static char str_hash[33];
 unsigned int i;
 
 ZeroMemory(str_hash,sizeof(str_hash));
 ZeroMemory(md5hash,sizeof(md5hash));
 CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
 CryptCreateHash(hProv,CALG_MD5,0,0,&hHash);
 CryptHashData(hHash,(const unsigned char *)data,size,0);
 dwSize=sizeof(md5hash_size);
 CryptGetHashParam(hHash,HP_HASHSIZE,(BYTE *)&md5hash_size,&dwSize,0);
 CryptGetHashParam(hHash,HP_HASHVAL,md5hash,&md5hash_size,0);
 CryptDestroyHash(hHash);
 CryptReleaseContext(hProv, 0);
 
 for(i = 0 ; i < md5hash_size ; i++)
  sprintf(str_hash+2*i,"%2.2X",md5hash[i]);
 
 return str_hash;
}


void Check_MD5(WCHAR* file)
{
#define MD5_BUFFER_SIZE 600
char Buffer2[MD5_BUFFER_SIZE];
DWORD n_md5_file_size;
HANDLE	hFile;
char* Buffer;
DWORD dwFileLength;
char str[100];


 	hFile=CreateFileW(file, GENERIC_READ,
		 FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,OPEN_EXISTING, 0, NULL);
		 if (hFile==INVALID_HANDLE_VALUE)
			{	 
				MessageBoxW(hWnd, L"Cannot open the File", L"Error", MB_ICONERROR);
				return ;
			}
	dwFileLength=GetFileSize (hFile, NULL) ;
	if(dwFileLength>MD5_BUFFER_SIZE)
	{	 
			MessageBoxW(hWnd, L"md5 File is too big", L"Error", MB_ICONERROR);
			return ;
	}

	memset(Buffer2,0x00, MD5_BUFFER_SIZE);	
	ReadFile(hFile, (LPVOID)Buffer2, 	dwFileLength, &n_md5_file_size, NULL);
	CloseHandle(hFile);

	char pdf_file[MAX_PATH];
	memset(pdf_file,0,MAX_PATH);
	char pdf_md5[MAX_PATH];
	memset(pdf_md5,0,MAX_PATH);
 

	for(int s=dwFileLength;s>0;s--)
	{
		if(Buffer2[s]=='.'&&Buffer2[s+1]=='p'&&Buffer2[s+2]=='d'&&Buffer2[s+3]=='f' )
		{
			int end=s+4;
			int d=0;
			while(s>0&&Buffer2[s]!='*')s--;
			int start=s+1;
			memcpy(pdf_file,&Buffer2[start],end-start);

			end=start-2;
			s=end;
			while(s>0&&Buffer2[s]!=0x0A)s--;
			start=s+1;
			memcpy(pdf_md5,&Buffer2[start],end-start);

		}	
	}
//--------to upper case----------
int len3=strlen(pdf_md5);
for(int e=0;e<len3;e++)
{
if(pdf_md5[e]=='a') pdf_md5[e]='A';
if(pdf_md5[e]=='b') pdf_md5[e]='B';
if(pdf_md5[e]=='c') pdf_md5[e]='C';
if(pdf_md5[e]=='d') pdf_md5[e]='D';
if(pdf_md5[e]=='e') pdf_md5[e]='E';
if(pdf_md5[e]=='f') pdf_md5[e]='F';
}
///----------------------
 
int len2=wcslen(file);
for(int a=0;a<len2;a++)
if((file[a]=='.'&& file[a+1]=='m' && file[a+2]=='d' && file[a+3]=='5')||
   (file[a]=='.'&& file[a+1]=='M' && file[a+2]=='D' && file[a+3]=='5'))
{
	file[a+1]='p';
	file[a+2]='d';
	file[a+3]='f';
}

 hFile=CreateFileW(file, GENERIC_READ,
		 FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,OPEN_EXISTING, 0, NULL);
		 if (hFile==INVALID_HANDLE_VALUE)
			{	 
				MessageBoxW(hWnd, L"Cannot open the File", L"Error", MB_ICONERROR);
				return ;
			}
	  dwFileLength=GetFileSize (hFile, NULL) ;
	  if( dwFileLength==0){
		  wsprintfA(str,"Error    %s",pdf_file);
		SendMessage((hListSourceFiles), LB_ADDSTRING,(WPARAM)(int)(0), (LPARAM)(LPCSTR)str);
	  }

	int	dwSourceBufferLength= dwFileLength+dwFileLength/2;// +128-dwFileLength%128;

	Buffer=(char*)new char[dwSourceBufferLength];
		
	if(Buffer==0)
		{
			CloseHandle(hFile);
			MessageBoxW(hWnd, L"Cannot allocate memory for buffer. Please close unused programs and try again.", L"Warning", MB_ICONWARNING);			
			return;
		}
	memset(Buffer,0x00, dwSourceBufferLength);	
	
	ReadFile(hFile, (LPVOID)Buffer, dwFileLength, &n_md5_file_size, NULL);
	CloseHandle(hFile);
					
char *p =md5_noerrors(Buffer,dwFileLength);

delete(Buffer);



 
if(pdf_md5[0])
{
	if(!memcmp(pdf_md5,p,strlen(pdf_md5)))
	{
		wsprintfA(str,"OK    %s",pdf_file);
		//MessageBoxA(0, str, "MD5 is OK", MB_OK);
			SendMessage((hListSourceFiles), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)(LPCSTR)str);
									
	}
	else
	{
		wsprintfA(str,"Wrong %s",pdf_file);	
		//MessageBoxA(0, str, "Wrong MD5", MB_ICONWARNING);
		SendMessage((hListSourceFiles), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)(LPCSTR)str);
									
	}
}
else
MessageBoxA(0,"Something wrong with PDF filename", "Warning", MB_ICONWARNING);
			

}



int SelectFolder(WCHAR* dir)
{

//char SelFolder[MAX_PATH];	
LPITEMIDLIST lpIID;
BROWSEINFOW bi;
WCHAR szName[MAX_PATH];

ZeroMemory(&bi,sizeof(BROWSEINFO));
WCHAR Dir[MAX_PATH];

LPITEMIDLIST pidlRoot = NULL;
LPSHELLFOLDER pDesktopFolder = NULL;
SHGetDesktopFolder(&pDesktopFolder);
	if ( pDesktopFolder != NULL )
	{
	//char szPath[MAX_PATH];
	OLECHAR olePath[MAX_PATH]; wcscpy(olePath,dir);
	ULONG chEaten;
	ULONG dwAttributes;
	HRESULT hr;

	//GetCurrentDirectory(MAX_PATH, szName);
	//GetWindowText(hDirectory,CurrentDirectory,MAX_PATH);
	//if(CurrentDirectory[3]==' ')CurrentDirectory[3]=0;
	//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "e:\\project", -1, olePath, MAX_PATH);

	// Convert the path to an ITEMIDLIST.
	hr = pDesktopFolder->ParseDisplayName(NULL,
	NULL,
	olePath,
	&chEaten,
	&pidlRoot,
	&dwAttributes);

	pDesktopFolder->Release();

	}

	bi.pidlRoot = pidlRoot;

	bi.hwndOwner = hWnd;
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_USENEWUI|  
		BIF_BROWSEINCLUDEURLS |BIF_NONEWFOLDERBUTTON|BIF_SHAREABLE   ;
	bi.lpszTitle = L"Select a Folder";
	bi.pszDisplayName = Dir;

	lpIID = SHBrowseForFolderW(&bi);
	if(lpIID)
	{
		SHGetPathFromIDListW (lpIID, szName );

		int len=wcslen(szName);
		if(szName[len-1]=='\\') 
								szName[len-1]=0;



		wcscpy(FindFileDirectory_, szName);
			wcscpy(dir, szName);
	//	strcpy(CurrentDirectory , szName);
		SetWindowTextW(hEditSourceDirecrory,szName);
		//SendMessageW( (HWND)hListSourceFiles,LB_RESETCONTENT,0,0);
		return 1;
	}
	return 0;

}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_CHECH_MD5);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= (LPCSTR)IDC_CHECH_MD5;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    

   hInst = hInstance; // Store instance handle in our global variable

hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      	(GetSystemMetrics(SM_CXSCREEN)-PROJECT_WINDOW_WIDTH)/2,
		(GetSystemMetrics(SM_CYSCREEN)-PROJECT_WINDOW_HIGHT)/2,
		PROJECT_WINDOW_WIDTH, PROJECT_WINDOW_HIGHT,  
			NULL,NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
void	GetFileCount()
	{
	File_count=0;
							wcscpy(FindZip,sU);
							wcscat(FindZip,L"\\");
							wcscat(FindZip,L"*.xml");
						//	Find_Directory3(FindZip);
						//	FindXML(FindZip);
						//	Zip_files=File_count;
							/*Md5_files
							File_count=0;
							wcscpy(FindZip,sU);
							wcscat(FindZip,L"\\");
							wcscat(FindZip,L"*.md5");
							Find_Directory3(FindZip);
							 Md5_files=File_count;*/
	}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent,d;
 	PAINTSTRUCT ps;
 	HDC hdc;

	switch (message) 
	{
		
		case WM_DROPFILES:
			{
				char SourceFile [MAX_PATH];
		 
			HDROP hdrop;
			hdrop=(struct HDROP__ *)wParam;

			 int uNumFiles = DragQueryFile ( hdrop, -1, NULL, 0 );

			  if(uNumFiles>1)
			  {
			 	MessageBox(hWnd, "Sorry, this function is performing in debug mode.\n Only one file is supported.", "Warning", MB_OK);
			  	DragFinish ( hdrop );
				break;
			  }

			  //close opened file
		
			for ( int uFile = 0; uFile < uNumFiles; uFile++ )
				{
				// Get the next filename from the HDROP info.
				if ( DragQueryFile ( hdrop, uFile, SourceFile, MAX_PATH ) > 0 )
					{
					// ***
					// Do whatever you want with the filename in szNextFile.
					// ***
					}
				}
			// Free up memory.
			DragFinish ( hdrop );
			
			

			//open file
			SetWindowText(hEditSourceDirecrory,SourceFile);
			GetWindowTextW(hEditSourceDirecrory,sU, sizeof(sU)-1 ); 
			//GetFileCount();	
			InvalidateRect( hWnd, NULL, TRUE );
			}			
				break;
		
		
		case WM_SIZE:
	
			GetClientRect(hWnd, &rt);
			SetWindowPos(hEditSourceDirecrory,NULL,10,				10, rt.right-200,24,   SWP_NOZORDER);  
			SetWindowPos(hBrowseFolderButton,NULL,rt.right-200+10+10 , 10, 80, 24,   SWP_NOZORDER); 
			SetWindowPos(hFindFilesButton_,    NULL, rt.right-100+10 , 10, 80, 24,    SWP_NOZORDER);
			SetWindowPos(hRemoveImageButton_,  NULL, 10+90 , 35, 80, 24,    SWP_NOZORDER);

			SetWindowPos(hFixXMLButton,  NULL,  10 , 35, 80, 24,    SWP_NOZORDER);

			
			 SetWindowPos(hProgressBarWnd,    NULL,10,				 58, rt.right-20, 8, SWP_NOZORDER); 
			  SetWindowPos(hProgressBarWnd2,    NULL,10,			rt.bottom-10, rt.right-20, 10, SWP_NOZORDER); 
			 SetWindowPos( hListSourceFiles,  NULL,10,				65, rt.right-20, rt.bottom*2/3-40, SWP_NOZORDER);
			  SetWindowPos( hEdit,			 NULL,10,				rt.bottom*2/3+20, rt.right-20, rt.bottom-rt.bottom*2/3-30, SWP_NOZORDER); 
			///SetWindowPos(hRenameButton,      NULL,rt.right-90  , rt.bottom-40, 80, 24, SWP_NOZORDER); 
			//SetWindowPos(hUndoRenameButton,  NULL,rt.right-190  , rt.bottom-40, 80, 24, SWP_NOZORDER); 
			//SetWindowPos(hRenameSameFile,    NULL,10 ,			rt.bottom-40, 180, 24, SWP_NOZORDER); 
		
			//SetWindowPos(hOpenCompareWindowButton, NULL,   	10, 35, 130, 24,   SWP_NOZORDER); 
			//SetWindowPos(hLoadXLSX, NULL,   150, 35, 130, 24,   SWP_NOZORDER); 
			break;
			
		case WM_CREATE:	
			//----------------------------get path------------------------------------------	
			DragAcceptFiles(hWnd,1);
			
			GetModuleFileName(hInst,ProgramPatch,sizeof(ProgramPatch));
						for( d=MAX_PATH;d>0;d--)
							if(ProgramPatch[d]==0x5c)
							{ProgramPatch[d+1]=0;
							break;}

			 SetCurrentDirectory(ProgramPatch);	
		
			//CreateDirectory("Temp", NULL);
			//	wsprintf(TempPath,"%sTemp",ProgramPatch);
			//----------------------------get path------------------------------------------
 

				hUIFont=CreateFontW(16,0,0,0,FW_MEDIUM,0,0,0,
						0,OUT_CHARACTER_PRECIS,CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,DEFAULT_PITCH,L"Arial Unicode MS");			
			memset(&sm,0,sizeof(SYSTEMTIME));
			GetLocalTime(&sm);
			if(sm.wYear>2017 )//&&sm.wMonth>9 &&sm.wDay>5)
				{	
					MessageBoxW(hWnd, L"The debug version has expired. Please update the tool.", L"Error", MB_ICONSTOP);
					break;
				}
			
			hEditSourceDirecrory = CreateWindowEx( WS_EX_TOPMOST     ,
									"EDIT", "",
									WS_VISIBLE | WS_CHILD | WS_BORDER  | ES_AUTOHSCROLL |WS_TABSTOP ,//|WS_CAPTION |  ES_RIGHT   ES_RIGHT            0x0002L
												10, 10, PROJECT_WINDOW_WIDTH-200,24,
												hWnd, (HMENU)FILE_DIR, hInst, NULL );
									SendMessage(hEditSourceDirecrory , WM_SETFONT, (WPARAM)hUIFont, FALSE);
			 //DragAcceptFiles(hEditSourceDirecrory,1);

							 	SetWindowText(	hEditSourceDirecrory,"\\");
								//	SetWindowText(	hEditSourceDirecrory,"c:\\xml\\xml");
									
			hBrowseFolderButton  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
								  L"BUTTON",     L"Browse"  ,       // button text
									WS_VISIBLE | WS_CHILD | WS_TABSTOP ,  // styles
									 PROJECT_WINDOW_WIDTH-200+10 , 10, 80, 24,      
										hWnd, (HMENU)SET_PATH_FOR_FOLDERS, hInst, NULL ); 
							SendMessage(hBrowseFolderButton, WM_SETFONT, (WPARAM)hUIFont, FALSE);
				

			hFindFilesButton_  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
					L"BUTTON",     L"Check",       // button text
						WS_VISIBLE | WS_CHILD |WS_TABSTOP ,  // styles
						 PROJECT_WINDOW_WIDTH-200+10+90, 10, 80, 24,     
						hWnd, (HMENU) CHECK_ZIP, hInst, NULL );
					SendMessage(hFindFilesButton_, WM_SETFONT, (WPARAM)hUIFont, FALSE);

			hRemoveImageButton_  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
					L"BUTTON",     L"Clean",       // button text
						WS_VISIBLE | WS_CHILD |WS_TABSTOP ,  // styles
						  90, 35, 80, 24,     
						hWnd, (HMENU) CLEAN_INFO, hInst, NULL );
					SendMessage(hRemoveImageButton_, WM_SETFONT, (WPARAM)hUIFont, FALSE);

			hFixXMLButton  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
					L"BUTTON",     L"Fix",       // button text
						WS_VISIBLE | WS_CHILD |WS_TABSTOP ,  // styles
						 0, 35, 80, 24,     
						hWnd, (HMENU) FIX_XML, hInst, NULL );
					SendMessage(hFixXMLButton, WM_SETFONT, (WPARAM)hUIFont, FALSE);

			hProgressBarWnd=CreateWindow( PROGRESS_CLASS        ,
							  "progress bar",    
							WS_VISIBLE | WS_CHILD  ,  // styles
							  10,  55, PROJECT_WINDOW_WIDTH-40, 6,      
							  hWnd,   NULL,  (HINSTANCE) GetWindowLong(hWnd,0), NULL); 


				 SendMessage( hProgressBarWnd, PBM_SETRANGE, 0, MAKELPARAM(0,100));
				 SendMessage( hProgressBarWnd, PBM_SETSTEP, 1, 0);
				 SendMessage( hProgressBarWnd, PBM_SETPOS, 0, 0);
				 ShowWindow(hProgressBarWnd, SW_HIDE);

			hProgressBarWnd2=CreateWindow( PROGRESS_CLASS        ,
							  "progress bar",    
							WS_VISIBLE | WS_CHILD  ,  // styles
							  10,  50, PROJECT_WINDOW_WIDTH-40, 6,      
							  hWnd,   NULL,  (HINSTANCE) GetWindowLong(hWnd,0), NULL); 


				 SendMessage( hProgressBarWnd2, PBM_SETRANGE, 0, MAKELPARAM(0,100));
				 SendMessage( hProgressBarWnd2, PBM_SETSTEP, 1, 0);
				 SendMessage( hProgressBarWnd2, PBM_SETPOS, 0, 0);
				 ShowWindow(hProgressBarWnd2, SW_HIDE);

			hListSourceFiles  = CreateWindowExW(0       ,
							L"LISTBOX",     L"",       // button text
							WS_CHILD | WS_BORDER |WS_VISIBLE |LBS_NOTIFY | ES_AUTOHSCROLL |ES_AUTOVSCROLL |ES_RIGHT| WS_VSCROLL,//|LBS_STANDARD,   // styles
							10,  60, PROJECT_WINDOW_WIDTH-40, 220,             // parent window
							hWnd, (HMENU)-1, hInst, NULL ); 
				 	SendMessage(hListSourceFiles , WM_SETFONT, (WPARAM)hUIFont, FALSE);

			SendMessageW((HWND)hListSourceFiles, LB_RESETCONTENT,0,0);

				hEdit  = CreateWindowExW(0       ,
							L"EDIT",     L"",       // button text
							WS_VISIBLE | WS_CHILD | WS_BORDER   |WS_TABSTOP |ES_MULTILINE|
							      ES_AUTOVSCROLL  |WS_VSCROLL  ,//|LBS_STANDARD,   // styles
							10,  PROJECT_WINDOW_HIGHT-300, PROJECT_WINDOW_WIDTH-40, 200,             // parent window
							hWnd, (HMENU)-1, hInst, NULL ); 
				 	SendMessage(hEdit, WM_SETFONT, (WPARAM)hUIFont, FALSE);
			//SendMessageW((HWND)hListSourceFiles, LB_SETCURSEL,0,0);

					InvalidateRect( hWnd, NULL, TRUE );
			 
			break;
	
	
	case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				 	case XML_CHECK_FINISHED:	

							hCheckXML=0;
					 
					break;

					case XML_CLEAN_FINISHED:
						hCleanXML=0;
							break;

						case FIX_XML_FINISHED:
							hFixXML=0;
							break;

					
					
					case SET_PATH_FOR_FOLDERS:
					
						GetWindowTextW(hEditSourceDirecrory,sU, sizeof(sU)-1 );

						if(SelectFolder(sU))
						{
						 Zip_files=0;
						 GetFileCount();
						}
						break;

				 case CHECK_ZIP:

					 /*GetWindowTextW(hEditSourceDirecrory,sU, sizeof(sU)-1 );
					 if(sU[1]>0)
						{
							 File_count=0; 
							memset(File_list,0,MAX_XML_FILES*MAX_PATH*2);
							SendMessageW((HWND)hListSourceFiles, LB_RESETCONTENT,0,0);
							
							wcscpy(FindZip,sU);
							wcscat(FindZip,L"\\");
							wcscat(FindZip,L"*.zip");
							Find_Directory3(FindZip);

							ShowWindow(hProgressBarWnd, SW_SHOWNORMAL);
							SendMessage( hProgressBarWnd, PBM_SETPOS,0,0);

							ShowWindow(hProgressBarWnd2, SW_SHOWNORMAL);
							SendMessage( hProgressBarWnd2, PBM_SETPOS,0,0);

							for(i=0;i<File_count;i++)
							{
								memset(FindZip,0,MAX_PATH*2);
								wcscpy(FindZip,sU);
								wcscat(FindZip,L"\\");
								wcscat(FindZip,File_list[i]);

								ValidateXML(FindZip);
 
								SendMessage( hProgressBarWnd2, PBM_SETPOS, (int)100*i/File_count, 0);
							  
							}
							ShowWindow(hProgressBarWnd, SW_HIDE);
							ShowWindow(hProgressBarWnd2, SW_HIDE);
	 
						}*/
					 	if(hCheckXML)
							TerminateThread(hCheckXML,0);
					 	hCheckXML=CreateThread( NULL, 0,ValidateXMLThread, NULL, 0, &dwCheckZip );
					break;

				case CLEAN_INFO:

						if(hCleanXML)
							TerminateThread(hCheckXML,0);
					 	hCleanXML=CreateThread( NULL, 0,CleanXMLThread, NULL, 0, &dwCleanZip );

						break;


				case FIX_XML:

						if(hFixXML)
							TerminateThread(hFixXML,0);
					 	hFixXML=CreateThread( NULL, 0,FixXMLThread, NULL, 0, &dwCleanZip );

						break;
				 
				 case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		 case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			 
			RECT rt;
			 GetClientRect(hWnd, &rt);
//			 char szOut[100];
			 SelectObject(hdc,hUIFont);
			 SetBkMode(hdc , 0); 

			 /*if(Pdf_files)
			 {
				 wsprintf(szOut,"PDF files: %d",Pdf_files );
				TextOut(hdc, 10,rt.bottom-30,szOut,strlen(szOut));
			 }
			if(	Md5_files)
			{ 
				wsprintf(szOut,"MD5 files: %d",Md5_files );
				TextOut(hdc, 100,rt.bottom-30,szOut,strlen(szOut));
			}*/

			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	HDC hdc;
		PAINTSTRUCT ps;
		HFONT hCurFont;
		HFONT hCurFont2;
		char ss[256];	int y;
		 WCHAR s[256];
	switch (message)
	{
			case WM_PAINT:
			                      
			hdc = BeginPaint(hDlg, &ps);

			 hCurFont=CreateFontW(15,0,0,0,FW_NORMAL,0,0,0,
				RUSSIAN_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY,VARIABLE_PITCH|FF_ROMAN,L"Arial");
			  hCurFont2=CreateFontW(15,0,0,0,FW_BOLD,0,0,0,
				RUSSIAN_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY,VARIABLE_PITCH|FF_ROMAN,L"Arial");
				
			SelectObject(hdc,hCurFont);

			SetBkMode(hdc, 0);
			y=0;			
				
			//swprintf(s ,L"%s",szTitleU);
			if((ver%100)<10)
				wsprintfA(ss ,"XML-Checker  Version %d.0%d%s  ", ver/100,ver%100,subver);
			else
				wsprintfA(ss ,"XML-Checker  Version %d.%d%s  ", ver/100,ver%100,subver);
		
			TextOutA(hdc, 80,20+y*20,ss ,strlen(ss  ));
			//y++;
				SelectObject(hdc,hCurFont);
			y++;	swprintf(s,L"Copyright © 2014 by Sasha P (sasha_p@asus.com)");TextOutW(hdc, 80,20+y*20,s ,wcslen(s  ));	
			//y++;	wsprintf(s,"E-mail: hinayana@126.com");TextOut(hdc, 80,20+y*20,s ,strlen(s  ));	
		
			//y++;	swprintf(s,L"Thanks Benjamin Bentmann (benjamin.bentmann@becyhome.de)");TextOutW(hdc, 80,20+y*20,s ,wcslen(s  ));	
		
				
		
			DeleteObject(hCurFont);
				DeleteObject(hCurFont2);

			EndPaint(hDlg, &ps);

			break;
			
			case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}



int Find_Directory3 (WCHAR* file)
{

int result=0;
//File_count=0;
HANDLE hFile;
WIN32_FIND_DATAW findFileData;
WCHAR s[512];
WCHAR s2[512];
//WCHAR Buffer2[20000];
int p=0;
/*
WCHAR value_dir[16];	
ZeroMemory(&value_dir,sizeof(value_dir));
int find_lang=SendMessageW((HWND)hListBoxSourceLang ,CB_GETCURSEL,0,0);
	
wcscpy(value_dir,AndroidLanguageFolder[find_lang]);
*/
//ZeroMemory(&Buffer2,sizeof(Buffer2));
ZeroMemory(&findFileData,sizeof(findFileData));

wcscpy(s2,file); 
for(int i=wcslen(s2);i>0;i--)
		if(s2[i]==0x5c)
		{
			s2[i+1]=0;
			break;
		}
wcscpy(s,s2);
//WCHAR *pFileName;
ZeroMemory(&findFileData,sizeof(findFileData));

//int len,len2=0;
hFile=0;
hFile=FindFirstFileW(file,&findFileData);


	if(hFile!=INVALID_HANDLE_VALUE)
	{
				
		if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE ||findFileData.dwFileAttributes&FILE_ATTRIBUTE_NORMAL )
		{
		/*	len=wcslen(findFileData.cFileName);
			wcscpy(s,s2);
			wcscat(s,findFileData.cFileName);

						int k;
						  for (  k=0;k<File_count;k++)
						  if(!wcscmp(File_list[k],s))
							break;

						  	if(s[0])
							{
								pFileName=wcsrchr(s,'\\');
								*pFileName++;					
							}
						

						  if(k==File_count)
						  */{  
							  wcscpy(File_list[File_count],findFileData.cFileName);
							  //wcscpy (File_listU[File_count],ChangeToUnicode(link_name));
							 File_count++;
						  }

		/*	len=wcslen(findFileData.cFileName);
					 		memcpy(&Buffer2[p], findFileData.cFileName,len*2); p+=len;
					 		Buffer2[p]=0x0d; p++;
					 		Buffer2[p]=0x0a; p++;*/
		}
		
		/*if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY&& findFileData.cFileName[0]!='.')  
			{
				int len=wcslen(findFileData.cFileName);
				for(int g=0;g<len;g++)
					if(findFileData.cFileName[g]==(WCHAR)' '||findFileData.cFileName[g]>=255||findFileData.cFileName[g]<=(WCHAR)'+')
					{
							wcscpy(s,s2);	
							wcscat(s,findFileData.cFileName);
							wcscat(s,L"\\");
							SendMessageW((HWND)hListSourceFiles , LB_ADDSTRING, (WPARAM)-1,(LPARAM)(LPCSTR)s);
							File_count++;
							break;
					}
					
					g=0;
			
					{
						wcscpy(s,s2);	
						wcscat(s,findFileData.cFileName);
						wcscat(s,L"\\");
						wcscat(s,L"*.*");
						Find_Directory3(s);
					}
			}*/
	

		while(FindNextFileW(hFile,&findFileData))
		{	
			if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE ||findFileData.dwFileAttributes&FILE_ATTRIBUTE_NORMAL )
			{
				/*len=wcslen(findFileData.cFileName);
				wcscpy(s,s2);
				wcscat(s,findFileData.cFileName);

				int k;
						  for (  k=0;k<File_count;k++)
						  if(!wcscmp(File_list[k],s))
							break;

						  if(s[0])
							{
								pFileName=wcsrchr(s,'\\');
								*pFileName++;					
							}

						  int len=wcslen(pFileName);

						if(pFileName[len-1]=='b')
						if(pFileName[len-2]=='d')
							continue;

						  if(k==File_count)
						 */ {  
							  wcscpy(File_list[File_count],findFileData.cFileName);
							 // SendMessageW((HWND)hListSourceFiles , LB_INSERTSTRING, (WPARAM)-1,(LPARAM)(LPCSTR)findFileData.cFileName);
							  //wcscpy (File_listU[File_count],ChangeToUnicode(link_name));
							 File_count++;
						  }

						 /* if(k==File_count)
						  {  
							  wcscpy(File_list[File_count],s);
							  //wcscpy (File_listU[File_count],ChangeToUnicode(link_name));
							 File_count++;
						  }*/
						  if(File_count>=MAX_XML_FILES)
						  {	
							  MessageBox(hWnd ,"Maximum files count","Error", MB_ICONERROR);
								return 0;
						  }

			/*	len=wcslen(findFileData.cFileName);
					 		memcpy(&Buffer2[p], findFileData.cFileName,len*2); p+=len;
					 		Buffer2[p]=0x0d; p++;
					 		Buffer2[p]=0x0a; p++;*/
			}
		
		}
	
	FindClose(hFile);

 
	}




				/*		char File[512];
						memset(File ,0,sizeof(File ));
						strcpy(File ,ProgramPatch);
						strcat(File ,"source_name.txt");
					 
						HANDLE hFile2=CreateFile(File , GENERIC_WRITE,
									FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,CREATE_ALWAYS, 0, NULL);
							if (hFile2==INVALID_HANDLE_VALUE)
							{ 
								MessageBox(hWnd ,"Cannot write file. Please check the filename or path","Error", MB_ICONERROR);
								return 0;
							}				
							else
							{
								SetFilePointer(hFile2, 0, NULL, FILE_BEGIN);			
								 if(!WriteFile(hFile2, (char*) Buffer2, wcslen( Buffer2)*2  , &n_bytes4, NULL))
								 { 
									 MessageBox(hWnd ,"Cannot write file. Please check the filename or path","Error", MB_ICONERROR);
									return 0;
								}

								CloseHandle(hFile2);								
							}*/

return result;
}



int FindXML(WCHAR* file)
{
 int item_count=0;
HANDLE hFile;
WIN32_FIND_DATAW findFileData;
WCHAR source_path[MAX_PATH];
//WCHAR target_path[MAX_PATH];
WCHAR folder[1024]=L"";
//WCHAR file[512];
 //WCHAR source_file[MAX_PATH];
 //WCHAR target_file[MAX_PATH];
ZeroMemory(&findFileData,sizeof(findFileData));

//wcscpy(file,file_); wcscat(file,L"\\*.*");
	int s_file_len=wcslen (sU);
wcscpy(folder,file); 

for(int i=wcslen(folder);i>0;i--)
		if(folder[i]==0x5c)
		{
			folder[i+1]=0;
			break;
		}
 
hFile=0;
static int changed=0,g;

hFile=FindFirstFileW(file,&findFileData);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
			//len=wcslen(findFileData.cFileName);	
			if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE ||findFileData.dwFileAttributes&FILE_ATTRIBUTE_NORMAL && findFileData.cFileName[0]!='.' )
			
			{
				 //wcscpy(source_path,&folder[s_file_len+1]);
				//	wcscat(source_path,findFileData.cFileName);

					//if(PDF_count<MAX_PDF)
					//{wcscpy(PDFfiles[PDF_count],source_path)	;	
					//PDF_count++;}*/

					 wcscpy(source_path,&folder[s_file_len+1]);
					wcscat(source_path,findFileData.cFileName);
					wcscpy(File_list[File_count],source_path);
							  //wcscpy (File_listU[File_count],ChangeToUnicode(link_name));
							 File_count++;
							 item_count++;
			}


		while(FindNextFileW(hFile,&findFileData))
		{	
			//len=wcslen(findFileData.cFileName);	
			if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE ||findFileData.dwFileAttributes&FILE_ATTRIBUTE_NORMAL &&findFileData.cFileName[0]!='.'  )
			{
				/*	wcscpy(source_path,folder);
					wcscat(source_path,findFileData.cFileName);

					if(PDF_count<MAX_PDF)
					{wcscpy(PDFfiles[PDF_count],source_path)	;	
					PDF_count++;}
					*/
						  wcscpy(source_path,&folder[s_file_len+1]);
					wcscat(source_path,findFileData.cFileName);
					wcscpy(File_list[File_count],source_path);
					//wcscpy(File_list[File_count],findFileData.cFileName);
							  //wcscpy (File_listU[File_count],ChangeToUnicode(link_name));
							 File_count++;
							 item_count++;

			}
		}
	
	FindClose(hFile);

	}

	//if(Find_in_subdirectory)
	{
	wcscpy(source_path,folder);
	wcscat(source_path,L"*.*");
	//Find_DirectoryForArh (s);
 Find_DirectoryForXML(source_path);
	}

return item_count;
}



int Find_DirectoryForXML(WCHAR* file)
{
int item_count=0;
HANDLE hFile;
WIN32_FIND_DATAW findFileData;
WCHAR source_path[MAX_PATH];
//WCHAR target_path[MAX_PATH];
WCHAR folder[512];
ZeroMemory(&findFileData,sizeof(findFileData));

//wcscpy(file,file_); wcscat(file,L"\\*.*");

wcscpy(folder,file); 

for(int i=wcslen(folder);i>0;i--)
		if(folder[i]==0x5c)
		{
			folder[i+1]=0;
			break;
		}

ZeroMemory(&findFileData,sizeof(findFileData));

hFile=0;
hFile=FindFirstFileW(file,&findFileData);

	if(hFile!=INVALID_HANDLE_VALUE)
	{
				
		if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY && findFileData.cFileName[0]!='.'&& findFileData.cFileName[0]!='$') 
		{
			 
					wcscpy(source_path,folder);
					wcscat(source_path,findFileData.cFileName);
					wcscat(source_path,L"\\");
					wcscat(source_path,L"*.xml");
					FindXML(source_path);
					item_count++;
		}
	 

		while(FindNextFileW(hFile,&findFileData))
		{	
			if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY&& findFileData.cFileName[0]!='.')  		
			{
					wcscpy(source_path,folder);
					wcscat(source_path,findFileData.cFileName);
					wcscat(source_path,L"\\");
					wcscat(source_path,L"*.xml");
					FindXML(source_path);
					item_count++;
			}
		}		
	
	FindClose(hFile);
	}

return item_count;
}

