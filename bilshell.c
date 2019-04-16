#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define MAX_COMMAND 100
#define MAX_PARAM_NUM 10
#define READ  0
#define WRITE 1
#define EXIT 1
int terminate = 0;

void consumer_command(char **params, int val);
void producer_command(char **params, int val);
void time_command(char **params, int val);

char *builtin_commands[] = {
  "cd",
  "exit"
};

char *bilshell_commands[]={
  	"consumer",
  	"producer",
	"time"
};

int builtins() {
  return sizeof(builtin_commands) / sizeof(char *);
}

int bil_builtins() {
  return sizeof(bilshell_commands) / sizeof(char *);
}
void consumer_command(char **params, int val)
{
	int M = atoi(params[1]);
	//printf("girdi");
	char buff[M];	
  	if (params[1] == NULL) {
    		fprintf(stderr, "Consumer command failed\n");
  	} else{
		read(0,buff,val);
  	}
	//int index = 0;
}

void producer_command(char **params , int val)
{
	int M = atoi(params[1]);
	char buff[M];
	//char* random = malloc( sizeof(char) * ( M + 1 ) );	
  	//printf("abc");
  	if (params[1] == NULL) {
    		fprintf(stderr, "Consumer command failed\n");
  	}
	else{
		//printf("abc");
		srandom(time(NULL));
		char alphanumerics[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
		for( int i = 0; i < M; ++i)
		{
			//srandom(time(NULL));

			buff[i] = alphanumerics[random() % (sizeof(alphanumerics) - 1)];
		}
		
	}
	int m_val = M;
	while( m_val > 0)
	{
		
		write(1, buff,val);
		m_val -= val;
	}
	//printf("\n");
	//return random;
}

void time_command(char **params, int val){

}
int cd_command(char **params)
{
  if (params[1] == NULL) {
    fprintf(stderr, "Expected argument to \"cd\"\n");
  } else {
    if (chdir(params[1]) != 0) {
      perror("bilshell error");
    }
  }
  return 1;
}

int exit_command(char **params)
{
  terminate = 1;
  return 0;
}

void (*bilshell_func[]) (char ** , int) = {
  &consumer_command,
  &producer_command,
  &time_command
};

int (*builtin_func[]) (char **) = {
  &cd_command,
  &exit_command
};

int exec_pipe(char** str1, char** str2, int val)
{
	char buff[4096];
	int counter = 0;
	ssize_t write_count = 0;
	ssize_t r_count = 0;
	ssize_t read_count = 0;

	int fd1[2];
	int fd2[2];
	pid_t pid1, pid2;

	if(pipe(fd1) == -1){
		fprintf(stderr,"First pipe failed");
		return 1;
	}

	if(pipe(fd2) == -1){
		fprintf(stderr,"Second pipe failed");
		return 1;
	}

	/* fork a child process */
	pid1 = fork();	
	
	if(pid1 == 0){
		//CHILD1		
		dup2(fd1[WRITE], 1);
		for (int i = 0; i < bil_builtins(); i++) {
				if (strcmp(str1[0], bilshell_commands[i]) == 0) {
					(*bilshell_func[i])(str1,val);
				}
			}
		execvp(str1[0],str1);
		
		exit(0);	
	}

	else if (pid1 < 0){ /* error occurred */
		fprintf(stderr, "First fork failed");
		return 1;
	}
	 else{ /* parent process */
		/*close the unused end of the pipe */
		
		wait(NULL);
                close(fd1[WRITE]);
		while( r_count = read(fd1[READ], buff, val) > 0)
		{
			write_count += write(fd2[WRITE],buff,val);
			read_count += r_count;
		}
		
		close(fd1[READ]);
		close(fd2[WRITE]);

		/* fork a child process */
		pid2 = fork();
		if(pid2 == 0){
		//CHILD2
		
		dup2(fd2[READ], 0);
		for (int i = 0; i < bil_builtins(); i++) {
				if (strcmp(str2[0], bilshell_commands[i]) == 0) {
					//printf("abc");
					(*bilshell_func[i])(str2,val);
				}
			}
		execvp(str2[0],str2);
		exit(0);
		
		}
		else if(pid2 < 0){ /* error occurred */
			fprintf(stderr, "Second fork failed");
			return 1;
		}
		else if(pid2 > 0){ /* parent process */
			/*close the unused end of the pipe */
		
			wait(NULL);
			close(fd2[READ]);
			printf("read-count: %ld\n", read_count);
			printf("character-count: %ld\n", write_count);
		
		}	
		
	}
	return 0;
}

int execute(char **args)
{
	pid_t pid;

	/* fork a child process*/
	pid = fork();

	if(pid < 0){ /* error occurred */
		fprintf(stderr, "Fork Failed");
		return -1;
	}
	else if(pid == 0){ /* child process */
		execvp(args[0], args);
	}
	else{ /* parent process */
		/*parent will wait for the child to complete */
		wait(NULL);
	}
	return 1;
}

char *read_line(void)
{
	char* comd = NULL;
	ssize_t strsize = 0;
	getline(&comd, &strsize, stdin);
	return comd;
}



char **parse_line(char *comd)
{
	int str_size = 1000;
	int index = 0;

	char **params = malloc(str_size * sizeof(char*));
	char *param;

	if(params ==  NULL)
	{
		exit(EXIT_FAILURE);
	}

	param = strtok(comd," \t\r\n\a");

	while(param != NULL)
	{
		params[index] = param;
		index++;

		param = strtok(NULL," \t\r\n\a");
	}

	params[index] = NULL;

	return params;
}

void interactive(char **args, int val){
	int str_size = 1000;
	char** str_1 = malloc(str_size * sizeof(char*));
	char** str_2 = malloc(str_size * sizeof(char*));
	char *command;
	char ** params;

	char *rand;

	int done;
	while(1){
		printf("Enter command: ");

		command = read_line();
		params = parse_line(command);
		
	
		int n = atoi(params[0]);
		char n_char_value = n + '0';

		if(n_char_value != '0' && isdigit(n_char_value)){
			break;
		}
		if(params != NULL)
		{
			for (int i = 0; i < builtins(); i++) {
				if (strcmp(params[0], builtin_commands[i]) == 0) {
					(*builtin_func[i])(params);
				}
			}
			
			int k = 0;
			int piped = 0;
			while (params[k] != NULL)
			{

				if(strcmp(params[k], "|") == 0)
				{
					piped = 1;
					break;
				}
				k++;
			}


			if(piped)
			{

				for(int i = 0; i < k; ++i){
					int width = strlen(params[i]) + 1;
					str_1[i] = malloc(width);
					memcpy(str_1[i], params[i], width);
				}

				str_2[0] = params[k + 1];
				str_2[1] = params[k + 2];
				
				exec_pipe(str_1,str_2,val);
				
			}
			else{
				for (int i = 0; i < bil_builtins(); i++) {
				if (strcmp(params[0], bilshell_commands[i]) == 0) {
					(*bilshell_func[i])(params,val);
				}
			}
			}

			if(terminate == EXIT){
				terminate = 0;
				break;
			}
			else if(!piped) {
				done = execute(params);
			}

			free(command);
			free(args);
		}
	}
	free(str_1);
	free(str_2);
}

void batch(char **args, int val){
	int str_size = 10;

	char** str1 = malloc(str_size * sizeof(char*));
	char** str2 = malloc(str_size * sizeof(char*));
	FILE *text;
	FILE *file;
	char line[200];
	char **params;
	
	int done;
	
	int line_count = 0;
	char curr;

	file = fopen(args[1],"r");

	if(file == NULL)
	{
		printf("The file doesn't exist !");
	}
	
	while((curr = fgetc(file)) != EOF){
		if(curr == '\n'){
			line_count++;
		}
	}
	fclose(file);

	text = fopen(args[1], "r");
	if(text == NULL)
	{
		perror("Can't open file.");
		exit(EXIT_FAILURE);
	}
	int count = 0;

	for(int i = 0; i < line_count + 1; i++){
		fgets(line, sizeof(line),text);		
		params = parse_line(line);
		if(params != NULL)
		{
			
			for (int i = 0; i < builtins(); i++) {
    				if (strcmp(params[0], builtin_commands[i]) == 0) {
      				(*builtin_func[i])(params);
    				}
			}
			if(terminate == EXIT){
				terminate = 0;
				break;
			}

			for (int i = 0; i < bil_builtins(); i++) {
    				if (strcmp(params[0], bilshell_commands[i]) == 0) {
      				(*bilshell_func[i])(params,val);
    				}
			}

			int k = 0;
			int piped = 0;
			while (params[k] != NULL)
			{

				if(strcmp(params[k], "|") == 0)
				{
									
					piped = 1;
					break;
				}
				k++;
			}
			if(piped)
			{
				
				for(int i = 0; i < k; ++i){
					
					int width = strlen(params[i]) + 1;
					str1[i] = malloc(width);
					memcpy(str1[i], params[i], width);
				}
				str2[0] = params[k + 1];
				str2[1] = params[k + 2];
				exec_pipe(str1,str2,val);
				free(str1);
				free(str2);
				
			}
			else if(!piped){
				done = execute(params);
			}

		}
		if(!text)
			break;
	}
	
	fclose(text);
	free(params);
	free(args);
}
int main(int argc, char **argv)
{

	char *comd;
	char **args;

	do
	{
		printf("bilshell ");
		comd = read_line();
		args = parse_line(comd);
		printf("\n");

		int val = atoi(args[0]);
		
		char first = val + '0';

		if((val >= 1 && val <= 4096 && args[1] == NULL) && first != '0')
		{
			interactive(args, val);
			exit(0);
		}
		if(val >=1 && val <= 4096 && args[1] != NULL && args[2] == NULL)
		{
			batch(args, val);
			exit(0);
		}
		

	}while(args[0] != "exit");

	free(args);
	free(comd);
	return 0;
}
