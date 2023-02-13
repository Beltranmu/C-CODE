#include "logger.h"

namespace AWK {

  Logger::Logger(){}

  Logger::~Logger(){}

  void Logger::initLogger(int argc, char* argv[]){
    loguru::init(argc, argv);
   
    error_msg[0] = "Everything OK";
    error_msg[1] = "Size of the Window Invalid.\n make sure is a positive number.\n";
    error_msg[2] = "Uninitiated Window or issue at initiate it.\n";
    error_msg[3] = "Window is not create or badly create it\n";
    error_msg[4] = "Failed to initialize GLAD\n";
    //...
  }



  void Logger::log(loguru::NamedVerbosity error_priority,
    const char* file, unsigned line,
     enumErrorCode error_code){
    loguru::log(error_priority, file, line, error_msg[error_code]);
  }
  
  void Logger::log(loguru::NamedVerbosity error_priority,
    const char* file, unsigned line,
    const char* error_msg){
    loguru::log(error_priority, file, line, error_msg);
    
  }


  void Logger::logToFile(const char* path, loguru::FileMode mode,
    loguru::Verbosity verbosity) {
    loguru::add_file(path,mode, verbosity);
  }

  
}