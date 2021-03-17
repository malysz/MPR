#!/bin/sh

mpiexec -machinefile ./nodes1 -np 2 ./capacity.py std > wyniki/capacity1_std.txt
mpiexec -machinefile ./nodes1 -np 2 ./capacity.py sync > wyniki/capacity1_sync.txt
mpiexec -machinefile ./nodes2 -np 2 ./capacity.py std > wyniki/capacity2_std.txt
mpiexec -machinefile ./nodes2 -np 2 ./capacity.py sync > wyniki/capacity2_sync.txt
mpiexec -machinefile ./nodes3 -np 2 ./capacity.py std > wyniki/capacity3_std.txt
mpiexec -machinefile ./nodes3 -np 2 ./capacity.py sync > wyniki/capacity3_sync.txt

mpiexec -machinefile ./nodes1 -np 2 ./delay.py std > wyniki/delay1_std.txt
mpiexec -machinefile ./nodes1 -np 2 ./delay.py sync > wyniki/delay1_sync.txt
mpiexec -machinefile ./nodes2 -np 2 ./delay.py std > wyniki/delay2_std.txt
mpiexec -machinefile ./nodes2 -np 2 ./delay.py sync > wyniki/delay2_sync.txt
mpiexec -machinefile ./nodes3 -np 2 ./delay.py std > wyniki/delay3_std.txt
mpiexec -machinefile ./nodes3 -np 2 ./delay.py sync > wyniki/delay3_sync.txt



