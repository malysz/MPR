#!/usr/bin/env python2

import random
import sys

from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

n = 10 ** int(sys.argv[1])
per_worker = n if "SCALABLE" in sys.argv else (n/size)

o = 0

comm.Barrier()

start = MPI.Wtime()

for i in xrange(per_worker):
	x, y = random.random(), random.random()
	if x**2 + y**2 <= 1.0:
		o += 1

sum = comm.reduce(o, op=MPI.SUM, root=0)
end = MPI.Wtime()

if rank == 0:
	print "{0:.10f}".format(end-start)
