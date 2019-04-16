all: bilshell
bilshell: bilshell.c
	gcc -g -o bilshell bilshell.c
clean:
	rm -rf *o bilshell


