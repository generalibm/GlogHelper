#include <iostream>
#include <string>

#include <glog/logging.h>
#include <fstream>
#include <boost/shared_ptr.hpp>

#define LOGDIR "log"
#define MKDIR "mkdir -p " LOGDIR

class GLogHelper {
public:
    typedef std::shared_ptr<GLogHelper> ptr;

    GLogHelper(const std::string & program);
    ~GLogHelper();
};

static void SignalHandle() {
    std::string str = "core dump";
    std::ofstream of("glog_dump.log", std::ios::app);
    of << str;
    of.close();

    LOG(FATAL) << str;
}

GLogHelper::GLogHelper(const std::string & program) {
    system(MKDIR);

    google::InitGoogleLogging(program.c_str());
    google::SetStderrLogging(google::INFO);
    google::SetLogDestination(google::INFO, LOGDIR"/INFO_");
    google::SetLogDestination(google::WARNING, LOGDIR"/WARNING_");
    google::SetLogDestination(google::ERROR, LOGDIR"/ERROR_");
    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 100;
    FLAGS_stop_logging_if_full_disk = true;
    google::SetLogFilenameExtension("websocket_");
    google::InstallFailureSignalHandler();
    google::InstallFailureFunction(&SignalHandle);
}

GLogHelper::~GLogHelper() {
    google::ShutdownGoogleLogging();
}

void test_glog_encapsulation() {
    GLogHelper::ptr gh_ptr(new GLogHelper("test_encapsulation"));

    LOG(INFO) << "track program";
    LOG(WARNING) << "there is a warning";
    LOG(ERROR) << "error happened";
}

void test_glog_simple() {
    FLAGS_log_dir = "./log";

    google::InitGoogleLogging("test_glog");

    LOG(INFO) << "test info";
    LOG(WARNING) << "test warning";
    LOG(ERROR) << "test error";
    //LOG(FATAL) << "test fatal";

    google::ShutdownGoogleLogging();

    std::cout << __cplusplus << std::endl;

}

void coredump_func() {
    int * pi = new int;
    delete pi;

    pi = 0;

    /// coredump here
    //int j = *pi;
}

void test_coredump() {
    GLogHelper::ptr gh_ptr(new GLogHelper("coredump"));

    coredump_func();
}


int main() {
    std::cout << "Hello, World!" << std::endl;

   // test_glog_simple();

    //test_glog_encapsulation();
    test_coredump();

    return 0;
}