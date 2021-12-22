// averysumner - traveller
// include/traveller/logger.hh
// contains logger class declarations
// Copyright 2021 averysumner
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TRAVELLER_LOGGER_HH
#define TRAVELLER_LOGGER_HH

#include <string>

namespace traveller {

enum class e_LogLevel : uint8_t {
    INFO,
    WARNING,
    ERR, // if named ERROR it conflicts with a default windows #define; thank you microsoft
    DEBUG
};

// generic logger class
class Logger {
    public:
        Logger();
        static Logger* getInstance();
        void log(e_LogLevel __level, std::string __content);
        void logFormat(e_LogLevel __level, const char* __content, ...); // MSVC literally spits a C1001 if you don't use a c string
        static Logger* instance;
    protected:
        e_LogLevel _level;
    private:
        // todo: maybe make this configurable via config file
        static const std::string _log_level_strings[];
        static const std::string _log_level_colors[];
        static const std::string _output_directory;
        static const std::string _latest_log_name;
        static const std::string _previous_log_name;
        static const std::string _log_file_extension;
        static const std::string _timestamp_format;
        static const std::string _log_level_format;
        static const std::string _content_format;
};

// todo: maybe make this support more than 16 format arguments?
// macros for logging
#define TRAVELLER_LOG_EXPAND(X) X // MSVC's preprocessor is broken, so we need this
#define TRAVELLER_LOG_GET_MACRO(One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Eleven, Twelve, Thirteen, Fourteen, Fifteen, Sixteen, Seventeen, Name, ...) Name
#define TRAVELLER_LOG_INFO(...) TRAVELLER_LOG_EXPAND(TRAVELLER_LOG_GET_MACRO(__VA_ARGS__, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_FORMAT, TRAVELLER_LOG_INFO_STANDARD))(__VA_ARGS__)
#define TRAVELLER_LOG_INFO_STANDARD(Content) traveller::Logger::getInstance()->log(traveller::e_LogLevel::INFO, Content)
#define TRAVELLER_LOG_INFO_FORMAT(Content, ...) traveller::Logger::getInstance()->logFormat(traveller::e_LogLevel::INFO, Content, __VA_ARGS__)
#define TRAVELLER_LOG(...) TRAVELLER_LOG_INFO(__VA_ARGS__) // alias for TRAVELLER_LOG_INFO
#define TRAVELLER_LOG_WARNING(...) TRAVELLER_LOG_EXPAND(TRAVELLER_LOG_GET_MACRO(__VA_ARGS__, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_FORMAT, TRAVELLER_LOG_WARNING_STANDARD))(__VA_ARGS__)
#define TRAVELLER_LOG_WARNING_STANDARD(Content) traveller::Logger::getInstance()->log(traveller::e_LogLevel::WARNING, Content)
#define TRAVELLER_LOG_WARNING_FORMAT(Content, ...) traveller::Logger::getInstance()->logFormat(traveller::e_LogLevel::WARNING, Content, __VA_ARGS__)
#define TRAVELLER_LOG_ERROR(...) TRAVELLER_LOG_EXPAND(TRAVELLER_LOG_GET_MACRO(__VA_ARGS__, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_FORMAT, TRAVELLER_LOG_ERROR_STANDARD))(__VA_ARGS__)
#define TRAVELLER_LOG_ERROR_STANDARD(Content) traveller::Logger::getInstance()->log(traveller::e_LogLevel::ERR, Content)
#define TRAVELLER_LOG_ERROR_FORMAT(Content, ...) traveller::Logger::getInstance()->logFormat(traveller::e_LogLevel::ERR, Content, __VA_ARGS__)
#define TRAVELLER_LOG_DEBUG(...) TRAVELLER_LOG_EXPAND(TRAVELLER_LOG_GET_MACRO(__VA_ARGS__, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_FORMAT, TRAVELLER_LOG_DEBUG_STANDARD))(__VA_ARGS__)
#define TRAVELLER_LOG_DEBUG_STANDARD(Content) traveller::Logger::getInstance()->log(traveller::e_LogLevel::DEBUG, Content)
#define TRAVELLER_LOG_DEBUG_FORMAT(Content, ...) traveller::Logger::getInstance()->logFormat(traveller::e_LogLevel::DEBUG, Content, __VA_ARGS__)

} // namespace traveller

#endif // TRAVELLER_LOGGER_HH