#ifndef LAUNCH_CONTAINER_H
#define LAUNCH_CONTAINER_H
#include "node.h"
#include "app.h"
#include <boost/thread.hpp>
#include <boost/ref.hpp>
using namespace std;

const int io_cap = 80;
const int GPU = 0;
const int CPU = 1;
const int sleep_interval = 105;
void launch_container(App& a, Node& n, int res, string& logfile);
#endif
