

all:
	$(CC) -o precheck precheck.c -ldl

clean:
	$(RM) ./precheck
