#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
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
void print_title(char* msg);
void print_info(char* msg);
void print_success(char* msg);
void print_warn(char* warning, int warning_code);
void print_err(char* err, int err_code);
void print_err_and_exit(char* err, int err_code);