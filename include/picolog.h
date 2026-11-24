#ifndef __PICOLOG_H_INCLUDED__
#define __PICOLOG_H_INCLUDED__

#include "picolog_base.h"
#include <stdarg.h>

#ifndef PICOLOG_DEFAULT_LEVEL
#define PICOLOG_DEFAULT_LEVEL PICOLOG_INFO
#endif

#define __PICOLOG_DISPATCH(lvl, id, fmt, ...)                              \
	do {                                                               \
		if (lvl <= PICOLOG_DEFAULT_LEVEL) {                        \
			extern void _picolog_write(int, int, const char *, \
						   ...);                   \
			_picolog_write(lvl, id, fmt, ##__VA_ARGS__);       \
		}                                                          \
	} while (0)

#define picolog_alert(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_ALERT, id, fmt, ##__VA_ARGS__)
#define picolog_crit(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_CRIT, id, fmt, ##__VA_ARGS__)
#define picolog_erro(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_ERROR, id, fmt, ##__VA_ARGS__)
#define picolog_warn(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_WARN, id, fmt, ##__VA_ARGS__)
#define picolog_notice(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_NOTICE, id, fmt, ##__VA_ARGS__)
#define picolog_info(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_INFO, id, fmt, ##__VA_ARGS__)
#define picolog_debug(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_DEBUG, id, fmt, ##__VA_ARGS__)

#define picolog_alert_chk(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_ALERT, id, fmt, ##__VA_ARGS__)
#define picolog_crit_chk(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_CRIT, id, fmt, ##__VA_ARGS__)
#define picolog_erro_chk(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_ERROR, id, fmt, ##__VA_ARGS__)
#define picolog_warn_chk(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_WARN, id, fmt, ##__VA_ARGS__)
#define picolog_notice_chk(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_NOTICE, id, fmt, ##__VA_ARGS__)
#define picolog_info_chk(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_INFO, id, fmt, ##__VA_ARGS__)
#define picolog_debug_chk(id, fmt, ...) \
	__PICOLOG_DISPATCH(PICOLOG_DEBUG, id, fmt, ##__VA_ARGS__)

#undef __PICOLOG_DISPATCH

#endif // __PICOLOG_H_INCLUDED__
