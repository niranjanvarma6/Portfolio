#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Declaration of the struct and its contents
struct movie
{
    char* title;
    int year;
    float rating;
    char* languages;
    struct movie *next;

};
//When the user chooses 1
void choice1_(struct movie* move){
    int user_year;
    int file_check = 0;
    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &user_year);
    
    while(move != NULL){
        if(move->year == user_year){//if the year of the movies being checked is there then print it out
            file_check++;
            printf("%s \n", move->title);
        }
        move = move -> next;
    }
    //if statement used to make sure that there is no data for year chosen
    if(file_check == 0){
        printf("No data about movies released in the year %d \n", user_year);
    }
}
//When the user chooses 2
void choice2_(struct movie* move){
    int i = 1900;
    char str[2000];
    //For loop used to go through the rough amount of years
    for(i; i <2100; i++){
        float highest = -5;
        struct movie* node = move;
        while(node != NULL){
            if((node->year == i) && (node->rating > highest)){
                highest = node->rating;
                strcpy(str, node->title);
            }
            node = node-> next;
        }
        if(highest != -5){
            printf("%d %s \n", i, str);
        }    
    } 
}
//When the user chooses 3
void choice3_(struct movie* move){
}        
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

    printf("Parsed data for %d movies\n", size);
    //free memory and close files
    free(current_line);
    fclose(mov_file);
    return head;
}
//function that you see when running the program
void main_menu(struct movie* move){
    int choice;
    //loop used to structure error handling 
    while(1){
        printf("\n1. Show movies released in the specified year\n");
        printf("2. Show hightest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n\n");
        scanf("%d", &choice);
        if(choice<5 && choice>0){
            break;
        }
        //error handling
        else{
            printf("You entered an incorrect choice. Try again.\n");
            continue;
        }
    }
    //execution of commands except quit takes place
        if(choice == 1){
            choice1_(move);
            main_menu(move);
        }
        else if(choice == 2){
            choice2_(move);
            main_menu(move);
        }
        else if(choice == 3){
            choice3_(move); 
            main_menu(move); 
        }
}
//executable function
int main(int argc, char *argv[])
{
    //simple error handling
    if (argc < 2)
	{
		printf("Provide a file to process.\n");
		return 1;
	}
    else if(argc > 2)
    {
        printf("Error too many command.\n");
    }
    //true execution of program takes place here
    struct movie * list = read_file(argv[1]);
    main_menu(list);
return 0;
}