#ifndef __PICOLOG_BASE_INCLUDED_H__
#define __PICOLOG_BASE_INCLUDED_H__

#include <stdint.h>

#define MAX_DATA_LEN 2044

#define PICOLOG_EMERG 0
#define PICOLOG_ALERT 1
#define PICOLOG_CRIT 2
#define PICOLOG_ERROR 3
#define PICOLOG_WARN 4
#define PICOLOG_NOTICE 5
#define PICOLOG_INFO 6
#define PICOLOG_DEBUG 7

#define PICOLOG_EMERG_STR "[EMERG] "
#define PICOLOG_ALERT_STR "[ALERT] "
#define PICOLOG_CRIT_STR "[CRIT] "
#define PICOLOG_ERROR_STR "[ERROR] "
#define PICOLOG_WARN_STR "[WARNING] "
#define PICOLOG_NOTICE_STR "[NOTICE] "
#define PICOLOG_INFO_STR "[INFO] "
#define PICOLOG_DEBUG_STR "[DEBUG] "

#define PICOLOG_SOH "\001"
#define PICOLOG_EMERG_CODE PICOLOG_SOH "0"
#define PICOLOG_ALERT_CODE PICOLOG_SOH "1"
#define PICOLOG_CRIT_CODE PICOLOG_SOH "2"
#define PICOLOG_ERROR_CODE PICOLOG_SOH "3"
#define PICOLOG_WARN_CODE PICOLOG_SOH "4"
#define PICOLOG_NOTICE_CODE PICOLOG_SOH "5"
#define PICOLOG_INFO_CODE PICOLOG_SOH "6"
#define PICOLOG_DEBUG_CODE PICOLOG_SOH "7"

struct picolog_header {
	uint64_t timestamp_ms;
	uint16_t level;
	uint16_t log_id;
} __attribute__((packed, aligned(2)));

struct picolog_payload {
	uint16_t msg_id;
	uint8_t param_count;
	uint8_t reserved;
	uint8_t data[MAX_DATA_LEN];
} __attribute__((packed, aligned(2)));

struct picolog_packet {
	struct picolog_header header;
	struct picolog_payload payload;
} __attribute__((packed, aligned(2)));

struct picolog_register {
	uint16_t version;
	uint16_t level;
	uint16_t nlen;
	char name[58];
} __attribute__((packed, aligned(2)));

#endif // __PICOLOG_BASE_INCLUDED_H__
