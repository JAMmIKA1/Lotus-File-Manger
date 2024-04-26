#include "lotus.h"
#include <string.h>

void pcerror(const char *err) {
	// custom error message
	char buff[1000] = "\x1b[1;31m";
	strcat(buff, err);
	perror(buff);
	printf("\x1b[0m");
	sleep(ST);
}
int isDir(const char *path) {
	DIR *dir;
	dir = opendir(path);
	if (dir == NULL) {
		return 0;
	}
	closedir(dir);
	return 1;
}
void trim(char *input) {
	if (!input) {
		return;
	}
	long len = strlen(input), i, start = 0, end = 0;
	for (i = 0; i < len; i++) {
		if (!(input[i] == ' ' || input[i] == '\n')) {
			start = i;
			break;
		}
	}
	for (i = len - 1; i >= 0; i--) {
		if (!(input[i] == ' ' || input[i] == '\n' || (input[i] == '/' && i!=start))) {
			end = i + 1;
			break;
		}
	}
	for (i = 0; start + i < end; i++) {
		input[i] = input[start + i];
	}
	if (start > end) {
		*input = 0;
	} else {
		input[end - start] = 0;
	}
}
char *getFullPath(const char *path, const char *name) {
	// concatenate two pathes with respect of special pathes ~ . .. /
	char *home = getHome();
	char *fpath;
	if (!path) {
		fpath = (char *) malloc(sizeof(char) * 4096);
		strcpy(fpath, name);
	} else if (!strcmp(name, "..")) {
		int i = strlen(path) - 1;
		asprintf(&fpath, "%s", path);
		while (i--) {
			if (fpath[i] == '/') {
				fpath[i ? i : i + 1] = 0;
				break;
			}
		}
	} else if (!strcmp(name, ".")) {
		asprintf(&fpath, "%s", path);
    } else if (!strlen(name)) {
		asprintf(&fpath, "%s", home);
	} else if (name[0] == '~') {
		fpath = getFullPath(home, name + 2);
	} else if (name[0] == '/') {
		fpath = getFullPath(0, name);
	} else {
		asprintf(&fpath, "%s/%s", path, name);
	}
	return fpath;
}
char *getNextPath(const char *path) {
	char input[BUFFSIZE] = "";
	scanf("%[^\n]%*c", input);
    trim(input);
	return getFullPath(path, input);
}
char *getHome() {
	struct passwd *pw = getpwuid(getuid());
	return pw->pw_dir;
}
