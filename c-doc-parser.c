#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

struct word {
    char* data;
};

struct sentence {
    struct word* data;
    int word_count;//denotes number of words in a sentence
};

struct paragraph {
    struct sentence* data  ;
    int sentence_count;//denotes number of sentences in a paragraph
};

struct document {
    struct paragraph* data;
    int paragraph_count;//denotes number of paragraphs in a document
};

/* INPUT.txt
2                               -* int paragraph_count (number of paragraphs in the document)
Learning C is fun.              -* paragraph[0]: a string containing 1 sentence
Learning pointers is more fun.It is good to have pointers. -* paragraph[1]: 2 sentence string
3                               -* int q, number of queries
1 2                             -* query type 1: print paragraph #2 (index 2 -> 1-based)
2 1 1                           -* query type 2: print sentence #1 of paragraph #1
3 1 1 1                         -* query type 3: print word #1 of sentence #1 of paragraph #1
*/


// Function that returns a document which contains 
// struct paragraph data
// int num of paragraphs
// THIS FUNCTION MUST BUILD AND RETURN A STRUCT DOCUMENT
// WHOSE DATA POINTS TO AN ARRAY OF PARAGRAPHS
// Doc.data = {{{"Learning", "C", "is", "fun"}}, {{"Learning", "pointers", "is", "more", "fun"}, {"It", "is", "good", "to", "have", "pointers"}}};
// EACH PARAGRAPH MUST CONTAIN ITS OWN ARRAY OF SENTENCES
// EACH SENTENCE IS ITS OWN ARRAY OF WORDS

// I AM TURNING ONE FLAT STRING INTO A NESTED TREE OF ARRAYS
struct document get_document(char* text) {
    
    // initialize 'doc' to be returned
    struct document doc;
    
    // create indices to scan through every struct data type: 
    // - char, word, sentence, parapgraph
    int char_idx = 0;
    int word_idx = 0;
    int sen_idx = 0;
    int par_idx = 0;
    
    // create variables to record the number of data types
    // - word, sentence, paragraph
    int word_count = 0;
    int sentence_count = 0;
    int paragraph_count = 1; // first paragraph already exists; each '\n' adds another

    // Traverse until we reach the end of the text string
    while(text[char_idx] != '\0'){
        // Did we reach a paragraph separator ('\n') ?
        if(text[char_idx] == '\n'){
            // Another paragraph follows, so increase paragraphCount
            paragraph_count++;
        }
        // Move to the next character in the text
        char_idx++;
    }
    
    // Now, I know how many paragraph slots I need
    // Store the total number of paragraphs in the document stuct
    doc.paragraph_count = paragraph_count;
    
    // Allocate array with one struct paragraph slot for each paragraph
    // memory allocation: malloc(quantity * sizeof(data-type)) -> total bytes
    doc.data = malloc(doc.paragraph_count * sizeof(struct paragraph));
    
    // ???- HAVE I REACHED THE END OF THIS PARAGRAPH -???
    // ???- OR HAVE I REACHED THE END OF THE ENTIRE DOCUMENT -???
    // Begin counting sentences for the current paragraph.
    
    // Restart from the beginning because the first pass ended at '\0'
    // You set it back to 0 so it can do its next job: 
    // start at the first character again and parse/build the actual paragraph data:
    char_idx = 0;   
    
    // Build each allocated paragraph slot one at a time
    for(par_idx = 0; par_idx < doc.paragraph_count; par_idx++){
        // This count belongs only to the paragraph currently being built
        sentence_count = 0;
        
        // Stay inside this paragraph until newline or end-of-document delimiters
        while(text[char_idx] != '\n' && text[char_idx] != '\0'){
            // A period finished one sentence in this paragraph
            // Check if there is a period, IF so incremement sentence count
            if(text[char_idx] == '.'){
                sentence_count++;
            }
            // Examine the next character
            char_idx++;
        }
        // Save this paragraph's number of sentences
        doc.data[par_idx].sentence_count = sentence_count;
        
        // Allocate memory for one sentence struct slot for each sentence found
        // memory allocation: malloc(quantity * sizeof(data-type)) -> total bytes
        doc.data[par_idx].data = malloc(sentence_count * sizeof(struct sentence));
        
        // Skip the newline so the next paragraph starts at its first character
        if(text[char_idx] == '\n'){
            char_idx++;
        }
    }
    
    // So far: 
    // doc.data contains allocated paragraph slots
    // each paragraph contains an allocated sentence array
    
    // Reset char_idx; re-scan document from start and count/allocate word array for each sentence
    // Each paragraph scan ends at '\n'; the final paragraph ends at '\0' (End of Doc). 
    char_idx = 0;
    
    
    // BUILD THE WORD ARRAY FOR EVERY SENTENCE IN EVERY PARAGRAPH
    
    // Visit one paragraph at a time
    for(par_idx = 0; par_idx < doc.paragraph_count; par_idx++){
        // Visit each sentence that belongs to this paragraph
        for(sen_idx = 0; sen_idx < doc.data[par_idx].sentence_count; sen_idx++){
            
            // Every sentence has at least one word; each space adds another.
            word_count = 1;
            
            // Count the words in this sentence
            // scan entire sentence until '.' marks its end
            while(text[char_idx] != '.'){
                // Each space means another word follows
                // IF we reach a 'space', we increment word count by 1
                if(text[char_idx] == ' '){
                    word_count++;
                }
                char_idx++;
            }
            // Save the current sentence's word count
            doc.data[par_idx].data[sen_idx].word_count = word_count;
            
            // Allocate one word struct slot for every word in this sentence
            doc.data[par_idx].data[sen_idx].data = malloc(word_count * sizeof(struct word));
            
            // Skip '.' so the next scan starts at the following sentence
            char_idx++;
        }
        
        // If a '\n' follows, skip it so char_idx starts at the next paragraph.
        if(text[char_idx] == '\n'){
            char_idx++;
        }
        
    }
    
    // RESET 
    char_idx = 0;
    
    // BUIlD AND COPY THE ACTUAL CHARACTER STRING FOR EVERY WORD
    
    // Visit one paragraph at a time
    for(par_idx = 0; par_idx < doc.paragraph_count; par_idx++){
        // Visit each sentence in the current paragraph
        for(sen_idx = 0; sen_idx < doc.data[par_idx].sentence_count; sen_idx++){
            // Visit each word in the current sentence
            for(word_idx = 0; word_idx < doc.data[par_idx].data[sen_idx].word_count; word_idx++){
                // Save the current character position as this word's starting point
                int start_idx = char_idx;
                // move char_idx until space, '.', '\n', or '\0'
                while(text[char_idx] != ' ' && 
                      text[char_idx] != '.' && 
                      text[char_idx] != '\n' && 
                      text[char_idx] != '\0'){
                        char_idx++;
                }
                
                // We need to know how much memory to allocate for the current word
                int length = char_idx - start_idx;
                
                // Allocate space for the word's letters plus one byte for the null terminator
                doc.data[par_idx].data[sen_idx].data[word_idx].data = malloc((length+1) * sizeof(char));
                
                // Copy each character from original text into this word's new bufer
                for(int copy_idx = 0; copy_idx < length; copy_idx++){
                    doc.data[par_idx].data[sen_idx].data[word_idx].data[copy_idx] = text[start_idx + copy_idx];
                }
                
                // Add '\0' so this character buffer comes a valid C string
                doc.data[par_idx].data[sen_idx].data[word_idx].data[length] = '\0';
            
                // If this word ended at a space, skip the space before reading next word.
                if(text[char_idx] == ' '){
                    char_idx++;
                }
            
            }
            
            if(text[char_idx] == '.'){
                char_idx++;
            }
              
        }
        
        if(text[char_idx] == '\n'){
            char_idx++;
        }
        
    }
         
    return doc;

}

// Return paragraph using Doc.data[n-1].data[m-1].data[k-1]
struct word kth_word_in_mth_sentence_of_nth_paragraph(struct document Doc, int k, int m, int n) {
    return Doc.data[n-1].data[m-1].data[k-1];

}

// Return paragraph using Doc.data[n-1].data[m-1]
struct sentence kth_sentence_in_mth_paragraph(struct document Doc, int k, int m) { 
    return Doc.data[m-1].data[k-1];
}

// Return paragraph using Doc.data[n-1]
struct paragraph kth_paragraph(struct document Doc, int k) {
    return Doc.data[k-1];
}


void print_word(struct word w) {
    printf("%s", w.data);
}

void print_sentence(struct sentence sen) {
    for(int i = 0; i < sen.word_count; i++) {
        print_word(sen.data[i]);
        if (i != sen.word_count - 1) {
            printf(" ");
        }
    }
}

void print_paragraph(struct paragraph para) {
    for(int i = 0; i < para.sentence_count; i++){
        print_sentence(para.data[i]);
        printf(".");
    }
}

void print_document(struct document doc) {
    for(int i = 0; i < doc.paragraph_count; i++) {
        print_paragraph(doc.data[i]);
        if (i != doc.paragraph_count - 1)
            printf("\n");
    }
}

char* get_input_text() {	
    int paragraph_count;
    scanf("%d", &paragraph_count);

    char p[MAX_PARAGRAPHS][MAX_CHARACTERS], doc[MAX_CHARACTERS];
    memset(doc, 0, sizeof(doc));
    getchar();
    for (int i = 0; i < paragraph_count; i++) {
        scanf("%[^\n]%*c", p[i]);
        strcat(doc, p[i]);
        if (i != paragraph_count - 1)
            strcat(doc, "\n");
    }

    char* returnDoc = (char*)malloc((strlen (doc)+1) * (sizeof(char)));
    strcpy(returnDoc, doc);
    return returnDoc;
}

int main() 
{
    char* text = get_input_text();
    struct document Doc = get_document(text);

    int q;
    scanf("%d", &q);

    while (q--) {
        int type;
        scanf("%d", &type);

        if (type == 3){
            int k, m, n;
            scanf("%d %d %d", &k, &m, &n);
            struct word w = kth_word_in_mth_sentence_of_nth_paragraph(Doc, k, m, n);
            print_word(w);
        }

        else if (type == 2) {
            int k, m;
            scanf("%d %d", &k, &m);
            struct sentence sen= kth_sentence_in_mth_paragraph(Doc, k, m);
            print_sentence(sen);
        }

        else{
            int k;
            scanf("%d", &k);
            struct paragraph para = kth_paragraph(Doc, k);
            print_paragraph(para);
        }
        printf("\n");
    }     
}
