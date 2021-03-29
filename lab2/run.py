#!/usr/bin/env python2

import subprocess

def to_csv(measure):
	n_size = len(measure[1])

	rows = [",".join(["N"] + ["time"] * n_size)]
	for n, measurements in measure.items():
		rows.append(",".join([str(n)] + measurements))
	
	return "\n".join(rows)

for n in (6, 7, 8):
	for mode in ("CONST_SIZE", "SCALABLE"):
		file_name = "10**{}_{}.csv".format(n, mode)
		print file_name, "\n"

		measure = {}
		for p in range(1, 13):
			measure[p] = [
				subprocess.check_output(["mpiexec", "-np", str(p), "./pi.py", str(n), mode])[:-1] for _ in range(5)
			]
			print measure[p]

		with open(file_name, "w") as f:
			csv = to_csv(measure)
			print csv, "\n\n"
			f.write(to_csv(measure))
