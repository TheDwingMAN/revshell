
#include "httplib.h"

// HTTP
httplib::Client cli("http://cpp-httplib-server.yhirose.repl.co");



auto res = cli.Get("/hi");
