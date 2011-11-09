#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <prio.h>
#include <debug.h>
#include <unistd.h>

#define dc_log(prio, msg) condlog(prio, "%s: iet prio: " msg, dev)
// 
// name: find_regex
// @param string: string you want to search into
// @param regex: the pattern used
// @return result: string finded in string with regex, "none" if none
char *find_regex (char * string, char * regex)
{
	int err;
	regex_t preg;
	err = regcomp (&preg, regex, REG_EXTENDED);
	
	if (err == 0) {
			int match;
			size_t nmatch = 0;
			regmatch_t *pmatch = NULL;
			nmatch = preg.re_nsub;
			pmatch = malloc (sizeof (*pmatch) * nmatch);
			
			if (pmatch) {	
				match = regexec (&preg, string, nmatch, pmatch, 0);
				regfree (&preg);
	
				if (match == 0) {
					char *result = NULL;
					int start = pmatch[0].rm_so;
					int end = pmatch[0].rm_eo;
					size_t size = end - start;
					result = malloc (sizeof (*result) * (size + 1));
					
					if (result) {
						strncpy (result, &string[start], size);
						result[size] = '\0';
						return result;
					}
				}
				else return "none";
			}	
		}
		return "none";	
}

// 
// name: inet_prio
// @param
// @return prio
int iet_prio (const char *dev, char * args)
{
	char preferredip_buff[255] = "";
	char * preferredip = &preferredip_buff[0];
    // Phase 1 : checks. If anyone fails, return prio 0.
    // check if args exists
	if (!args) {
		dc_log(0, "need prio_args with preferredip set");
		return 0;
    }
    // check if args format is OK
    if (sscanf(args, "preferredip=%s", preferredip) ==1) {}
	else {
		dc_log(0, "unexpected prio_args format");
		return 0;
	}
	// check if ip is not too short
	if (strlen(preferredip) <= 7) {
		dc_log(0, "prio args: preferredip too short");
		return 0;
	}
	// Phase 2 : find device in /dev/disk/by-path to match device/ip
    DIR           *dir_p;
    struct dirent *dir_entry_p;
    enum { BUFFERSIZE = 1024 };
    char buffer[BUFFERSIZE];
    char fullpath[BUFFERSIZE] = "/dev/disk/by-path/";
    dir_p = opendir(fullpath);
	
	// loop to find device in /dev/disk/by-path
    while( NULL != (dir_entry_p = readdir(dir_p))) {
		if (strncmp(dir_entry_p->d_name,".",1) != 0) {
			char path[BUFFERSIZE] = "/dev/disk/by-path/";
			strcat(path,dir_entry_p->d_name);
	        ssize_t nchars = readlink(path, buffer, sizeof(buffer)-1);
	        if (nchars != -1) {
				buffer[nchars]='\0';
				path[BUFFERSIZE]='\0';
				char *device;
				device = find_regex(buffer,"(sd[a-z]+)");
				// if device parsed is the right one
				if (strncmp(device,dev,strlen(device)) == 0) {
					char *ip;
					ip = find_regex(dir_entry_p->d_name,"([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})");
					// if prefferedip and ip fetched matches
					if (strncmp(ip,preferredip,strlen(ip)) == 0) {
						// high prio
						return 10;
					}
				}
			}
			else {
				printf("error\n");
				path[BUFFERSIZE]='\0';
			}
		}
    }
// nothing found, low prio 
return 20;
closedir(dir_p);
}

int getprio (struct path * pp, char * args)
{
        return iet_prio(pp->dev, args);
}
