#ifndef QS_ERROR_H_
#define QS_ERROR_H_
typedef signed int qs_ret_code_t;
#define QS_SUCCESS                  ((qs_ret_code_t)   0x00)
#define QS_ERROR_INTERNAL           ((qs_ret_code_t)  -0x01)
#define QS_ERROR_INVALID_PARAM      ((qs_ret_code_t)  -0x02)
#define QS_ERROR_NO_RESOURCE        ((qs_ret_code_t)  -0x03)
#define QS_ERROR_NOT_FOUND          ((qs_ret_code_t)  -0x04)
#define QS_ERROR_BUSY               ((qs_ret_code_t)  -0x05)
#define QS_ERROR_INVALID_STATE      ((qs_ret_code_t)  -0x06)
#define QS_ERROR_NO_MEM             ((qs_ret_code_t)  -0x07)
#define QS_ERROR_INVALID_LENGTH     ((qs_ret_code_t)  -0x08)
#define QS_ERROR_NOT_IMPLEMENTED    ((qs_ret_code_t)  -0x09)
#define QS_ERROR_NOT_READY          ((qs_ret_code_t)  -0x0A)
#define QS_ERROR_TIMEOUT            ((qs_ret_code_t)  -0x0B)
#define QS_ERROR_COUNT_ZERO         ((qs_ret_code_t)  -0x0C)
#define QS_ERROR_SENSOR	    		((qs_ret_code_t)  -0x0D)
#define QS_ERROR_TEMPERATURE	    ((qs_ret_code_t)  -0x0E)
#define QS_ERROR_MEASUREMENT	    ((qs_ret_code_t)  -0x0F)
#define QS_ERROR_INVALID_FIRMWARE	((qs_ret_code_t)  -0x10)


#endif

