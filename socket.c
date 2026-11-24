#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#define _handle_err(msg) msg // perror_win(msg)
#define _handle_err_and_clean(msg) _handle_err(msg), WSACleanup()
#define _close_socket(socket) closesocket(socket), WSACleanup()
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define _handle_err(msg) perror(msg)
#define _handle_err_and_clean(msg) _handle_err(msg)
#define _close_socket(socket, status) status = close(socket)
#endif // _WIN32

#define SUN_MAX_PATH_LEN 108

typedef int socket_t;

typedef enum { TCP_SOCKET, UDP_SOCKET, UNIX_SOCKET, RAW_SOCKET } socket_type_t;

// TODO fix this. Not Thread Safe. Change for atomic
static int __listening = 1;

void sighand(int s)
{
	__listening = 0;
}

#ifdef _WIN32

#else
static void _set_sighandler(void)
{
	struct sigaction sa = { 0 };

	sa.sa_handler = sighand;

	sigaction(SIGINT, &sa, NULL);
}
#endif

static int _socket_info_local(const char *path)
{
	size_t _path_len = strlen(path);
	if ((_path_len) + 1 > SUN_MAX_PATH_LEN)
		return -1;

	return _path_len;
}

static int _socket_inet_info(struct sockaddr_in *sa, const char *addr,
			     unsigned short port)
{
	if (!sa)
		return -1;

	sa->sin_family = AF_INET;
	sa->sin_port = htons(port);
	sa->sin_addr.s_addr = inet_addr(addr);

	return 0;
}

static int _socket_close(socket_t sd)
{
#ifdef _WIN32
	if (closesocket(sd) != 0)
		goto err;

	WSACleanup();
#else
	if (close(sd) < 0)
		goto err;

#endif
	return 0;

err:
#ifdef _WIN32
	WSACleanup();
#endif

	return -1;
}

socket_t socket_create(socket_type_t type)
{
#ifdef __WIN32
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return -1;
	}
#endif // __WIN32

	int _sd, _domain, _type, _proto;

	switch (type) {
	case TCP_SOCKET:
		_domain = AF_INET;
		_type = SOCK_STREAM;
		_proto = IPPROTO_TCP;
		break;
	case UDP_SOCKET:
		_domain = AF_INET;
		_type = SOCK_DGRAM;
		_proto = IPPROTO_UDP;
		break;
	case UNIX_SOCKET:
		_domain = AF_UNIX;
		_type = SOCK_STREAM;
		_proto = 0;
		break;
	case RAW_SOCKET:
		// TODO: implement sock raw
		break;
	default:
		goto err;
	}

	if ((_sd = socket(_domain, _type, _proto)) == 0)
		goto err;

	return _sd;

err:
#ifdef _WIN32
	WSACleanup();
#endif
	return -1;
}

int socket_listen_local(socket_t sd, const char *path, int backlog)
{
	struct sockaddr_un _su;

	if (!path)
		goto err;

	size_t _path_len = strlen(path);
	if ((_path_len) + 1 > SUN_MAX_PATH_LEN)
		goto err;

	_su.sun_family = AF_UNIX;
	memcpy(_su.sun_path, path, _path_len + 1);

	if (bind(sd, (const struct sockaddr *)&_su,
		 sizeof(struct sockaddr_un)) < 0)
		goto err;

	if (listen(sd, backlog) < 0)
		goto err;

	return 0;

err:
#ifdef _WIN32
	WSACleanup();
#endif
	return -1;
}

int socket_listen_tcp(socket_t sd, const char *addr, unsigned short port,
		      int backlog)
{
	struct sockaddr_in _sa = { 0 };

	_sa.sin_family = AF_INET;
	_sa.sin_port = htons(port);
	_sa.sin_addr.s_addr = inet_addr(addr);

	if (bind(sd, (struct sockaddr *)&_sa, sizeof(struct sockaddr_in)) < 0)
		goto err;

	if (listen(sd, backlog) < 0)
		goto err;

	return 0;

err:
#ifdef _WIN32
	WSACleanup();
#endif
	return -1;
}

socket_t socket_accept(socket_t sd)
{
	socket_t _nsd;
	if ((_nsd = accept(sd, NULL, NULL) < 0)) {
#ifdef _WIN32
		WSACleanup();
#endif
		return -1;
	}

	return 0;
}

int socket_connect_tcp(socket_t sd, const char *addr, unsigned short port)
{
	assert(addr != NULL);

	struct sockaddr_in _dsa;
	_dsa.sin_family = AF_INET;
	_dsa.sin_port = htons(port);
	_dsa.sin_addr.s_addr = inet_addr(addr);

	if (connect(sd, (struct sockaddr *)&_dsa, sizeof(struct sockaddr)) < 0)
		goto err;

	return 0;

err:
#ifdef _WIN32
	WSACleanup();
#endif

	return -1;
}

int socket_bind_connect_tcp(socket_t sd, const char *saddr,
			    unsigned short sport, const char *daddr,
			    unsigned short dport)
{
	assert(saddr != NULL);
	assert(daddr != NULL);

	struct sockaddr_in _ssa;
	_ssa.sin_family = AF_INET;
	_ssa.sin_port = htons(sport);
	_ssa.sin_addr.s_addr = inet_addr(saddr);

	if (bind(sd, (struct sockaddr *)&_ssa, sizeof(struct sockaddr_in)) < 0)
		goto err;

	struct sockaddr_in _dsa;
	_dsa.sin_family = AF_INET;
	_dsa.sin_port = htons(dport);
	_dsa.sin_addr.s_addr = inet_addr(daddr);

	if (connect(sd, (struct sockaddr *)&_dsa, sizeof(struct sockaddr)) < 0)
		goto err;

	return 0;

err:
#ifdef _WIN32
	WSACleanup();
#endif

	return -1;
}

int socket_connect_local(socket_t sd, const char *path)
{
	assert(path != NULL);
	assert((strlen(path) + 1) > SUN_MAX_PATH_LEN);

	struct sockaddr_un _su = { .sun_family = AF_UNIX, .sun_path = *path };

	if (connect(sd, (struct sockaddr *)&_su, sizeof(struct sockaddr_un)) <
	    0) {
#ifdef _WIN32
		WSACleanup();
#endif
		return -1;
	}

	return 0;
}

#define MAX_SERVER_EVENTS 32
#define MAX_CLIENT_EVENTS 1024

struct event_sock {
	int fd;
	uint32_t ev;
};

struct event_batch {
	struct event_sock server[MAX_SERVER_EVENTS];
	struct event_sock client[MAX_CLIENT_EVENTS];
	int server_cnt;
	int client_cnt;
};

int socket_events(struct pollfd *fds, int n, int timeout, int server_sock,
		  struct event_batch *batch)
{
#ifdef _WIN32
	int _events = WSAPoll(fds, (unsigned long)n, timeout);
#else
	int _events = poll(fds, (nfds_t)n, timeout);
#endif

	batch->server_cnt = 0;
	batch->client_cnt = 0;

	if (_events <= 0)
		return _events;

	if (_events > 0) {
		for (int i = 0; i < n; ++i) {
			if (fds[i].revents) {
				int _is_server = (fds[i].fd == server_sock);

				if (__builtin_expect(_is_server, 0)) {
					int _s = batch->server_cnt;
					batch->server[_s].fd = fds[i].fd;
					batch->server[_s].ev = fds[i].revents;
					batch->server_cnt++;
				} else {
					int _c = batch->client_cnt;
					batch->client[_c].fd = fds[i].fd;
					batch->client[_c].ev = fds[i].revents;
					batch->client_cnt++;
				}
			}
		}
	}

	return _events;
}
