/* $Id$ */
/* Copyright (c) 2011-2014 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS System VPN */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <System.h>
#include <System/App.h>
#include "../src/common.c"


/* libVPN */
/* private */
/* types */
typedef struct _VPNSocket
{
	int fd;
} VPNSocket;


/* constants */
#define PROGNAME	"libVPN"


/* variables */
static AppClient * _appclient = NULL;

static int _vpn_offset = 1024;

/* local functions */
static int (*old_connect)(int fd, const struct sockaddr * name,
		socklen_t namelen);


/* prototypes */
static void _libvpn_init(void);


/* functions */
static void _libvpn_init(void)
{
	static void * hdl = NULL;
	/* FIXME some symbols may be in libsocket.so instead */
	static char * libc[] = { "/lib/libc.so", "/lib/libc.so.6" };
	size_t i;
#ifdef RLIMIT_NOFILE
	struct rlimit r;
#endif

	if(hdl != NULL)
		return;
	for(i = 0; i < sizeof(libc) / sizeof(*libc); i++)
		if((hdl = dlopen(libc[i], RTLD_LAZY)) != NULL)
			break;
	if(hdl == NULL)
	{
		fprintf(stderr, "%s: %s\n", PROGNAME, dlerror());
		exit(1);
	}
	if((old_connect = dlsym(hdl, "connect")) == NULL)
	{
		fprintf(stderr, "%s: %s\n", PROGNAME, dlerror());
		dlclose(hdl);
		exit(1);
	}
	dlclose(hdl);
	if((_appclient = appclient_new(NULL, "VPN", NULL)) == NULL)
	{
		error_print(PROGNAME);
		exit(1);
	}
#ifdef RLIMIT_NOFILE
	if(getrlimit(RLIMIT_NOFILE, &r) == 0 && r.rlim_max > _vpn_offset)
		_vpn_offset = r.rlim_max;
# ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %u\n", __func__, _vpn_offset);
# endif
#endif
}


/* public */
/* interface */
/* connect */
int connect(int fd, const struct sockaddr * name, socklen_t namelen)
{
	int ret;

	_libvpn_init();
	if(fd < _vpn_offset)
		return old_connect(fd, name, namelen);
	if(appclient_call(_appclient, (void **)&ret, "connect",
				fd - _vpn_offset)
			!= 0)
		return -1;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: connect(%d) => %d\n", fd - VFS_OFFSET, ret);
#endif
	if(ret != 0)
		return _vpn_errno(_vpn_error, _vpn_error_cnt, -ret, 1);
	return ret;
}
