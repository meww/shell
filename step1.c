#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

int main(void)
{
    char str[MAX];
    char check[MAX];
    uint8_t count = 0;
    while (1) {
        printf("mysh $ ");
        fgets(str, MAX, stdin);
        printf("%s", str);
    }
    return 0;
}
