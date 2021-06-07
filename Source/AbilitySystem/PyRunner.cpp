#include "PyRunner.h"

#include <string>

#include "Windows/MinWindows.h"
#include "IPythonScriptPlugin.h"
#include "PyFunction.h"

HANDLE s_hPipe	= NULL;
const int SCP_SIZE = 1024;
const int RET_SIZE = 1024*1024;
FString s_Scp = TEXT("");
TQueue<FString> s_RetList;

void initPipe();
void updatePipe();

void APyRunner::Tick(float DeltaTime)
{
	if (s_hPipe == NULL)
	{
		initPipe();
	}
	else
	{
		updatePipe();
	}

	UPyFunction::SetWorld(GetWorld());
}

DWORD WINAPI RunNamedPipe(VOID* pDatea)
{
	BOOL isSuccess = ::ConnectNamedPipe(s_hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
	while (true)
	{
		TCHAR CmdOrg[SCP_SIZE] = {0,};
		TCHAR Cmd[SCP_SIZE] = {0,};
		DWORD CmdSize = 0;
		isSuccess = ::ReadFile(s_hPipe, CmdOrg, SCP_SIZE*sizeof(TCHAR), &CmdSize, NULL);
		memcpy(Cmd, &CmdOrg[1], CmdSize - sizeof(TCHAR));
		DWORD lengthW = (DWORD)lstrlenW(Cmd) + 1;
		DWORD lengthA = lengthW*2;
		char* Scp = new char[lengthA*sizeof(char)];
		::WideCharToMultiByte(CP_ACP, 0, Cmd, lengthW, Scp, lengthA, NULL, NULL);
		s_Scp = FString(Scp);
		delete[] Scp;

		while (s_RetList.IsEmpty())
		{
			::Sleep(0.1f);
		}
		::Sleep(0.5f);
		while (s_RetList.IsEmpty() == false)
		{
			std::wstring wStr = *(*s_RetList.Peek());
			s_RetList.Pop();
			DWORD bytesWritten = 0;
			::WriteFile(s_hPipe, wStr.c_str(), wStr.length() * sizeof(TCHAR), &bytesWritten, NULL);
		}
		s_RetList.Empty();
	}
	::FlushFileBuffers(s_hPipe);
	::DisconnectNamedPipe(s_hPipe);
	::CloseHandle(s_hPipe);

	return 1;
}

void initPipe()
{
	s_hPipe = ::CreateNamedPipe(_T("\\\\.\\pipe\\simple_pipe"),
		PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,
		RET_SIZE, SCP_SIZE, 60*60*24, NULL);

	DWORD  dwThreadId;
	HANDLE hThread = CreateThread(NULL, 0, RunNamedPipe, NULL, 0, &dwThreadId);
}

void updatePipe()
{
	if (s_hPipe == NULL || s_Scp.Len() == 0)
	{
		return;
	}

	FPythonCommandEx InOutPythonCommand;
	InOutPythonCommand.Command = s_Scp;
	IPythonScriptPlugin::Get()->ExecPythonCommandEx(InOutPythonCommand);

	FString StrRet = TEXT("");
	bool IsFirst = true;
	for (auto Log : InOutPythonCommand.LogOutput)
	{
		if (IsFirst)
		{
			IsFirst = false;
		}
		else
		{
			StrRet += TEXT("|");
		}
		StrRet += Log.Output;
	}
	s_RetList.Enqueue(StrRet);

	s_Scp = TEXT("");
}