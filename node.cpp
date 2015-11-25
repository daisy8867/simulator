#include "node.h"

Node::Node(int gpu, int cpu, int io) {
	atomic_init(&num_gpu, gpu);
	atomic_init(&num_cpu, cpu);
	atomic_init(&io_bw, io);
}
