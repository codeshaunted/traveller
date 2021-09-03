// averysumner - traveller
// logger.cc
// contains logger class definitions
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

#include "logger.hh"

#include <Windows.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstdarg>

#define TRAVELLER_LOG_MAX_LENGTH 256
#define TRAVELLER_LOG_TIMESTAMP_MAX_LENGTH 32
#define TRAVELLER_LOG_LEVEL_MAX_LENGTH 16

namespace traveller {

const std::string Logger::_log_level_strings[] = {
  "INFO",
  "WARNING",
  "ERROR",
  "DEBUG"
};

const std::string Logger::_log_level_colors[] = {
  "\u001b[37;1m",
  "\u001b[33;1m",
  "\u001b[31;1m",
  "\u001b[32;1m"
};

const std::string Logger::_output_directory = "logs";
const std::string Logger::_latest_log_name = "latest";
const std::string Logger::_previous_log_name = "previous";
const std::string Logger::_log_file_extension = "log";
const std::string Logger::_timestamp_format = "[%d-%m-%y %H:%M:%S] ";
const std::string Logger::_log_level_format = "[%s] ";
const std::string Logger::_content_format = "%s";

Logger::Logger() {
  // enable support for ANSI color escape sequences on windows terminal emulator
  HANDLE output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD console_mode = 0;
  GetConsoleMode(output_handle, &console_mode);
  console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(output_handle, console_mode);

  // create log directory if it doesn't exist
  if (!std::filesystem::exists(_output_directory)) std::filesystem::create_directory(_output_directory);

  // todo: maybe use some path joiner rather than regular string addition
  if (std::filesystem::exists(_output_directory + "/" + _latest_log_name + "." + _log_file_extension)) {
    std::fstream file_stream;
    std::string previous_log_data;

    // read previous log
    file_stream.open(_output_directory + "/" + _latest_log_name + "." + _log_file_extension);
    file_stream.seekg(0, std::ios::end);
    previous_log_data.reserve(file_stream.tellg());
    file_stream.seekg(0, std::ios::beg);
    previous_log_data.assign((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
    file_stream.close();

    // write to previous log file
    file_stream.open(_output_directory + "/" + _previous_log_name + "." + _log_file_extension, std::ios::out | std::ios::trunc);
    file_stream.write(previous_log_data.c_str(), previous_log_data.size());
    file_stream.close();
  }

  // redirect stdout to our log file
  freopen((_output_directory + "/" + _latest_log_name + "." + _log_file_extension).c_str(), "w", stdout);
}

void Logger::log(e_LogLevel __level, std::string __content) {
  auto current_time = std::chrono::system_clock::now();
  time_t time = std::chrono::system_clock::to_time_t(current_time);
  struct tm local_time;

  localtime_s(&local_time, &time); // thanks for always making it fun, Windows

  char time_string[TRAVELLER_LOG_TIMESTAMP_MAX_LENGTH];
  strftime(time_string, sizeof(time_string), _timestamp_format.c_str(), &local_time);

  char log_level_string[TRAVELLER_LOG_LEVEL_MAX_LENGTH];
  snprintf(log_level_string, sizeof(log_level_string), _log_level_format.c_str(), _log_level_strings[(uint8_t)__level].c_str());

  char content_string[TRAVELLER_LOG_MAX_LENGTH];
  snprintf(content_string, sizeof(content_string), _content_format.c_str(), __content.c_str());

  std::cout << time_string << log_level_string << content_string << std::endl; // for the log file
  std::clog << _log_level_colors[(uint8_t)__level]; // add color
  std::clog << time_string << log_level_string << content_string << std::endl; // for the console
  std::clog << "\033[m"; // add reset sequence so we don't paint the console
}

Logger* Logger::getInstance() {
  if (!instance) instance = new Logger();
  return instance;
}

void Logger::logFormat(e_LogLevel __level, const char* __content, ...) {
  char formatted_content[TRAVELLER_LOG_MAX_LENGTH];
  va_list arguments;
  va_start(arguments, __content);
  vsnprintf(formatted_content, TRAVELLER_LOG_MAX_LENGTH, __content, arguments);
  va_end(arguments);
  log(__level, formatted_content);
}

Logger* Logger::instance;

} // namespace traveller

#undef TRAVELLER_LOG_MAX_LENGTH
#undef TRAVELLER_LOG_TIMESTAMP_MAX_LENGTH
#undef TRAVELLER_LOG_LEVEL_MAX_LENGTH