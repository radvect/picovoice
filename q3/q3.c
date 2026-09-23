#define _GNU_SOURCE


#include <ctype.h>
#include <stdio.h>
#include <curl/curl.h>
#include "uthash.h" 
#include <string.h>

struct HASH_ITEM {
    char word[1024];             
    int count;  
    UT_hash_handle hh;    
};

int by_id(const struct HASH_ITEM *a, const struct HASH_ITEM *b) {
    return -(a->count - b->count);
}




void line_to_hash(char* line, struct HASH_ITEM **table ){    
    char * words = strtok(line, "\n ");
    while(words!=NULL){
        struct HASH_ITEM *item= NULL;
        HASH_FIND_STR(*table, words, item);
        if(item!=NULL){
            item->count++;
        }
        else{
            item = malloc(sizeof(struct HASH_ITEM));
            strcpy(item->word, words);
            item->count = 1;
            
            HASH_ADD_STR(*table, word, item);}
        words = strtok(NULL, "\n ");
    }   


}

char* line_preprocessing(char* line){
    int i = 0;

    while(line[i]!='\0'){
        if(ispunct((unsigned char) line[i])){
            line[i] = ' ';
        }
        else{
            line[i] = (char)tolower((unsigned char) line[i]);
        }
        i++;

    }
    return line;
}

// }
void download_file(const char *url, const char* filename){

    FILE *file = fopen(filename, "wb") ;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,file);

    CURLcode result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();
}


void file_to_hash(FILE *file, struct HASH_ITEM ** table){
    char line[4096];
 
        
    while (fgets(line, sizeof(line), file) != NULL){
        char* line_prep;
        line_prep = line_preprocessing(line);
        line_to_hash(line_prep, table);
    }

    HASH_SORT(*table, by_id);


}







char **find_frequent_words(const char *path, int32_t n){
    FILE *f = fopen("shakespear.txt", "r");
    struct HASH_ITEM *hash_table = NULL;    
    file_to_hash(f, &hash_table);
    int i =0;
    struct HASH_ITEM *s = hash_table;
    
    char** res = malloc(sizeof(char * )*n) ;

    while(s !=NULL && i<n){
        res[i] = strdup(s->word);
        i++;
        s = s->hh.next;
    }
    for (int i = 0; i<n; i++){
        printf("%s ", res[i]);
    }   

    return res;
}

int main(){
    download_file("https://storage.googleapis.com/download.tensorflow.org/data/shakespeare.txt", "shakespear.txt");
    
    find_frequent_words("sheakspear.txt", 4);


    return 0;
}



