#include <stdio.h>

int main()
{
    int c;
    
    printf(">");
    
    while ((c = getchar()) != EOF) {
        switch (c) {
            case '>':
                printf(">>>");
                break;
                
            case '<':
                printf("<<<");
                break;
                
            case ',':
                printf(",");
                break;
                
            case '.':
                printf(".");
                break;
                
            case '+':
                printf("+[<+>>>+<<-]<[>+<-]+>>>[<<<->>>[-]]<<<[->>+<<]>");
                break;
                
            case '-':
                printf("[<+>>>+<<-]<[>+<-]+>>>[<<<->>>[-]]<<<[->>-<<]>-");
                break;
                
            case '[':
                printf("[>>+>>>+<<<<<-]>>>>>[<<<<<+>>>>>-]<<<[[-]<<<+>>>]<[>+>"
                       ">>+<<<<-]>>>>[<<<<+>>>>-]<<<[[-]<<<+>>>]<<<[[-]>");
                break;
                
            case ']':
                printf("[>>+>>>+<<<<<-]>>>>>[<<<<<+>>>>>-]<<<[[-]<<<+>>>]<[>+>"
                       ">>+<<<<-]>>>>[<<<<+>>>>-]<<<[[-]<<<+>>>]<<<]>");
                break;
                
            case '#':
                printf("#");
                break;
        }
    }
    
    return 0;
}
