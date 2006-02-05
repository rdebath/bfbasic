#include "ast.h"
#include "y.tab.h"

struct file *ast_root;

int main()
{
    if (yyparse() == 0) {
        disp_file(1, ast_root);
    }
}
