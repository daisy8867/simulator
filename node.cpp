#include "node.h"

Node::Node(int gpu, int cpu) {
	atomic_init(&num_gpu, gpu);
	atomic_init(&num_cpu, cpu);
	atomic_init(&io_rate, 0);
	atomic_init(&io_bw, 0);
	atomic_init(&io_gpu, 0);
}
