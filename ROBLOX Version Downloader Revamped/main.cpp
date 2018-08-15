/*
ROBLOX Version Downloader Revamped, created by (Scripter) John.
Enjoy?
Possible arguments:
-hash [hash] (REQUIRED)
-year [year] (REQUIRED)
-hash is, well, the hash you're downloading
-year is the year of the hash you're downloading
*/

#include <fstream>
#include <Windows.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <WinInet.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include "unzip.h"
#pragma comment(lib, "wininet.lib")

int SaveFileFromInternet(const char* useragent, const char* url, const char* filename)
{
	int ret = 0;
	HINTERNET open = InternetOpenA(useragent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!open)
	{
		std::cout << "An error occurred when attempting to open an internet connection." << std::endl;
		InternetCloseHandle(open);
		return ret;
	}
	HINTERNET addr = InternetOpenUrlA(open, url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
	if (!addr)
	{
		std::cout << "An error occurred when attempting to access the URL." << std::endl;
		InternetCloseHandle(addr);
		InternetCloseHandle(open);
		return ret;
	}
	ret = 1;
	char data[1024];
	memset(&data, NULL, sizeof(data));
	DWORD bytesread = 0;
	std::fstream fs;
	fs.open(filename, std::ios::out | std::ios::binary);
	while (InternetReadFile(addr, data, sizeof(data), &bytesread) && bytesread)
	{
		if (std::string(data).find("AccessDenied") != std::string::npos) {
			break;
		}
		fs.write(data, bytesread);
		memset(&data, NULL, sizeof(data));
	}
	if (fs.peek() == std::fstream::traits_type::eof()) {
		ret = 2;
	}
	fs.flush();
	fs.close();
	InternetCloseHandle(addr);
	InternetCloseHandle(open);
	return ret;
}
bool file_exists(const char* file) {
	std::fstream fs;
	fs.open(file);
	bool yes = fs.good();
	fs.close();
	return yes;
}
int main(int argc, char* argv[]) {
	SetConsoleTitleA("RVDR-1.1 by John");
	if (argc == 5) {
		if (strcmp(argv[1], "-hash") == 0 && strcmp(argv[3], "-year") == 0) {
			std::string hash = std::string(argv[2]);
			int year = atoi(argv[4]);
			if (hash.find("version-") == std::string::npos) {
				hash.insert(0, "version-");
			}
			std::string base = "http://setup.roblox.com/";
			if (year == 2008) {
				std::cout << "Downloading.." << std::endl;
				std::vector<std::string> todownload = { "RobloxApp.zip", "Libraries.zip", "redist.zip", "content-music.zip", "content-fonts.zip", "content-textures.zip", "content-sounds.zip", "content-sky.zip" };
				for (std::string x : todownload) {
					SaveFileFromInternet("RVDR-1.1 by John", std::string(base + hash + "-" + x).c_str(), std::string(hash + "-" + x.c_str()).c_str());
				}
				std::cout << "Done! Unpacking.." << std::endl;
				CreateDirectory(hash.c_str(), 0);
				CreateDirectory(std::string(hash + "/content").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/music").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/fonts").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/textures").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/sounds").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/sky").c_str(), 0);
				for (std::string x : todownload) {
					HZIP zip = OpenZip(std::string(hash + "-" + x.c_str()).c_str(), 0);
					if (x.substr(0, 8) == "content-") {
						std::string part = x.substr(8);
						SetUnzipBaseDir(zip, std::string(hash + "/content/" + part.substr(0, part.find(".zip"))).c_str());
					}
					else {
						SetUnzipBaseDir(zip, hash.c_str());
					}
					ZIPENTRY zipentry;
					GetZipItem(zip, -1, &zipentry);
					int s = zipentry.index;
					for (int j = 0; j < s; j++) {
						GetZipItem(zip, j, &zipentry);
						UnzipItem(zip, j, zipentry.name);
					}
					CloseZip(zip);
				}
				std::fstream fs;
				fs.open(std::string(hash + "/AppSettings.xml"), std::ios::out);
				fs << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Settings>\n\t<ContentFolder>content</ContentFolder>\n\t<BaseUrl>http://www.roblox.com</BaseUrl>\n</Settings>";
				fs.flush();
				fs.close();
				for (std::string x : todownload) {
					std::remove(std::string(hash + "-" + x).c_str());
				}
				std::cout << "Done!" << std::endl;
			}
			else if (year == 2009 || year == 2010) {
				std::cout << "Downloading.." << std::endl;
				std::vector<std::string> todownload = { "RobloxApp.zip", "Libraries.zip", "redist.zip", "content-music.zip", "content-fonts.zip", "content-textures.zip", "content-textures2.zip", "content-particles.zip", "content-sounds.zip", "content-sky.zip", "content-materials.zip" };
				for (std::string x : todownload) {
					if (SaveFileFromInternet("RVDR-1.0 by John", std::string(base + hash + "-" + x).c_str(), std::string(hash + "-" + x.c_str()).c_str()) == 2) {
						std::remove(std::string(hash + "-" + x.c_str()).c_str());
					}
				}
				std::cout << "Done! Unpacking.." << std::endl;
				CreateDirectory(hash.c_str(), 0);
				CreateDirectory(std::string(hash + "/content").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/music").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/fonts").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/textures").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/sounds").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/sky").c_str(), 0);
				if (file_exists(std::string(hash + "-" + "content-particles.zip").c_str()) == true) {
					CreateDirectory(std::string(hash + "/content/particles").c_str(), 0);
				}
				if (file_exists(std::string(hash + "-" + "content-materials.zip").c_str()) == true) {
					CreateDirectory(std::string(hash + "/content/materials").c_str(), 0);
				}
				for (std::string x : todownload) {
					if (file_exists(std::string(hash + "-" + x).c_str()) == true) {
						HZIP zip = OpenZip(std::string(hash + "-" + x.c_str()).c_str(), 0);
						if (x.substr(0, 8) == "content-") {
							std::string part = x.substr(8);
							if (part.find("textures") != std::string::npos) {
								SetUnzipBaseDir(zip, std::string(hash + "/content/textures").c_str());
							}
							else {
								SetUnzipBaseDir(zip, std::string(hash + "/content/" + part.substr(0, part.find(".zip"))).c_str());
							}
						}
						else {
							SetUnzipBaseDir(zip, hash.c_str());
						}
						ZIPENTRY zipentry;
						GetZipItem(zip, -1, &zipentry);
						int s = zipentry.index;
						for (int j = 0; j < s; j++) {
							GetZipItem(zip, j, &zipentry);
							UnzipItem(zip, j, zipentry.name);
						}
						CloseZip(zip);
					}
				}
				std::fstream fs;
				fs.open(std::string(hash + "/AppSettings.xml"), std::ios::out);
				fs << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Settings>\n\t<ContentFolder>content</ContentFolder>\n\t<BaseUrl>http://www.roblox.com</BaseUrl>\n</Settings>";
				fs.flush();
				fs.close();
				for (std::string x : todownload) {
					std::remove(std::string(hash + "-" + x).c_str());
				}
				std::cout << "Done!" << std::endl;
			}
			else if (year == 2011 || year == 2012) {
				std::cout << "Downloading.." << std::endl;
				std::vector<std::string> todownload = { "RobloxApp.zip", "Libraries.zip", "redist.zip", "content-music.zip", "content-fonts.zip", "content-textures.zip", "content-textures2.zip", "content-textures3.zip", "BuiltInPlugins.zip", "shaders.zip", "content-particles.zip", "content-sounds.zip", "content-materials.zip", "content-sky.zip" };
				for (std::string x : todownload) {
					if (SaveFileFromInternet("RVDR-1.0 by John", std::string(base + hash + "-" + x).c_str(), std::string(hash + "-" + x.c_str()).c_str()) == 2) {
						std::remove(std::string(hash + "-" + x.c_str()).c_str());
					}
				}
				std::cout << "Done! Unpacking.." << std::endl;
				CreateDirectory(hash.c_str(), 0);
				CreateDirectory(std::string(hash + "/content").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/music").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/fonts").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/textures").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/sounds").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/sky").c_str(), 0);
				CreateDirectory(std::string(hash + "/content/particles").c_str(), 0);
				if (file_exists(std::string(hash + "-" + "BuiltInPlugins.zip").c_str()) == true) {
					CreateDirectory(std::string(hash + "/BuiltInPlugins").c_str(), 0);
				}
				if (file_exists(std::string(hash + "-" + "shaders.zip").c_str()) == true) {
					CreateDirectory(std::string(hash + "/shaders").c_str(), 0);
				}
				if (file_exists(std::string(hash + "-" + "content-materials.zip").c_str()) == true) {
					CreateDirectory(std::string(hash + "/content/materials").c_str(), 0);
				}
				for (std::string x : todownload) {
					if (file_exists(std::string(hash + "-" + x).c_str()) == true) {
						HZIP zip = OpenZip(std::string(hash + "-" + x.c_str()).c_str(), 0);
						if (x.substr(0, 8) == "content-") {
							std::string part = x.substr(8);
							if (part.find("textures") != std::string::npos) {
								SetUnzipBaseDir(zip, std::string(hash + "/content/textures").c_str());
							}
							else {
								SetUnzipBaseDir(zip, std::string(hash + "/content/" + part.substr(0, part.find(".zip"))).c_str());
							}
						}
						else if (x == "BuiltInPlugins.zip") {
							SetUnzipBaseDir(zip, std::string(hash + "/BuiltInPlugins").c_str());
						} 
						else if (x == "shaders.zip") {
							SetUnzipBaseDir(zip, std::string(hash + "/shaders").c_str());
						}
						else {
							SetUnzipBaseDir(zip, hash.c_str());
						}
						ZIPENTRY zipentry;
						GetZipItem(zip, -1, &zipentry);
						int s = zipentry.index;
						for (int j = 0; j < s; j++) {
							GetZipItem(zip, j, &zipentry);
							UnzipItem(zip, j, zipentry.name);
						}
						CloseZip(zip);
					}
				}
				std::fstream fs;
				fs.open(std::string(hash + "/AppSettings.xml"), std::ios::out);
				fs << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Settings>\n\t<ContentFolder>content</ContentFolder>\n\t<BaseUrl>http://www.roblox.com</BaseUrl>\n</Settings>";
				fs.flush();
				fs.close();
				for (std::string x : todownload) {
					std::remove(std::string(hash + "-" + x).c_str());
				}
				std::cout << "Done!" << std::endl;
			}
		}
	}
}
