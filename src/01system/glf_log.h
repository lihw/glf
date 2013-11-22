// -------------------------------------------------------------- 
// glf_log.h
// Logging
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_LOG_H
#define GLF_LOG_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

//
// The levels of the log
enum LogLevelEnum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,

    LOG_LEVEL_LOWEST = LOG_DEBUG,
    LOG_LEVEL_HIGHEST = LOG_ERROR,

    LOG_VERBOSITY_DEFAULT = LOG_LEVEL_LOWEST
};

enum LogOutputEnum
{
    LOG_OUTPUT_CONSOLE,
    LOG_OUTPUT_DEBUG,
    LOG_OUTPUT_FILE,

    LOG_OUTPUT_DEFUALT = LOG_OUTPUT_CONSOLE
};

void logSetOutput(LogOutputEnum output);
LogOutputEnum logGetOutput();
// Only allow log with level higher than given verbosity.
void logSetVerbosity(LogLevelEnum verbosity);
LogLevelEnum logGetVerbosity();

// Output the log in the formatted way
// \param level the level of this log message
// \param file at which file this log message is generated.
// \param line at which line of the file this log message is created.
// \param format the format specifier as the one in printf.
void log(LogLevelEnum level, const char* file, int line, const char* format, ...);

#define GLF_LOGERROR(fmt, ...) \
    do { glf::log(glf::LOG_ERROR, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#define GLF_LOGWARNING(fmt, ...) \
    do { glf::log(glf::LOG_WARNING, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#define GLF_LOGINFO(fmt, ...) \
    do { glf::log(glf::LOG_INFO, NULL, 0, fmt, __VA_ARGS__); } while (0)

#if defined DEBUG
# define GLF_LOGDEBUG(fmt, ...) \
     do { glf::log(glf::LOG_DEBUG, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
# define GLF_LOGDEBUGIF(cond, fmt, ...) \
     { if ((cond)) { glf::log(glf::LOG_DEBUG, __FILE__, __LINE__, fmt, __VA_ARGS__); } }
#else
#   define GLF_LOGDEBUG(fmt, ...) 
#   define GLF_LOGDEBUGIF(cond, fmt, ...) 
#endif

void logPlain(const char* format, ...);

GLF_NAMESPACE_END

#endif // !GLF_LOG_H
