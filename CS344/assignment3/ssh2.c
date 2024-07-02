#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
//declared global variables for the program
//the 2 structs used to 
struct sigaction sigint;
struct sigaction sigtstp;
//used to check if it is on background mode(1) or foreground mode(0) and ignoring &
int background_mode = 1;
int pid;
//
int background_check = 1;
//parses commands into arrays that can be more easily understood and executed to corresponding commands, or they could be input or output files
void input(char* com_data[], char infile[300], char outfile[300]) {
	char in[2048];
    memset(in,0,strlen(in));
    printf(": ");
    fflush(stdout);
    //takes in command put in by user
    fgets(in, 2048, stdin);
    //tokening the input line by line
    fflush(stdin);
    strtok(in, "\n");
    int i;
    for (i = 0; i< strlen(in); i++){ //turn the $$ into pid
        if (in[i] == '$' && in[i-1] == '$'){
            strcpy(&in[i-1], &in[i]);
            strcpy(&in[i-1], &in[i]);
            char pid[10];
            sprintf(pid, "%d", getpid());
            char tempstr [3000];
            memset(tempstr,0,strlen(tempstr));
            strncpy(tempstr, in, i);
            strcat(tempstr, pid);
            strcat(tempstr, in + i);
            strcpy(in, tempstr);
        }
    }
    //token the whole input 
    char *datab = strtok(in, " ");
    //for loop used to put different inputs into different categories of arrays
	for (i = 0; datab; i++) {
        //check to make sure it is an input file and parsed into the infile array
        if(strcmp(datab, "<") == 0){
            datab = strtok(NULL, " ");
            strcpy(infile, datab);
        }
        //check to make sure it is an output file and parsed into the outfile array
        else if(strcmp(datab, ">") == 0){
            datab = strtok(NULL, " ");
            strcpy(outfile, datab);
        }
        //used to ignore the & by making sure that it is not a part of the command 
        else if(strcmp(datab, "&") == 0){
            background_check = 0;
        }		
        //all commands are duplicated into a particular index of com_data array
        else {
            com_data[i] = strdup(datab);
		}
        //creates space to move onto the next input to categorize
		datab = strtok(NULL, " ");
	}
}
//gives the exit status of a process
void check_status(int stat_idd){
    //process was exited with status(normally)
    if(WIFEXITED(stat_idd)){
        printf("exited with status %d\n", WEXITSTATUS(stat_idd));
        fflush(stdout);
    }
    //process terminated by a signal
    else{
        printf("terminated with signal %d\n", WTERMSIG(stat_idd));
        fflush(stdout);
    }
}
//executes the commands by reading the arrays with lists of commands or files 
void execut(char* com_data[], char infile[300], char outfile[300], int* stat_id){
    int in;
    int out;
    int result;
    //initialize process
    pid_t spawnpid = -5;
    //create a child process by forking
    spawnpid = fork();
    //failed fork
    if(spawnpid == -1){
        perror("fork() ain't working");
        exit(1);
    }
    //returned to child process
    else if(spawnpid == 0){ //source for code idea: https://stackoverflow.com/questions/37303570/c-redirecting-stdout-to-a-file-for-process-called-by-exec-in-child-process
        //used to handle ctrlc by taking the default action
        sigint.sa_handler = SIG_DFL;
        sigaction(SIGINT, &sigint, NULL);

        //makes sure the input file is not blank
        if (strcmp(infile, "") != 0){
            //attempt to open input file
            in = open(infile, O_RDONLY);
            //error handling for files that couldn't be opened with exit of process
            if (in == -1) {
                perror("Unable to open input file\n");
                exit(1);
            }
            //attempt to assign the input file
            result = dup2(in, 0);
            //error handling to show input file couldn't be assigned with exiting of process
            if (result == -1) {
                perror("Unable to assign input file\n");
                exit(2);
            }
            //close the input file 
            fcntl(in, F_SETFD, FD_CLOEXEC);
        }
        //makes sure the output file is not blank
        if (strcmp(outfile, "") != 0){
            //attempt to open input file
            out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666); 
            //error handling for files that couldn't be opened with exit of process
            if (out == -1) {
                perror("unable to open output file\n");
                exit(1);
            }
            //attempt to assign the output file
            result = dup2(out, 1);
            //error handling to show output file couldn't be assigned with exiting of proces
            if (result == -1) {
                perror("unable to assign output file\n");
                exit(2);
            }   
            //close the output file
            fcntl(out, F_SETFD, FD_CLOEXEC);
        }
        //execute the command
        //an error check which is used to see if the process went through. If not the error is given and the process is exited
        if (execvp(com_data[0], (char* const*)com_data) == -1) { 
            printf("file does not exist \n");
            fflush(stdout);
            exit(2);
        }
    }
    //when the process is returned to the parent/caller
    else{
        //
        if(background_mode == 0 || background_check == 1){
            pid_t verifypid = waitpid(spawnpid, stat_id, 0);
        }
        else{
            pid_t verifypid = waitpid(spawnpid, stat_id, WNOHANG);
            printf("background pid is %d \n", spawnpid);
            fflush(stdout);
        }
    }
    //used to make sure that the child process has been terminated 
    if ((spawnpid = waitpid(-1, stat_id, WNOHANG)) > 0) {
        printf("child terminated %d \n", spawnpid);
        check_status(* stat_id);
        fflush(stdout);
    }
}
// this function is used to change the mode of the shell to either foreground or background mode
void handleSIGTSTP(){
    //exit foreground and back into background
    if(background_mode == 0){
        background_mode = 1;
        printf("exiting foreground only mode\n");
        fflush(stdout);
    } 
    //enter foreground and ignore & or other variables like ctrlz
    else{
        background_mode = 0;
        printf("entering foreground only mode\n");
        fflush(stdout);
    } 
}
//used to change the directory if the user commands a 'cd'
void check_directory(char * data[512]){
    //if there is anything else besides cd
    if(data[1]){
        //directory check unsuccessful
        if(chdir(data[1]) == -1){
            printf("the directory was not found");
            fflush(stdout);
        }
        //successfully changed the directory
        else{
            chdir(data[1]);
        }
    }
    //redirected home since nothing is given
    else{
        chdir(getenv("HOME"));
    }
}
//where calls to particular functions are made for the shell
int commando(){
    //the 2 variables that are used as arrays to store input and output files categorized when scanning the input 
    char fin[300];
    char fout[300];
    memset(fin,0,strlen(fin));
    memset(fout,0,strlen(fout));
    //exit status initial value
    int stat_ = 0;
    //initializing the inputs so weird encryptions are outputted during the running of the program
    char * inter[512] = {0};
    //pid value is assigned
    pid = getpid();
    //syntax used to ignore ctrlc by using sig_ign 
	sigint.sa_handler = SIG_IGN;
	sigfillset(&sigint.sa_mask);
	sigint.sa_flags = 0;
	sigaction(SIGINT, &sigint, NULL);
    //ctrlz ignored by changing the mode to foreground through handleSIGTSTP 
	sigtstp.sa_handler = handleSIGTSTP;
	sigfillset(&sigtstp.sa_mask);
	sigtstp.sa_flags = 0;
	sigaction(SIGTSTP, &sigtstp, NULL);
    //used in for-loop later on
    int i;
    //while loop used for repetition in the shell of course until the exit command is used
    while(1){
        input(inter, fin, fout);
        if (inter[0][0] == '#' || inter[0][0] == '\0'){
            //ignore commands with # or that are null by not executing
            continue;
        }
        else if (inter[0][0] == 's' && inter[0][1] == 't' && inter[0][2] == 'a' && inter[0][3] == 't' && inter[0][4] == 'u' && inter[0][5] == 's'  ){
            //gives the exit-status 
            check_status(stat_);
        }
        else if (inter[0][0] == 'c' && inter[0][1] == 'd'){
            //redirects to handle the change in directory
            check_directory(inter);
        }
        else if (inter[0][0] == 'e' && inter[0][1] == 'x' && inter[0][2] == 'i' && inter[0][3] == 't' ){
            //exiting the loop
            break;
        }
        else{
            //execution of more complicated commands
            execut(inter, fin, fout, &stat_);
        }
        // Reset variables to take in next potential command in the shell
        // Without this set of code the code will not be able to repeat the program properly with overloaded variables
        for (i=0; inter[i]; i++){
            inter[i] = NULL;
        }
        background_check = 1;
        fin[0] = '\0';
        fout[0] = '\0';
    }
}
//main function made and reduced to where a different function can handle the programs
int main(){
    commando();
    return 0;
}
