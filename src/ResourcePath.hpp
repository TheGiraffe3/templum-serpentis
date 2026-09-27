/*
Copyright (c) 2026 by TheGiraffe3

Templum Serpentis is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Templum Serpentis is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <string>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <sys/param.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

// TODO: complete / confirm Windows and Linux support

inline std::string getResourcesPath() {
#ifdef __APPLE__
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	if (!mainBundle) return "";

	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	if (!resourcesURL) return "";

	char path[PATH_MAX];
	std::string result = "";

	if (CFURLGetFileSystemRepresentation(resourcesURL, true, reinterpret_cast<UInt8*>(path), PATH_MAX)) {
		result = std::string(path) + "/data/";
	}

	CFRelease(resourcesURL);
	return result;
#elif defined(_WIN32)
	char buffer[MAX_PATH];
	DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
	if (length == 0 || length >= MAX_PATH) {
		return "data\\";
	}

	std::string exePath(buffer);
	size_t lastSlash = exePath.find_last_of("\\/");
	if (lastSlash != std::string::npos) {
		return exePath.substr(0, lastSlash + 1) + "data\\";
	}

	return "data\\";
#else
	return "data/";
#endif
}
