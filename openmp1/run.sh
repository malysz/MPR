for points in 100000 1000000 10000000 100000000; do
	for schedule in 1 2 3 4 5; do
		rm results/$points-$schedule.csv
		./array $points 12 $schedule > results/$points-$schedule.csv
	done
done
