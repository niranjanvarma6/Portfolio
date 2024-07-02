#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
//Declaration of the struct and its contents
struct movie
{
    char* title;
    int year;
    float rating;
    char* languages;
    struct movie *next;

};        
//makes the nodes for each movie
struct movie* make_movie(char* current_l) {         
    struct movie* newNode = malloc(sizeof(struct movie));         
    char * end_ptr = NULL;         
    char* language_string;         
    char* ratings_string = NULL;  
    //creation of token to the end of a data type. in this case the title
    char * token = strtok_r(current_l, ",", &end_ptr);         
    newNode->title = calloc(strlen(token) + 2, sizeof(char));           
    strcpy(newNode->title, token);           
    token = strtok_r(NULL, ",", &end_ptr);         
    newNode->year = atoi(token);           
    // ] used for extracting the languages
    token = strtok_r(NULL, "]", &end_ptr);          
    language_string = calloc(strlen(token) + 1, sizeof(char));   
    strcpy(language_string, token);       
    //remove [ from the extracted languages 
    memmove(&language_string[0], &language_string[1], strlen(language_string));                   
    // \n used as endpoint to indicate the end of the line of the csv line
    token = strtok_r(NULL, "\n", &end_ptr);         
    ratings_string = calloc(strlen(token) + 1, sizeof(char));         
    strcpy(ratings_string, token);        
    memmove(&ratings_string[0], &ratings_string[1], strlen(ratings_string));
    //convert into a string and allocate  
    newNode->rating = atoi(ratings_string);                    
    newNode->next = NULL;                          
    return newNode; 
  }

//Function reads the file's contents
struct movie* read_file(char* file_n){
    FILE * mov_file = fopen(file_n, "r");
    size_t len = 0;
    ssize_t read;
    char * current_line;
    struct movie* head = NULL;
    struct movie* tail = NULL;
    //First line read out and ignored from the csv
    getline(&current_line, &len, mov_file);
    int size = 0;
    //while loop to allocate each line of code into the node by creating a new node for each line
    while((read = getline(&current_line, &len, mov_file)) != -1){
        //new struct to allocate the memory for the current line being read through
        struct movie* newNode = make_movie(current_line);
        size++;
        if(head == NULL){
            head = newNode;
            tail = newNode;
        }
        else{
            tail->next = newNode;
            tail = newNode;
        }
    }
    //free memory and close files
    free(current_line);
    fclose(mov_file);
    return head;
}
//processing movies into txt files by year
void process_file(char* file_n){
    char direct[] = "varman.movies.";
    //creating a random number
    srand(time(NULL));
    int n = random() % 99999;
    //string variable of the random number 
    char r[150];
    //year variable
    int yerrr = 0;
    // r gets the random number
    sprintf(r, "%d", n);
    // the random string concatinates with the string
    strcat(direct, r);
    //directory made
    mkdir(direct, 0750);
    printf("Created directory with name %s \n", direct);
    //reads the particular file that is chosen by the user/largest size/ smallest size and extracts info
    struct movie* mov = read_file(file_n);
    //changes the directory to inside of varman etc.
    chdir(direct);
    //sets a baseline year
    yerrr = mov->year;
    //all the txt files of the directory are made inside the loop
    while(mov){
        //building blocks for txt files naming and opening to put contents inside
        char name[200];
        sprintf(name, "%d", yerrr);
        strcat(name, ".txt");
        FILE *fill = fopen(name, "a+");
        //make it read only to groups
        chmod(name, 0640);
        //this while loop goes through the whole struct to see which movies correspond to the particular year to be put into the txt file(using fprint)
        while(mov && mov->year == yerrr){
            fprintf(fill, "%s \n", mov->title);
            mov= mov->next;
        }
        //close the file
        fclose(fill);
        //the if statement is used to change the year of the txt file being made  
        //to accomodate to the movie that was exited of the previous while loop since the years did not correspond
        if(mov){
            yerrr = mov->year;
        }
    }
    //exit out of the varman directory
    chdir("..");
}
//When the user chooses 1 and creates a directory to the largest file directory
void choice1_(){
    int max = 0;
    //directory file object created
    struct dirent *d;
    //stat struct used to find file size
    struct stat dir_stat;
    char movie_max[100000];
    //open directory
    DIR *direto = opendir(".");
    //error handling to make sure the directory is able to be opened
    if(direto == NULL){
        printf("Directory could not be opened");
        return;
    }
    //while loop to go through files 
    while((d = readdir(direto)) != NULL){
        //strncmp used to make sure that the name of the csv file starts with movies_ but is not the executable file in the directory
        if((strncmp("movies_", d->d_name, strlen("movies_")) == 0) && (strncmp("movies_by_year", d->d_name, strlen("movies_by_year")) != 0)){
            //dir_stat is assigned the byte size of the file
            stat(d->d_name, &dir_stat);
            //makes sure the largest file so far is compared with the file being checked now
            if(dir_stat.st_size > max){
                max = dir_stat.st_size;
                //assigned as the largest file so far
                strcpy(movie_max, d->d_name);
            }
        }
    }
    //close directory
    closedir(direto);
    printf("Now processing the chosen file named %s \n", movie_max);
    //calls for the largest file to be made into a directory of txt files
    process_file(movie_max);
}
//When the user chooses 2 and creates a directory to the smallest file
void choice2_(){
    int min = 1000000000;
    //directory file object created
    struct dirent *d;
    //stat struct used to find file size
    struct stat dir_stat;
    char movie_min[100000];
    //open directory
    DIR *direto = opendir(".");
    //error handling to make sure the directory is able to be opened
    if(direto == NULL){
        printf("Directory could not be opened");
        return;
    }
    //while loop to go through files
    while((d = readdir(direto)) != NULL){
        //strncmp used to make sure that the name of the csv file starts with movies_ but is not the executable file in the directory
        if((strncmp("movies_", d->d_name, strlen("movies_")) == 0) && (strncmp("movies_by_year", d->d_name, strlen("movies_by_year")) != 0)){
            //dir_stat is assigned the byte size of the file
            stat(d->d_name, &dir_stat);
            //makes sure the smallest file so far is compared with the file being checked now
            if(dir_stat.st_size < min){
                min = dir_stat.st_size;
                //assigned as the smallest file so far
                strcpy(movie_min, d->d_name);
            }
        }
    }
    //close directory
    closedir(direto);
    printf("Now processing the chosen file named %s \n", movie_min);
    //calls for the smallest file to be made into a directory of txt files
    process_file(movie_min);
    
}
//When the user chooses 3
void choice3_(){
    int flag = 0;
    // directory file object 
    struct dirent *d;
    char find[100000];
    //while loop used for error handling when user inputs wrong file name
    while (flag == 0){
        //directory opened 
        DIR *direto = opendir(".");
        printf("Enter the complete file name: ");
        //clear input in case error for next input
        memset(find, 0, strlen(find));
        fflush(stdin);
        scanf("%s", &find);
        //while loop goes through the directory
        while((d = readdir(direto)) != NULL){
            //if a matching file name is found then it breaks the loops and moves to processing the file as a directory
            if(strcmp(d->d_name, find) == 0){
                //makes sure not to have to go through error handling loop
                flag = 1;
                break;
            }
        }
        //error handling used to check if a file doesn't exist in the directory
        if(flag == 0){
            printf("The file %s was not found. Try again \n", find);
            continue;
        }
        //close directory to reset
        closedir(direto);
    }
    
    printf("Now processing the chosen file named %s \n", find);
    //calling function to make directory with txt files for chosen csv file
    process_file(find);
}
//function that you see when running the program
void main_menu(){
    int choice;
    //loop used to structure error handling 
    while(1){
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");
        scanf("%d", &choice);
        if(choice<4 && choice>0){
            break;
        }
        //error handling
        else{
            printf("You entered an incorrect choice. Try again.\n");
            continue;
        }
    }
    //execution of commands takes place
        if(choice == 1){
            choice1_();
        }
        else if(choice == 2){
            choice2_();
        }
        else if(choice == 3){
            choice3_(); 
        }
}
//executable function
int main()
{
    int ch;
    //while loop used to make it endless and error handling and quit by
    while(1){
        printf("1. Select file to process. \n");
        printf("2. Exit the program. \n");
        scanf("%d", &ch);
        //only way to quit the program
        if(ch == 2){
            break;
        }
        //error handling
        else if(ch > 2 || ch < 1){
            printf("Choose between the given choices \n");
            continue;
        }
        //calling for the program execution after choice of 1 is made
        main_menu();
    }
return 0;
}