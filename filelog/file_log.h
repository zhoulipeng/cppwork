#ifndef INCLUDE_FILE_LOG_H_
#define INCLUDE_FILE_LOG_H_

enum
{
	LOG_FATAL,
	LOG_ERROR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG,

};
enum
{
	LOG_WITHOUT_BUFFER,
	LOG_WITH_BUFFER,

};
extern char LEVEL[6][16];
extern int __log_out_priority;
extern FILE *__log_out_file;
/*
 * prefix_str 日志文件名前缀字符串， 比如 prefix_str = "nihao_log",生成的日志文件名为 nihao_log.0
 * log_file_num 日志文件个数， 超过这个数,日志自动覆盖， 覆盖规则
 * max_size 日志文件大小，单位字节， 约等于这么大，如果日志文件超出指定大小，则删除最老的日志（如果存在），备份比较新的日志(nihao_log.0 改名为 nihao_log.1)，最新的日志继续写到nihao_log.0
 * priority 日志最大打印级别 LOG_FATAL=0 LOG_ERROR=1 LOG_WARN=2 LOG_INFO=3 LOG_DEBUG=4
 * buffer_flag 写日志是否带缓冲， 1 带缓冲 （写日志效率高， 但是可能写入不及时，不方便调试，建议发布版本使用）0 无缓冲（写入及时， 方便崩溃调试，但是写日志效率慢）
 */
int Log_init(const char *prefix_str, unsigned int log_file_num, unsigned int max_size, int priority, int buffer_flag);

void Log_close();

inline void Log_write(int priority, const char* a_format,...);

// if(priority > __log_out_priority || __log_out_file == NULL) break; 把判断放到函数外面， 提高性能
#define LOG(level, format, ...) \
    do {if(level > __log_out_priority || __log_out_file == NULL) break;\
    	Log_write(level, "[%s|| @ , ] " format "", \
    			LEVEL[level], ##__VA_ARGS__ ); \
    }while(0)

#define LOG_LINE(level, format, ...) \
    do {if(level > __log_out_priority || __log_out_file == NULL) break;\
    	Log_write(level, "[%s||@%s,%d] " format "", \
    			LEVEL[level], __FILE__, __LINE__, ##__VA_ARGS__ ); \
    }while(0)
// 打印日志级别信息 + 时间 +    + 文件名 + 行数
#define LOG_TIME(level, format, ...) \
    do {if(level > __log_out_priority || __log_out_file == NULL) break;\
    time_t t;time(&t);struct tm tm_t; localtime_r(&t, &tm_t);\
    	Log_write(level, "[%s||%ld|@%s,%d] " format "", \
    			LEVEL[level], localtimetime(NULL), __FILE__, __LINE__, ##__VA_ARGS__ ); \
    }while(0)
// 打印日志级别信息 + 时间 +    + 文件名 + 行数
#define LOG_TFL(level, format, ...) \
    do {if(level > __log_out_priority || __log_out_file == NULL) break;\
    	Log_write(level, "[%s|%ld|@%s,%d] " format "", \
    			LEVEL[level], time(NULL), __FILE__, __LINE__, ##__VA_ARGS__ ); \
    }while(0)
// 打印日志级别信息 + 时间 + 函数 + 文件名 + 行数
#define LOG_ALL(level, format, ...) \
    do {if(level > __log_out_priority || __log_out_file == NULL) break;\
    	Log_write(level, "[%s|%ld|%s@%s,%d] " format "", \
    			LEVEL[level], time(NULL), __func__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    }while(0)

#endif /* INCLUDE_FILE_LOG_H_ */
