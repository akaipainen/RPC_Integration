#ifndef LOG_H
#define LOG_H

#include <TClass.h>
#include <TObjArray.h>
#include <TObject.h>
#include <TString.h>

using std::ostream;

class Log: public TObject {
public:
    // Log levels: TRACE, DEBUG, INFO, WARN, ERROR, FATAL
    enum LogLevel {TRACE = 0, ERROR, WARNING, INFO, DEBUG, MAX_TYPE};
    typedef void (*LogNotification)(LogLevel type, const char* message );

    // get root logger singleton instance
    static Log *GetLogger();

    // delete root logger singleton instance
    static void DeleteLogger(); 

    // NB: the following functions should not be static
    // NB: deprecated: logging configuration should be made through to a configuration file
    static void  EnableCoreDump(Bool_t enabled);
    static void  MakeCoreDump(const char *fout);	
    static void  EnableDebug(Bool_t enabled);
    static void  SetGlobalLogLevel(LogLevel type);
    static Int_t GetGlobalLogLevel();
    static void  SetGlobalDebugLevel(Int_t level);
    static Int_t GetGlobalDebugLevel();
    static void  SetModuleDebugLevel(const char* module, Int_t level);
    static void  ClearModuleDebugLevel(const char* module);
    static void  SetClassDebugLevel(const char* className, Int_t level);
    static void  ClearClassDebugLevel(const char* className);

    static void  SetStandardOutput();
    static void  SetStandardOutput(LogLevel type);
    static void  SetErrorOutput();
    static void  SetErrorOutput(LogLevel type);
    static void  SetFileOutput(const char* fileName);
    static void  SetFileOutput(LogLevel type, const char* fileName);
    static void  SetStreamOutput(ostream* stream);
    static void  SetStreamOutput(LogLevel type, ostream* stream);
    static void  SetLogNotification(LogNotification pCallBack);
    static void  SetLogNotification(LogLevel type, LogNotification pCallBack);
    static void  Flush();

    static void  SetHandleRootMessages(Bool_t on);

    static void  SetPrintType(Bool_t on);
    static void  SetPrintType(LogLevel type, Bool_t on);
    static void  SetPrintModule(Bool_t on);
    static void  SetPrintModule(LogLevel type, Bool_t on);
    static void  SetPrintScope(Bool_t on);
    static void  SetPrintScope(LogLevel type, Bool_t on);
    static void  SetPrintLocation(Bool_t on);
    static void  SetPrintLocation(LogLevel type, Bool_t on);

    static void  SetPrintRepetitions(Bool_t on);

    static void  WriteToFile(const char* name, Int_t option = 0);

    // the following public methods are used by the preprocessor macros 
    // and should not be called directly
    static Bool_t IsDebugEnabled() {return fgDebugEnabled;}
    static Int_t GetDebugLevel(const char* module, const char* className);
    static void  Message(UInt_t level, const char* message, 
                         const char* module, const char* className,
                         const char* function, const char* file, Int_t line);
    static void  Debug(UInt_t level, const char* message, 
                       const char* module, const char* className,
                       const char* function, const char* file, Int_t line);

    static Int_t RedirectStdoutTo(LogLevel type, UInt_t level, const char* module, 
                                  const char* className, const char* function,
                                  const char* file, Int_t line, Bool_t print);
    static Int_t RedirectStderrTo(LogLevel type, UInt_t level, const char* module, 
                                  const char* className, const char* function,
                                  const char* file, Int_t line, Bool_t print);
    static void  RestoreStdout(Int_t original);
    static void  RestoreStderr(Int_t original);

    static ostream& Stream(LogLevel type, UInt_t level,
                           const char* module, const char* className,
                           const char* function, const char* file, Int_t line);
    static void TestException(Int_t level=10); 
    private:

            // constructor is made private for implementing a singleton
            Log();
            virtual ~Log();

            // NOT IMPLEMENTED?
    Log(const Log& log);
    Log& operator = (const Log& log);

    void           ReadEnvSettings();

    static void    RootErrorHandler(Int_t level, Bool_t abort, 
                    const char* location, const char* message);

    void           CloseFile(Int_t type);
    FILE*          GetOutputStream(Int_t type);

    UInt_t         GetLogLevel(const char* module, const char* className) const;
    void           PrintMessage(UInt_t type, const char* message, 
                                const char* module, const char* className,
                                const char* function, 
                                const char* file, Int_t line);

    void           PrintString(Int_t type, FILE* stream, const char* format, ...);
    void           PrintRepetitions();

    Int_t          RedirectTo(FILE* stream, LogLevel type, UInt_t level,
                                const char* module, const char* className,
                                const char* function,
                                const char* file, Int_t line, Bool_t print);

    ostream&       GetStream(LogLevel type, UInt_t level,
                            const char* module, const char* className,
                            const char* function, const char* file, Int_t line);

    enum {DEBUGOffset = DEBUG-1};

    static Log* fgInstance;                 //! pointer to current instance
    static Bool_t  fgDebugEnabled;             // flag for debug en-/disabling
    static Bool_t  fgCoreEnabled;             // flag for core dump en-/disabling

    UInt_t         fGlobalLogLevel;            // global logging level
    TObjArray      fModuleDebugLevels;         // debug levels for modules
    TObjArray      fClassDebugLevels;          // debug levels for classes

    Int_t          fOutputTypes[MAX_TYPE];     // types of output streams
    TString        fFileNames[MAX_TYPE];       // file names
    FILE*          fOutputFiles[MAX_TYPE];     //! log output files
    ostream*       fOutputStreams[MAX_TYPE];   //! log output streams

    Bool_t         fPrintType[MAX_TYPE];       // print type on/off
    Bool_t         fPrintModule[MAX_TYPE];     // print module on/off
    Bool_t         fPrintScope[MAX_TYPE];      // print scope/class name on/off
    Bool_t         fPrintLocation[MAX_TYPE];   // print file and line on/off

    Bool_t         fPrintRepetitions;          // print number of repetitions instead of repeated message on/off

    Int_t          fRepetitions;               //! counter of repetitions
    UInt_t         fLastType;                  //! type of last message
    TString        fLastMessage;               //! last message
    TString        fLastModule;                //! module name of last message
    TString        fLastClassName;             //! class name of last message
    TString        fLastFunction;              //! function name of last message
    TString        fLastFile;                  //! file name of last message
    Int_t          fLastLine;                  //! line number of last message
    LogNotification fCallBacks[MAX_TYPE];   //! external notification callback

    ClassDef(Log, 1)   // class for logging debug, info and error messages
};


// module name macro
#ifdef _MODULE_
# define MODULENAME() _MODULE_
#else
# define MODULENAME() "NoModule"
#endif

// function name macro
#if defined(__GNUC__) || defined(__ICC) || defined(__ECC) || defined(__APPLE__)
# define FUNCTIONNAME() __FUNCTION__
// #elif defined(__HP_aCC) || defined(__alpha) || defined(__DECCXX)
// #define FUNCTIONNAME() __FUNC__
#else
# define FUNCTIONNAME() "???"
#endif

// redirection
/** 
 * Redirect output to std::cout to specified log stream 
 * 
 * @param type      Type of stream to re-direct to
 * @param level     Level of output
 * @param scope     Scope
 * @param whatever  Any code that will output to std::cout 
 */
#define REDIRECTSTDOUT(type, level, scope, whatever) \
  do {Int_t originalStdout = Log::RedirectStdoutTo(type, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__, kFALSE); \
    whatever; Log::RestoreStdout(originalStdout);} while(false)
/** 
 * Redirect output to std::cerr to specified log stream 
 * 
 * @param type      Type of stream to re-direct to
 * @param level     Level of output
 * @param scope     Scope
 * @param whatever  Any code that will output to std::cout 
 */
#define REDIRECTSTDERR(type, level, scope, whatever) \
  do {Int_t originalStderr = Log::RedirectStderrTo(type, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__, kFALSE); \
    whatever; Log::RestoreStderr(originalStderr);} while(false)
/** 
 * Redirect output to std::cout and std::cerr to specified log stream 
 * 
 * @param type      Type of stream to re-direct to
 * @param level     Level of output
 * @param scope     Scope
 * @param whatever  Any code that will output to std::cout or std::cerr
 */
#define REDIRECTSTDOUTANDSTDERR(type, level, scope, whatever) \
  do {Int_t originalStdout = Log::RedirectStdoutTo(type, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__, kFALSE); \
    Int_t originalStderr = Log::RedirectStderrTo(type, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__, kFALSE); \
    whatever; Log::RestoreStderr(originalStderr); Log::RestoreStdout(originalStdout);} while(false)


// debug level
#ifdef LOG_NO_DEBUG
# define DebugLevel() -1
# define DebugLevelClass() -1
# define DebugLevelGeneral(scope) -1
#else
/** 
 * Get the object scope debug level
 */
# define DebugLevel() ((Log::IsDebugEnabled()) ? Log::GetDebugLevel(MODULENAME(), ClassName()) : -1)
/** 
 * Get the class (ROOT-enabled) debug level
 */
# define DebugLevelClass() ((Log::IsDebugEnabled()) ? Log::GetDebugLevel(MODULENAME(), Class()->GetName()) : -1)
/**
 * Get the debug level associated with scope 
 * @param scope Scope 
 */
# define DebugLevelGeneral(scope) ((Log::IsDebugEnabled()) ? Log::GetDebugLevel(MODULENAME(), scope) : -1)
#endif

// debug messages
#ifdef LOG_NO_DEBUG
# define Debug(level, message) do { } while (false)
# define DebugClass(level, message) do { } while (false)
# define DebugGeneral(scope, level, message) do { } while (false)
# define DebugF(level, message,...) do { } while (false)
# define DebugClassF(level, message,...) do { } while (false)
# define DebugGeneralF(scope, level, message,...) do { } while (false)
#else

// inspired by log4cxx code (see log4cxx/Logger.h)
// implements GCC branch prediction for increasing logging performance
# if !defined(RPCIA_UNLIKELY)
#  if defined(__GNUC__) && (__GNUC__ >= 3)
/**
 * Provides optimization hint to the compiler
 * to optimize for the expression being false.
 * @param expr boolean expression.
 * @returns value of expression.
 */
#   define RPCIA_UNLIKELY(expr) __builtin_expect(expr, 0)
#  else
/**
 * Provides optimization hint to the compiler
 * to optimize for the expression being false.
 * @param expr boolean expression.
 * @returns value of expression.
 */
#   define RPCIA_UNLIKELY(expr) expr
#  endif
# endif 

/**
 * 
 * Logs a message to a specified logger with the DEBUG level.
 * 
 * @param logLevel the debug level.
 * @param message message to print in the following format: Form(message).
 * Note, that message should contain balanced parenthesis, like 
 * <code>Debug(1, Form("Failed to decode line %d of %s", line, filename));</code> 
 */
# define Debug(logLevel, message) \
        do { if (RPCIA_UNLIKELY(Log::IsDebugEnabled() && Log::GetDebugLevel(MODULENAME(), ClassName()) >= logLevel)) {\
	  Log::Debug(logLevel, message, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
/**
 * 
 * Logs a message to a specified logger with the DEBUG level.  For use
 * in static member functions of a class 
 * 
 * @param logLevel the debug level.
 * @param message message to print in the following format: Form(message).
 * Note, that message should contain balanced parenthesis, like 
 * <code>Debug(1, Form("Failed to decode line %d of %s", line, filename));</code> 
 */
# define DebugClass(logLevel, message) \
	do { if (RPCIA_UNLIKELY(Log::IsDebugEnabled() && Log::GetDebugLevel(MODULENAME(), Class()->GetName()) >= logLevel)) {\
	  Log::Debug(logLevel, message, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)

/**
 * Logs a message to a specified logger with the DEBUG level.  For use
 * in non-ROOT-enabled-class scope.
 * 
 * @param scope the logging scope.
 * @param logLevel the debug level.
 * @param message message to print in the following format: Form(message).
 * Note, that message should contain balanced parenthesis, like 
 * <code>Debug(1, Form("Failed to decode line %d of %s", line, filename));</code> 
*/
# define DebugGeneral(scope, logLevel, message) \
	do { if (RPCIA_UNLIKELY(Log::IsDebugEnabled() && Log::GetDebugLevel(MODULENAME(), scope) >= logLevel)) {\
	  Log::Debug(logLevel, message, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
/** 
 * Macro to output debugging information.  This excepts a printf-like
 * format statement.   Note, at least 3 arguments (in total) must be
 * passed. 
 * 
 * @param logLevel Debug level
 * @param format   Printf-like format. 
 */
# define DebugF(logLevel,format,...) \
do { if (RPCIA_UNLIKELY(Log::IsDebugEnabled() && Log::GetDebugLevel(MODULENAME(), ClassName()) >= logLevel)) { \
    TString m;m.Form(format,__VA_ARGS__);					\
    Log::Debug(logLevel, m, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
/** 
 * Outut debug information, filtered on debug level.  For use in
 * static member function of a ROOT-enabled class. This excepts a
 * printf-like format statement.  Note, at least 3 arguments (in
 * total) must be passed.
 * 
 * @param logLevel Debug level
 * @param format   Printf-like format 
 * 
 * @return 
 */
# define DebugClassF(logLevel,format,...) \
  do { if (RPCIA_UNLIKELY(Log::IsDebugEnabled() && Log::GetDebugLevel(MODULENAME(), Class()->GetName()) >= logLevel)) { \
      TString m;m.Form(format,__VA_ARGS__);					\
      Log::Debug(logLevel, m, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
/** 
 * Outut debug information, filtered on debug level.  For use in
 * static member function of a non-ROOT-enabled class-scope. This
 * excepts a printf-like format statement.  Note, at least 3 arguments
 * (in total) must be passed.
 * 
 * @param scope    Scope 
 * @param logLevel Debug level
 * @param format   Printf-like format 
 * 
 * @return 
 */
# define DebugGeneralF(scope,logLevel,format,...) \
  do { if (RPCIA_UNLIKELY(Log::IsDebugEnabled() && Log::GetDebugLevel(MODULENAME(), scope) >= logLevel)) { \
      TString m;m.Form(format,__VA_ARGS__);					\
      Log::Debug(logLevel, m, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
    
#endif

// redirection to debug
#define StdoutToDebug(level, whatever) REDIRECTSTDOUT(Log::DEBUG, level, ClassName(), whatever)
#define StderrToDebug(level, whatever) REDIRECTSTDERR(Log::DEBUG, level, ClassName(), whatever)
#define ToDebug(level, whatever) REDIRECTSTDOUTANDSTDERR(Log::DEBUG, level, ClassName(), whatever)
#define StdoutToDebugClass(level, whatever) REDIRECTSTDOUT(Log::DEBUG, level, Class()->GetName(), whatever)
#define StderrToDebugClass(level, whatever) REDIRECTSTDERR(Log::DEBUG, level, Class()->GetName(), whatever)
#define ToDebugClass(level, whatever) REDIRECTSTDOUTANDSTDERR(Log::DEBUG, level, Class()->GetName(), whatever)
#define StdoutToDebugGeneral(scope, level, whatever) REDIRECTSTDOUT(Log::DEBUG, level, scope, whatever)
#define StderrToDebugGeneral(scope, level, whatever) REDIRECTSTDERR(Log::DEBUG, level, scope, whatever)
#define ToDebugGeneral(scope, level, whatever) REDIRECTSTDOUTANDSTDERR(Log::DEBUG, level, scope, whatever)

// debug stream objects
#define DebugStream(level) Log::Stream(Log::DEBUG, level, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define DebugClassStream(level) Log::Stream(Log::DEBUG, level, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define DebugGeneralStream(scope, level) Log::Stream(Log::DEBUG, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__)


/** 
 * Macro that will output stuff using the logging facilities. 
 * 
 * @param lvl     Message level 
 * @param message Message to show 
 */
#define Message(lvl,message) do { \
      Log::Message(lvl, message, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Macro that will output stuff using the logging facilities. 
 * For use in static member function of ROOT-enabled class-scope.
 *
 * @param lvl     Message level 
 * @param message Message to show 
 */
#define MessageClass(lvl,message) do { \
    Log::Message(lvl, message, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Macro that will output stuff using the logging facilities. 
 * For use in non-ROOT-enabled class-scope.
 *
 * @param scope   Scope 
 * @param lvl     Message level 
 * @param message Message to show 
 */
#define MessageGeneral(scope,lvl,message) do {			\
    Log::Message(lvl, message, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Print a message using the Log logging facility. This macro
 * accepts printf-like format arguments.  Note, at least 3 arguments
 * must be passed.  
 * @code
 *   MessageF(1, "foo");        // <-- Failes
 *   MessageF(1, "foo %d", 42); // <-- OK
 * @endcode
 *
 * @param lvl     Message level
 * @param format  printf-like format
 */
#define MessageF(lvl,format,...) do { \
  TString m; m.Form(format,__VA_ARGS__); \
  Log::Message(lvl, m, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Print a message using the Log logging facility. This macro
 * accepts printf-like format arguments.  Note, at least 3 arguments
 * must be passed.  
 * @code
 *   MessageF(1, "foo");        // <-- Failes
 *   MessageF(1, "foo %d", 42); // <-- OK
 * @endcode
 *
 * This is for static member function in for ROOT-enabled class-scope
 *
 * @param lvl     Message level
 * @param format  printf-like format
 */
#define MessageClassF(lvl,format,...) do { \
  TString m; m.Form(format,__VA_ARGS__); \
  Log::Message(lvl, m, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Print a message using the Log logging facility. This macro
 * accepts printf-like format arguments.  Note, at least 3 arguments
 * must be passed.  
 * @code
 *   MessageF(1, "foo");        // <-- Failes
 *   MessageF(1, "foo %d", 42); // <-- OK
 * @endcode
 *
 * This is for non-ROOT-enabled class-scope
 *
 * @param scope   Scope 
 * @param lvl     Message level
 * @param format  printf-like format
 */
#define MessageGeneralF(scope,lvl,format,...) do {	\
  TString m; m.Form(format,__VA_ARGS__); \
  Log::Message(lvl, m, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 

// info messages
#ifdef LOG_NO_INFO
# define Info(message) do { } while (false)
# define InfoClass(message) do { } while (false)
# define InfoGeneral(scope, message) do { } while (false)
# define InfoF(message,...) do { } while (false)
# define InfoClassF(message,...) do { } while (false)
# define InfoGeneralF(scope, message,...) do { } while (false)
#else
/**
 * Forwards to Message with log level of Log::INFO
 * @see Message 
 */
# define Info(message)               Message(Log::INFO, message)
/**
 * Forwards to MessageClass with log level of Log::INFO
 * @see MessageClass 
 */
# define InfoClass(message)          MessageClass(Log::INFO, message)
/**
 * Forwards to MessageGeneral with log level of Log::INFO
 * @see MessageGeneral
 */
# define InfoGeneral(scope, message) MessageGeneral(scope, Log::INFO, message)
/**
 * Forwards to MessageF with log level of Log::INFO
 * @see MessageF 
 */
# define InfoF(message,...)               MessageF(Log::INFO, message, __VA_ARGS__)
/**
 * Forwards to MessageClassF with log level of Log::INFO
 * @see MessageClassF 
 */
# define InfoClassF(message,...)          MessageClassF(Log::INFO, message, __VA_ARGS__)
/**
 * Forwards to MessageGeneralF with log level of Log::INFO
 * @see MessageGeneralF
 */
# define InfoGeneralF(scope,message,...)  MessageGeneralF(scope, Log::INFO, message, __VA_ARGS__)
#endif

// redirection to info
#define StdoutToInfo(whatever) REDIRECTSTDOUT(Log::INFO, 0, ClassName(), whatever)
#define StderrToInfo(whatever) REDIRECTSTDERR(Log::INFO, 0, ClassName(), whatever)
#define ToInfo(whatever) REDIRECTSTDOUTANDSTDERR(Log::INFO, 0, ClassName(), whatever)
#define StdoutToInfoClass(whatever) REDIRECTSTDOUT(Log::INFO, 0, Class()->GetName(), whatever)
#define StderrToInfoClass(whatever) REDIRECTSTDERR(Log::INFO, 0, Class()->GetName(), whatever)
#define ToInfoClass(whatever) REDIRECTSTDOUTANDSTDERR(Log::INFO, 0, Class()->GetName(), whatever)
#define StdoutToInfoGeneral(scope, whatever) REDIRECTSTDOUT(Log::INFO, 0, scope, whatever)
#define StderrToInfoGeneral(scope, whatever) REDIRECTSTDERR(Log::INFO, 0, scope, whatever)
#define ToInfoGeneral(scope, whatever) REDIRECTSTDOUTANDSTDERR(Log::INFO, 0, scope, whatever)

// info stream objects
#define InfoStream() Log::Stream(Log::INFO, 0, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define InfoClassStream() Log::Stream(Log::INFO, 0, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define InfoGeneralStream(scope) Log::Stream(Log::INFO, 0, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__)

// warning messages
#ifdef LOG_NO_WARNING
# define Warning(message) do { } while (false)
# define WarningClass(message) do { } while (false)
# define WarningGeneral(scope, message) do { } while (false)
# define WarningF(message,...) do { } while (false)
# define WarningClassF(message,...) do { } while (false)
# define WarningGeneralF(scope, message,...) do { } while (false)
#else
/**
 * Forwards to Message with log level of Log::WARNING
 * @see Message 
 */
# define Warning(message)               Message(Log::WARNING, message)
/**
 * Forwards to MessageClass with log level of Log::WARNING
 * @see MessageClass 
 */
# define WarningClass(message)          MessageClass(Log::WARNING, message)
/**
 * Forwards to MessageGeneral with log level of Log::WARNING
 * @see MessageGeneral
 */
# define WarningGeneral(scope, message) MessageGeneral(scope, Log::WARNING, message)
/**
 * Forwards to MessageF with log level of Log::WARNING
 * @see MessageF 
 */
# define WarningF(message,...)               MessageF(Log::WARNING, message, __VA_ARGS__)
/**
 * Forwards to MessageClassF with log level of Log::WARNING
 * @see MessageClassF 
 */
# define WarningClassF(message,...)          MessageClassF(Log::WARNING, message, __VA_ARGS__)
/**
 * Forwards to MessageGeneralF with log level of Log::WARNING
 * @see MessageGeneralF
 */
# define WarningGeneralF(scope,message,...)  MessageGeneralF(scope, Log::WARNING, message, __VA_ARGS__)
#endif

// redirection to warning
#define StdoutToWarning(whatever) REDIRECTSTDOUT(Log::WARNING, 0, ClassName(), whatever)
#define StderrToWarning(whatever) REDIRECTSTDERR(Log::WARNING, 0, ClassName(), whatever)
#define ToWarning(whatever) REDIRECTSTDOUTANDSTDERR(Log::WARNING, 0, ClassName(), whatever)
#define StdoutToWarningClass(whatever) REDIRECTSTDOUT(Log::WARNING, 0, Class()->GetName(), whatever)
#define StderrToWarningClass(whatever) REDIRECTSTDERR(Log::WARNING, 0, Class()->GetName(), whatever)
#define ToWarningClass(whatever) REDIRECTSTDOUTANDSTDERR(Log::WARNING, 0, Class()->GetName(), whatever)
#define StdoutToWarningGeneral(scope, whatever) REDIRECTSTDOUT(Log::WARNING, 0, scope, whatever)
#define StderrToWarningGeneral(scope, whatever) REDIRECTSTDERR(Log::WARNING, 0, scope, whatever)
#define ToWarningGeneral(scope, whatever) REDIRECTSTDOUTANDSTDERR(Log::WARNING, 0, scope, whatever)

// warning stream objects
#define WarningStream() Log::Stream(Log::WARNING, 0, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define WarningClassStream() Log::Stream(Log::WARNING, 0, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define WarningGeneralStream(scope) Log::Stream(Log::WARNING, 0, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__)


// error messages
/**
 * Forwards to Message with log level of Log::ERROR
 * @see Message 
 */
#define Error(message)               Message(Log::ERROR, message)
/**
 * Forwards to MessageClass with log level of Log::ERROR
 * @see MessageClass 
 */
#define ErrorClass(message)          MessageClass(Log::ERROR, message)
/**
 * Forwards to MessageGeneral with log level of Log::ERROR
 * @see MessageGeneral
 */
#define ErrorGeneral(scope, message) MessageGeneral(scope, Log::ERROR, message)
/**
 * Forwards to MessageF with log level of Log::ERROR
 * @see MessageF 
 */
#define ErrorF(message,...)               MessageF(Log::ERROR, message, __VA_ARGS__)
/**
 * Forwards to MessageClassF with log level of Log::ERROR
 * @see MessageClassF 
 */
#define ErrorClassF(message,...)          MessageClassF(Log::ERROR, message, __VA_ARGS__)
/**
 * Forwards to MessageGeneralF with log level of Log::ERROR
 * @see MessageGeneralF
 */
#define ErrorGeneralF(scope,message,...)  MessageGeneralF(scope, Log::ERROR, message, __VA_ARGS__)

// redirection to error
#define StdoutToError(whatever) REDIRECTSTDOUT(Log::ERROR, 0, ClassName(), whatever)
#define StderrToError(whatever) REDIRECTSTDERR(Log::ERROR, 0, ClassName(), whatever)
#define ToError(whatever) REDIRECTSTDOUTANDSTDERR(Log::ERROR, 0, ClassName(), whatever)
#define StdoutToErrorClass(whatever) REDIRECTSTDOUT(Log::ERROR, 0, Class()->GetName(), whatever)
#define StderrToErrorClass(whatever) REDIRECTSTDERR(Log::ERROR, 0, Class()->GetName(), whatever)
#define ToErrorClass(whatever) REDIRECTSTDOUTANDSTDERR(Log::ERROR, 0, Class()->GetName(), whatever)
#define StdoutToErrorGeneral(scope, whatever) REDIRECTSTDOUT(Log::ERROR, 0, scope, whatever)
#define StderrToErrorGeneral(scope, whatever) REDIRECTSTDERR(Log::ERROR, 0, scope, whatever)
#define ToErrorGeneral(scope, whatever) REDIRECTSTDOUTANDSTDERR(Log::ERROR, 0, scope, whatever)

// error stream objects
#define ErrorStream() Log::Stream(Log::ERROR, 0, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define ErrorClassStream() Log::Stream(Log::ERROR, 0, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define ErrorGeneralStream(scope) Log::Stream(Log::ERROR, 0, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__)


// fatal messages
/**
 * Forwards to Message with log level of Log::kFatal
 * @see Message 
 */
#define Fatal(message)               Message(Log::kFatal, message)
/**
 * Forwards to MessageClass with log level of Log::kFatal
 * @see MessageClass 
 */
#define FatalClass(message)          MessageClass(Log::kFatal, message)
/**
 * Forwards to MessageGeneral with log level of Log::kFatal
 * @see MessageGeneral
 */
#define FatalGeneral(scope, message) MessageGeneral(scope, Log::kFatal, message)
/**
 * Forwards to MessageF with log level of Log::kFatal
 * @see MessageF 
 */
#define FatalF(message,...)               MessageF(Log::kFatal, message, __VA_ARGS__)
/**
 * Forwards to MessageClassF with log level of Log::kFatal
 * @see MessageClassF 
 */
#define FatalClassF(message,...)          MessageClassF(Log::kFatal, message, __VA_ARGS__)
/**
 * Forwards to MessageGeneralF with log level of Log::kFatal
 * @see MessageGeneralF
 */
#define FatalGeneralF(scope,message,...)  MessageGeneralF(scope, Log::kFatal, message, __VA_ARGS__)

#endif