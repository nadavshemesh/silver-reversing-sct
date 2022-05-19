
#define MAX_LOCAL_ARR 1024
/* to be used for local var arrays */
typedef struct i_array {
    int arr[MAX_LOCAL_ARR];
    long len;
} i_arr;
typedef struct s_array {
    char *arr[MAX_LOCAL_ARR];
    long len;
} s_arr;

void *w_malloc(size_t size);
char *aapts(char* str);