
TARGET =matrix4000 serial parallel parallel_order mpi_mm mpi_order

all:$(TARGET)

matrix4000:
	python generate.py
serial:serial.c
	gcc -fopenmp -o serial serial.c
parallel:parallel.c
	gcc -fopenmp -o parallel parallel.c
parallel_order:parallel_order.c
	gcc -fopenmp -o parallel_order parallel_order.c
mpi_mm:mpi_mm.c
	mpicc -fopenmp -o mpi_mm mpi_mm.c
mpi_order:mpi_order.c
	mpicc -fopenmp -o mpi_order mpi_order.c
clean:
	rm -rf $(TARGET) 

