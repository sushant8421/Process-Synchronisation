all: coffee

coffee: coffee.c
	gcc coffee.c -o  coffee -lrt -pthread

clean:
	rm scheduling coffee sum 
