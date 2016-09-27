/**
	@file
	@copyright
		Copyright Bernd Amend and Michael Adam 2014-2016
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)
*/
#include <extension_system/filesystem.hpp>

#ifndef EXTENSION_SYSTEM_USE_STD_FILESYSTEM

#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

bool extension_system::filesystem::exists(const extension_system::filesystem::path &p)
{
	const std::string str = p.string();
	return access(str.c_str(), R_OK) == 0;
}


bool extension_system::filesystem::is_directory(const extension_system::filesystem::path &p)
{
	const std::string str = p.string();
	struct stat sb;
	return (stat(str.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode));
}


extension_system::filesystem::path extension_system::filesystem::canonical(const extension_system::filesystem::path &p)
{
#ifdef EXTENSION_SYSTEM_COMPILER_MINGW
	return p;
#else
	char buffer[PATH_MAX];
	const std::string path = p.string();
	const char *result = realpath(path.c_str(), buffer);
	if(result == nullptr)
		return p; // couldn't resolve symbolic link
	else
		return std::string(result);
#endif
}

void extension_system::filesystem::forEachFileInDirectory(const extension_system::filesystem::path &root, const std::function<void(const extension_system::filesystem::path &p)> &func, bool recursive)
{
	const std::function<void(const extension_system::filesystem::path &p)>
			handle_dir = [&func, &recursive, &handle_dir](const extension_system::filesystem::path &p) {

		const std::string path_string = p.string();
		DIR *dp = opendir (path_string.c_str());

		if (dp != nullptr)
		{
			dirent *ep = nullptr;
			while ((ep = readdir (dp))) {
				const std::string name = ep->d_name;
				const filesystem::path full_name = p / name;

				if(name=="." || name=="..")
					continue;

	#ifdef _DIRENT_HAVE_D_TYPE
				if(ep->d_type == DT_REG || ep->d_type == DT_LNK)
	#endif
					func(full_name);
	#ifdef _DIRENT_HAVE_D_TYPE
				else if(ep->d_type == DT_DIR && recursive)
	#endif
					handle_dir(full_name);
			}

			(void) closedir (dp);
		} else {
			// not a directory???
		}
	};


	handle_dir(root);
}

#endif
