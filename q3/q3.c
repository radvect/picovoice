#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <curl/curl.h>
#include "uthash.h" 
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

struct HASH_ITEM {
    char word[1024];  // a reasonable assumption that English doesn't have such long words           
    int count;  
    UT_hash_handle hh;    
};

// int by_id(const struct HASH_ITEM *a, const struct HASH_ITEM *b) {
//     return -(a->count - b->count);
// } for sorting HASH function - updated to heap option

int by_count(const void *a, const void *b) {
    struct HASH_ITEM *x = *(struct HASH_ITEM **)a;
    struct HASH_ITEM *y = *(struct HASH_ITEM **)b;

    return y->count - x->count;
}

//HEAPIFY///////////////////////////////////////////
void heapify_up(struct HASH_ITEM **heap, int index) {
        
    while(index!=0){
        
        if(heap[(index-1)/2]->count <= heap[index]->count){
            break;
        }
        struct HASH_ITEM *temp = heap[index];
        heap[index] = heap[(index-1)/2];
        heap[(index-1)/2] = temp;
        index = (index-1)/2;
    }
}
void heapify_down(struct HASH_ITEM **heap, int index, int heap_size) {

    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left >= heap_size) {
            break;
        }
        if (right >= heap_size) {
            if (heap[index]->count > heap[left]->count) {
                struct HASH_ITEM *temp = heap[index];
                heap[index] = heap[left];
                heap[left] = temp;}
            break;
        }

        if (heap[index]->count <= heap[left]->count && heap[index]->count <= heap[right]->count) {
            break;
        }

        if (heap[right]->count <= heap[left]->count) {
            struct HASH_ITEM *temp = heap[index];
            heap[index] = heap[right];
            heap[right] = temp;
            index = right;
        }
        else {
            struct HASH_ITEM *temp = heap[index];
            heap[index] = heap[left];
            heap[left] = temp;
            index = left;}
    }
}
    




///////////////////////////////////////////////////////////////

void line_to_hash(char* line, struct HASH_ITEM **table ){    //preprocessing words in a strign
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

char* line_preprocessing(char* line){ // removing punctuation marks and setting the low register
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
void download_file(const char *url, const char* filename){ //nothing essential, just my personal curiosity of writing a downloading function

    FILE *file = fopen(filename, "wb") ;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,file);

    CURLcode result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    fclose(file);
}


void file_to_hash(FILE *file, struct HASH_ITEM ** table){
    char line[4096]; // string should be smaller than 4000 symbols
        
    while (fgets(line, sizeof(line), file) != NULL){
        char* line_prep;
        line_prep = line_preprocessing(line);
        line_to_hash(line_prep, table);
    }

    //HASH_SORT(*table, by_id); an  old solution - we can reduce complexity from len(words)log(len(words)) to len(words)log n
    // assuming that the small n are passed to the function

}



char **find_frequent_words(const char *path, int32_t n){
    if (n <= 0) {
        return NULL;
    }


    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return NULL;
    }


    struct HASH_ITEM *hash_table = NULL;    
    file_to_hash(f, &hash_table);// building hash
    int i =0;
    struct HASH_ITEM *s = hash_table; 
    


    // my initial hash sorting solution - before heap introduction

    // char** res = malloc(sizeof(char * )*n) ;

    // while(s !=NULL && i<n){
    //     res[i] = strdup(s->word);
    //     i++;
    //     s = s->hh.next;
    // }
    // for (int i = 0; i<n; i++){
    //     printf("%s ", res[i]);
    // }   
    

    char** res = malloc(sizeof(char * )*n) ;
    struct HASH_ITEM ** min_heap = malloc(sizeof(struct HASH_ITEM *)*n);
    int min_heap_size= 0 ;
    
    for (s = hash_table; s != NULL; s = s->hh.next) { 
        if(min_heap_size<n){ //if heap isn't full 
            min_heap[min_heap_size] = s;
            min_heap_size++;
            heapify_up(min_heap, min_heap_size - 1); // after adding an element we need to pull it higher to recover min heap structure
        }
        else if(min_heap[0]->count < s->count){
            min_heap[0] = s;
            heapify_down(min_heap, 0, min_heap_size); // if a heap is full and we found out that the top element is bigger than lower after we replaced it- need to push it down
        }
    }



    qsort(min_heap, min_heap_size,
        sizeof(struct HASH_ITEM *),
        by_count);
    for (int i = 0; i < min_heap_size; i++) {
        res[i] = strdup(min_heap[i]->word);
    }
    struct HASH_ITEM *current, *tmp;

    HASH_ITER(hh, hash_table, current, tmp) {
    HASH_DEL(hash_table, current);
    free(current);
    }
    free(min_heap);

    fclose(f);

    return res;
}

int main(){
    download_file("https://storage.googleapis.com/download.tensorflow.org/data/shakespeare.txt", "shakespear.txt");
    
    int n = 10;
    char ** res = find_frequent_words("shakespear.txt", n);


    for (int i = 0; i < n; i++) {
        printf("%s\n", res[i]);
        free(res[i]);
    }

    free(res);

    return 0;
}



