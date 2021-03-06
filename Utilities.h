/*
 * @file Utilities.h
 * @brief Markov algorithm generator text for basic English language
 * @author Luis Daniel Carvallo González
 * @version 1.0
 * @date 27/11/2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#ifndef UTILITIES_H
#define UTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* UTILITIES_H */

/**
 * @brief Prototype to search and compare words with qsort functions and 
 * bsearch.
 */
typedef int (* compare)(const void *, const void *);

/**
 * @struct index_word
 * @brief Structure that stores words with an index.
 */
struct index_word {
    int  str_int;
    char str_char[30];
} typedef registry;

/**
 * @brief Convert words to lower case
 * 
 * @param Word without format
 * @return Word with correct format
 */
registry check_word(registry word) {
    registry temp_word = word;
    int i = 0;
    int j = 0;
    size_t word_length = 0;

    word_length = strlen(temp_word.str_char);

    for (i = 0; i <= word_length; i++) {
        temp_word.str_char[i] = tolower(temp_word.str_char[i]);
    }
    while (temp_word.str_char[j] != '\0') {
        if (ispunct(temp_word.str_char[j])) {
            for (i = j; i <= word_length; i++) {
                temp_word.str_char[i] = temp_word.str_char[i + 1];
            }
        } else {
            j++;
        }
    }
    return ( temp_word );
}

/**
 * @brief Transforms the error number in the expression to an error message.
 *
 * @param Message to print.
 * @return Void.
 */
void p_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
    return;
}

/**
 * @brief Generate text based on the word list English language and basic 
 * probability matrix.
 *
 * @param ptr_1 Pointer to words list.
 * @param file File to store words.
 * @param matrix Double pointer matrix.
 * @param end Sets the dimensions of the matrix.
 * @param all_words_eng Number of words in basic English language generated by 
 * the algorithm.
 * @return void.
 */
void gen_text(registry *ptr_1, 
              FILE *file, int **matrix, int end, int all_words_eng) {
    int j = 0;
    int k = 0;
    int l = 1;
    int m = rand() % (23 - end + 1) + 23;
    int previous_word = 0;
    int flag = 0;
    float random = 1.0, sum = 0.0;
    registry ptr_2;

    while (k < all_words_eng) {
        if (matrix[m][end] > 0) {
            random = drand48();
            l = 0;

            for (j = 0; (j < end) && (sum <= 1.0); j++) {
                sum += ((float) matrix[m][j] / (float) matrix[m][end]);
                if (random >= sum) {
                    l++;
                }
            }
            /* Check if the word is different to previous one */
            if (ptr_1[l].str_char != ptr_1[previous_word].str_char) {
                /* Convert "i" to "I" */
                if (strncmp(ptr_1[l].str_char, "i", 2) == 0) {
                    ptr_1[l].str_char[0] = toupper(ptr_1[l].str_char[0]);
                }
                /* First word of text generated */
                if (k == 0) {
                    strcpy(ptr_2.str_char, ptr_1[l].str_char);
                    ptr_2.str_char[0] = toupper(ptr_2.str_char[0]);
                    printf(" %s", ptr_2.str_char);
                    fprintf(file, "%s", ptr_2.str_char);
                }                    
                /* Text after the first word */
                //if (k > 0)
                else {
                    /* Check if the word is a dot */
                    if (strncmp(ptr_1[l].str_char, ".", 1) == 0) {
                        /* If the previous word is a comma then use the 
                         * preceding word and search again */
                        if (strncmp(ptr_1[previous_word].str_char, 
                                ",", 1) == 0) {
                            previous_word = l;
                            k--;
                        }                            
                        /* It is a point, print "." and turn the flag on
                         * the next word will be printed in capital */
                        else {
                            printf("%s", ptr_1[l].str_char);
                            fprintf(file, "%s", ptr_1[l].str_char);
                            previous_word = l;
                            flag = 1;
                        }
                    }                       
                    /* Check if the word is a comma */
                    else if (strncmp(ptr_1[l].str_char, ",", 1) == 0) {
                        if (strncmp(ptr_1[previous_word].str_char, 
                                ".", 1) == 0) {
                            previous_word = l;
                            k--;
                        } else {
                            printf("%s", ptr_1[l].str_char);
                            fprintf(file, "%s", ptr_1[l].str_char);
                            previous_word = l;
                        }
                    }                       
                    /* Common word */
                    else {
                        /* Last word was a "." and then the next word is printed
                         * in capital, the flag is turned off */
                        if (flag == 1) {
                            strcpy(ptr_2.str_char, ptr_1[l].str_char);
                            ptr_2.str_char[0] = toupper(ptr_2.str_char[0]);
                            printf(" %s", ptr_2.str_char);
                            fprintf(file, " %s", ptr_2.str_char);
                            previous_word = l;
                            flag = 0;
                        }                            
                        /* Next word is printed in lower case */
                        else {
                            printf(" %s", ptr_1[l].str_char);
                            fprintf(file, " %s", ptr_1[l].str_char);
                            previous_word = l;
                        }
                    }
                }
                k++;
                m = l;
            }
            sum = 0.0;
        }            
        /* The trick, same word as the previous one, use a new random number */
        else {
            m = rand() % (end + 1);
        }
    }
    /* ...this is the end */
    printf("...\n\n");
    return;
}

/**
 * @brief Save values in the probability matrix "<Martriz.csv>" file.
 *
 * @param matrix Double pointer matrix.
 * @param end Value that sets the dimensions of the matrix.
 * @param file File to store words.
 * @return void.
 */
void save_matrix(int **matrix, int end, FILE *file) {
    int i = 0;
    int j = 0;

    for (i = 0; i <= end; i++) {
        for (j = 0; j <= end; j++) {
            fprintf(file, "%i,", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    return;
}

/**
 * @brief Frees memory allocated to store values in the array.
 * 
 * @param matrix Double pointer matrix.
 * @param end Value that sets the dimensions of the matrix.
 * @return void.
 */
void free_matrix(int **matrix, int end) {
    while (end >= 0) {
        free(matrix[end--]);
    }
    
    if ( matrix )
    {
        free(matrix);
        matrix = NULL;
    }
    return;
}

/**
 * @brief Stochastic matrix is created.
 *
 * @param matrix Double pointer matrix.
 * @param end Value that sets the dimensions of the matrix.
 * @return void.
 */
void stochastic_matrix(int **matrix, int end) {
    int i = 0;
    int j = 0;
    int total = 0;

    for (i = 0; i < end; i++) {
        for (j = 0; j < end; j++) {
            total += matrix[i][j];
        }
        matrix[i][end] = total;
        total = 0;
    }
    return;
}

/**
 * @brief Frees memory allocated to the word list.
 *
 * @param ptr_1 Pointer to word list in English language basics.
 * @return void.
 */
void free_malloc(registry *ptr1) {
    if ( ptr1 ){
        free(ptr1);
        ptr1 = NULL;
    }
    return;
}

/**
 * @brief Close all text files used.
 *
 * @param file_1 File with common words in English language basics.
 * @param file_2 Target text.
 * @param file_3 New text file to save the generated text.
 * @param file_4 File to save unfamiliar words.
 * @param file_5 File to store values of the probability matrix.
 * @return void.
 */
void close_files(FILE *file_1, 
        FILE *file_2, FILE *file_3, FILE *file_4, FILE *file_5) {
    if ((fclose(file_1) != 0) && (file_1 != NULL)) {
        p_error("Closing the file with basic English language words");
    }
    if ((fclose(file_2) != 0) && (file_2 != NULL)) {
        p_error("Closing the file with the target text");
    }
    if ((fclose(file_3) != 0) && (file_3 != NULL)) {
        p_error("Closing the file with generated text");
    }
    if ((fclose(file_4) != 0) && (file_4 != NULL)) {
        p_error("Closing the file with unfamiliar words");
    }
    if ((fclose(file_5) != 0) && (file_5 != NULL)) {
        p_error("Closing the file to store values of the probability matrix");
    }
    return;
}

/**
 * @brief The matrix is created to store instances of the i-th and j-th word.
 *
 * @param end Value that sets the dimensions of the matrix.
 * @return Double pointer matrix.
 */
int **new_matrix(int end) {
    int i = 0;
    int **ptr_1 = NULL;
    
    ptr_1 = (int **) calloc(end + 1, sizeof (int *));

    if (ptr_1 != NULL) {
        for (i = 0; i <= end; i++) {
            ptr_1[i] = (int *) calloc(end + 1, sizeof (int));
        }
    } else {
        free(ptr_1);
        ptr_1 = NULL;
        p_error("Unable to create the matrix");
    }
    return ( ptr_1 );
}

/**
 * @brief Compare two words to determine if they match.
 *
 * @param ptr_1 Pointer to the first word.
 * @param ptr_2 Pointer to the second word.
 * @return Greater, equal, or less than zero.
 */
int compare_words(registry *ptr_1, registry *ptr_2) {
    return ( strcmp(ptr_1->str_char, ptr_2->str_char) );
}

/**
 * @brief Copy words from a text file.
 *
 * @param ptr_1 Apuntador a lista de palabra e índice.
 * @param file File to load word list.
 * @return Total number of words in the list.
 */
int load_words(registry *ptr_1, FILE *file) {
    int total = 0;

    while (!feof(file)) {
        if (ptr_1 != NULL) {
            fscanf(file, "%s", ptr_1[total++].str_char);
            ptr_1[total].str_int = total;
        }
    }
    return ( total );
}

/**
 * @brief Look for matching between words of the target text and the list of 
 * basic words of English.
 * 
 * @param file_2 File that contains the target text.
 * @param file_4 File where not found words are stored.
 * @param matrix Matrix where the numerical values of the matches found 
 * are stored.
 * @param size_list_1 Size of the list of words in English language basics.
 * @param list_1 Structure where the word is compared with the list of words in 
 * English language.
 * @return void.
 */
void coincidences(FILE *file_2, FILE *file_4, int **matrix, int size_list_1, registry *list_1) {
    registry *found_1 = NULL;
    registry *found_2 = NULL;
    registry search_1;
    registry search_2;
    registry temp_1;
    registry temp_2;
    size_t   length;

    while (!feof(file_2)) {
        fseek(file_2, -(strlen(temp_2.str_char)), SEEK_CUR);

        fscanf(file_2, "%s", temp_1.str_char);
        search_1 = check_word(temp_1);
        found_1 = bsearch(&search_1, list_1, size_list_1, sizeof (registry), (compare) compare_words);

        fscanf(file_2, "%s", temp_2.str_char);
        search_2 = check_word(temp_2);
        found_2 = bsearch(&search_2, list_1, size_list_1, sizeof (registry), (compare) compare_words);

        if ((found_1) && (found_2)) {
            matrix[found_1->str_int][found_2->str_int]++;

            length = strlen(temp_1.str_char);
            if (strncmp(&temp_1.str_char[length - 1], ".", 1) == 0) {
                matrix[found_1->str_int][2]++;
            }
            if (strncmp(&temp_1.str_char[length - 1], ",", 1) == 0) {
                matrix[found_1->str_int][1]++;
            }
            if (strncmp(&temp_1.str_char[length - 1], ";", 1) == 0) {
                matrix[found_1->str_int][0]++;
            }
        } else if (!found_1) {
            fprintf(file_4, "%s \n", temp_1.str_char);
        }
    }
    return;
}