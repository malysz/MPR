all: bucket

%: %.c
	$(CC) -O2 -o $@ $< -Wall -Wshadow -Wextra -std=c99 -fopenmp
