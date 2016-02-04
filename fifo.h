#ifndef FIFO_H
#define FIFO_H
#include "node.h"
#include "app.h"
#include "model.h"
#include "launchContainer.h"
#include <boost/thread.hpp>
#include <boost/ref.hpp>
extern atomic<int> io_mutex;
void fifo(vector<App>& apps, Node& n1, const vector<Model>& model);
void fair(set<App, function<bool (const App&, const App&)> >& apps, Node& n1, const vector<Model>& model);
void modelScheduler(vector<App>& apps, Node& n1, const vector<Model>& model);
void affinityScheduler(vector<App>& apps, Node& n1, const vector<Model>& model);
void dynamicScheduler(vector<App>& apps, Node& n1, const vector<Model>& model);
#endif
