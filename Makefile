all: scheduling coffee sum

scheduling: scheduling.c
	gcc scheduling.c -o scheduling 


coffee: coffee.c
	gcc coffee.c -o  coffee -lrt -pthread

sum: sum.c
	gcc sum.c -o sum -pthread


clean:
	rm scheduling coffee sum 
