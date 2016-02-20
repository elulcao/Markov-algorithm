/**
 * @file main.c
 * @brief Text generator algorithm for basic English language
 * @author Luis Daniel Carvallo González
 * @version 1.0
 * @date 27/11/2012
 */

#include "Utilities.h"

/**
 * @brief main.
 *
 * The main function with the variable declaration starts,
 * then the files that are used to read and store validated the words used.
 * If the open file is correct begin charging the words to be used to generate a
 * new text in memory. From the size of the list loaded into memory the size of 
 * it is obtained to create an array. The list of words that are used to 
 * generate text can not be ordered as the "qsort" function the order of the 
 * list for matching is ensured. When the new value of the word list is obtained
 * the square matrix where the occurrences of each word and the next will be 
 * stored is created. When there is a writing pattern is created creates a 
 * probability matrix, where the sum of each of the elements of each row of the 
 * matrix is ​​equal to 1. Values ​​with probability matrix text is created using 
 * the "gen_text" function. With the "gen_text" feature new text that is stored 
 * in the "new_text.txt" file is created. When you have generated the text
 * memory used to store the list of words and probability matrix is ​​released.
 * Finally all the files used by the program are closed.
 * 
 * @return An integer equal to zero on completion of the program correctly.
 */

int main(int argc, const char *argv[]) {
    int size_list_1 = 0;
    int **matrix = NULL;
    int all_words_eng = 0;
    registry *list_1 = NULL;
    FILE *file_1 = NULL;
    FILE *file_2 = NULL;
    FILE *file_3 = NULL;
    FILE *file_4 = NULL;
    FILE *file_5 = NULL;

    srand(getpid());
    srand48(getpid());

    if (argc != 7) {
        printf("Use: ./main <size> <list.txt> <objetive_text.txt> "
               "<new_text.txt> <uknown_words.txt> <matrix>.csv");
        exit(EXIT_FAILURE); 
    }

    sscanf(argv[1], "%d", &all_words_eng);

    file_1 = fopen(argv[2], "r");
    if (!file_1) {
        close_files(file_1, file_2, file_3, file_4, file_5);
        p_error("File with list of words");
    }
    file_2 = fopen(argv[3], "r");
    if (!file_2) {
        close_files(file_1, file_2, file_3, file_4, file_5);
        p_error("Target text file");
    }
    file_3 = fopen(argv[4], "w");
    if (!file_3) {
        close_files(file_1, file_2, file_3, file_4, file_5);
        p_error("New text file");
    }
    file_4 = fopen(argv[5], "w");
    if (!file_4) {
        close_files(file_1, file_2, file_3, file_4, file_5);
        p_error("File to store words not found");
    }
    file_5 = fopen(argv[6], "w");
    if (!file_5) {
        close_files(file_1, file_2, file_3, file_4, file_5);
        p_error("File to store values of probability matrix");
    }

    list_1 = (registry *) malloc(sizeof (registry));
    if (list_1 == NULL) {
        close_files(file_1, file_2, file_3, file_4, file_5);
        p_error("Unable to create word list English language basics");
    }

    size_list_1 = load_words(list_1, file_1);
    matrix = new_matrix(size_list_1);

    qsort(list_1, size_list_1, sizeof (registry), (compare) compare_words);

    coincidences(file_2, file_4, matrix, size_list_1, list_1);
    stochastic_matrix(matrix, size_list_1);
    save_matrix(matrix, size_list_1, file_5);
    gen_text(list_1, file_3, matrix, size_list_1, all_words_eng);
    free_matrix(matrix, size_list_1);
    free_malloc(list_1);
    close_files(file_1, file_2, file_3, file_4, file_5);

    return ( EXIT_SUCCESS);
}
