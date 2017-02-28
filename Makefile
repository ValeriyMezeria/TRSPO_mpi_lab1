all:
	rm -rf build
	mkdir build
	mpicc lab1.c -o build/lab1

clean:
	rm -rf build

run:
	mpiexec -np 16 ./build/lab1
