#!/usr/bin/env python

from mpi4py import MPI
import sys
import numpy
import time

def send_message_std():
	comm.Send(buff, dest=1)

def send_message_sync():
	comm.Ssend(buff, dest=1)

def send_message_buffered():
	comm.Bsend(buff, dest=1)

def receive_messages():
	status = MPI.Status()
	data = comm.Recv(buff, source=0, status=status)

def parse_input():
	if len(sys.argv) < 2:
		print 'Usage ./capacity <std|sync|buff> <num_of_messages>'
		sys.exit(1)

	if len(sys.argv) == 2:
		num_of_msg = 200
	else:
		num_of_msg = int(sys.argv[2])
	
	type = sys.argv[1]
	return (num_of_msg, type)

BITS_IN_BYTE = 8
NUMBER_OF_BITS_IN_MBYTE = 10**6

num_of_msg, type = parse_input()

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

results = []

for message_size in [2 ** x for x in xrange(3, 21)]:
	if type == 'buff':
		mpi_buffer = numpy.arange(20000000, dtype='int8')
		MPI.Attach_buffer(mpi_buffer)

	buff = numpy.arange(message_size/BITS_IN_BYTE, dtype='int8')

	comm.Barrier()

	transfered_data = 0
	if rank == 0:
		for _ in xrange(0, num_of_msg):
			if type == 'sync':
				send_message_sync()
			elif type == 'buff':
				send_message_buffered()
			else:
				send_message_std()
	elif rank == 1:
		start_time = MPI.Wtime()
		for _ in xrange(0, num_of_msg):
			receive_messages()
			transfered_data = transfered_data + message_size
		
		end_time = MPI.Wtime()
		result_time = end_time - start_time
		results.append((transfered_data / result_time) / NUMBER_OF_BITS_IN_MBYTE)
	else:
		print "Expected 2 nodes"
	
	if type == 'buff':
		MPI.Detach_buffer()

if rank == 1:
	for res in results:
		print res

MPI.Finalize()
