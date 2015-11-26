#ifndef FIFO_H
#define FIFO_H
#include "node.h"
#include "app.h"
#include "model.h"
#include "launchContainer.h"
#include <boost/thread.hpp>
#include <boost/ref.hpp>
using namespace std;

void fifo(vector<App> apps, Node& n1, const vector<Model>& model);
#endif
