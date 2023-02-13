 /************************************************
  * Logger Class
  * Developed by Ricardo BM & Javier BA
  * Graphic Engine Development
  * ESAT - 2021/2022
  ***********************************************/

#ifndef __LOGGER_H__ 
#define __LOGGER_H__ 1

#include "loguru.hpp"

namespace AWK{  

  //! Logger Class.
  /*! A logger to print information warning and error for the engine */
  class Logger {

    public:
      //! enumErrorCode Enum.
      /*! Store ID of the default msg */
      enum enumErrorCode{
        kErrorType_OK = 0,                  /*!< Enum value OK, Everything OK. */  
        kErrorType_Window_Size_Error = 1,   /*!< Enum value WindowSizeError. There are problems with the screen size. */  
        kErrorType_Window_Init_Error = 2,   /*!< Enum value WindowInitError. There are problems with the initialization. */  
        kErrorType_Window_Create_Error = 3, /*!< Enum value WindowCreateError. There are problems with the creation. */  
        kErrorType_ErrorLoadingGLAD = 4,    /*!< Enum value ErrorLoadingGlad. Problem with the creation of Glad. */  
      };


      //! Initialization of the logger for its usage
      /*!
        \param argc. number of params received in the entry point.
        \param argv[]. The params received in the entry point.
      */
      void initLogger(int argc, char* argv[]);
      

     //! Print a message on the console
     /*!
       \param error_priority. Type of error.
       \param file. The file where the error is. Use __FILE__ to make it automatically.
       \param line. The line where the error is. Use __LINE__ to make it automatically.
       \param error_code. type of predefined error.
       \sa log()
     */
      void log(loguru::NamedVerbosity error_priority,
        const char* file, unsigned line, 
        enumErrorCode error_code);

      //! Print a message on the console
      /*!
        \param error_priority. Type of error.
        \param file. The file where the error is. Use __FILE__ to make it automatically.
        \param line. The line where the error is. Use __LINE__ to make it automatically.
        \param error_code. The message to print.
        \sa log()
      */
      void log(loguru::NamedVerbosity error_priority,
        const char* file, unsigned line,
        const char* error_code);


      //! Create a file with all the messages
      /*!
        \param path. Where the file is going to be created.
        \param mode. The mode you want to create the file, append or truncated
        \param verbosity. Types of messages you want to add 
      */
      void logToFile(const char* path, loguru::FileMode mode, 
                     loguru::Verbosity verbosity);

      Logger();
      ~Logger();

    private:
      char* error_msg[5]; /*!< Array of defaul messages. */
      
   
  };

};
#endif // !__LOGGER_H__