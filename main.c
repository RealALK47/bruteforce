#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* BRUTE FORCER                    */
/* advanced main thread bruteforce */

void compute(int max_len, char *symbols)
{
    int symbols_len = strlen(symbols);
    int len = 0;
    char *buf = malloc((max_len + 1) * symbols_len * symbols_len);
    int *letters = malloc(max_len * sizeof(int));
    
    // Generates all current number of letter patterns
    // up to the given max_len.
    for (len = 1; len <= max_len; len++) {
        int i;
        int stride = len + 1;
        int buf_len = stride * symbols_len * symbols_len;

        if (len == 1) {
            // Handled seperately because the last two letters are hardcoded.
            int j = 0;
            buf_len = (len + 1) * symbols_len;

            for (i = 0; i < symbols_len; i++) {
                buf[j++] = symbols[i];
                buf[j++] = '\n';
            }
            
            write(STDOUT_FILENO, buf, buf_len);
            continue;
        }

        // Initialize buffer to contain all first letters.
        memset(buf, symbols[0], buf_len);

        // Writes the last two letters and newlines that will not change.
        {
            int let0 = 0;
            int let1 = 0;
            for (i = len -2; i < buf_len; i += stride) {
                buf[i] = symbols[let0];
                buf[i+1] = symbols[let1++];
                buf[i+2] = '\n';
                if (let1 == symbols_len) {
                    let1 = 0;
                    let0++;
                    if (let0 == symbols_len)
                    let0 = 0;
                }
            }
        }

        write(STDOUT_FILENO, buf, buf_len);

        if (len == 2)
            continue;
        
        // Sets all letters to 0.
        for (i = 0; i < len; i++)
            letters[i] = 0;
        
        i = len -3;
        do {
            char c;
            int  j;
            letters[i]++;
            if (letters[i] >= symbols_len)
                letters[i] = 0;
            
            c = symbols[letters[i]];
            for (j = i; j < buf_len; j += stride)
                buf[j] = c;

            if (letters[i] != 0) {
                write(STDOUT_FILENO, buf, buf_len);
                i = len - 3;
                continue;
            }

            i--;
            if (i < 0)
                break;
        } while(1);
    }
    
    free(letters);
    free(buf);
}

int main() {
    int len = 3;
    char *symbols = "abc123";

    compute(len, symbols);
    return 0;
}
