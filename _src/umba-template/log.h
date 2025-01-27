#pragma once

#include "umba/log.h"


// source parsing errors
// requires std::stringr curFile, unsigned lineNo in log scope
#define LOG_ERR_INPUT                UMBA_LOG_ERR_INPUT
#define LOG_WARN_INPUT(warnType)     UMBA_LOG_WARN_INPUT(warnType)
#define LOG_INFO_INPUT(infoType)     UMBA_LOG_INFO_INPUT(infoType)

// options and other errors
#define LOG_ERR                      UMBA_LOG_ERR
#define LOG_WARN(warnType)           UMBA_LOG_WARN(warnType)
#define LOG_INFO(infoType)           UMBA_LOG_INFO(infoType)

#define LOG_MSG                      UMBA_LOG_MSG
