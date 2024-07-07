%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

double frac(int num, int denom)
{
   return (double)num/denom;
}

char* remove_brackets(char *str) {
  int len = strlen(str);
  if (str[0] == '[') {
    for (int i = 0; i < len - 2; i++) {
      str[i] = str[i+1];
    }
    str[len-2] = '\0';
  }
return str;
}

extern "C" int fileno(FILE *stream);

%}

%%

-?[0-9]+(\.[0-9]+)?              { fprintf(stderr, "Number : %s\n", yytext); 
                                 
                                 yylval.numberValue = std::stof(yytext);
                                 return Number; }



-?[0-9]+\/[0-9]+               { fprintf(stderr, "Fraction : %s\n", yytext); 
                                 
                                 int num = std::stoi(strtok(yytext, "/"));
                                 int denom = std::stoi(strtok(NULL, "/"));
                                 double fraction = frac(num, denom);
                                 yylval.numberValue = fraction;
                                 
                                 return Number; }

[a-zA-Z]+|\[([^\]]*)\]          { fprintf(stderr, "Word : %s\n", yytext); 
                                 
                                 yylval.wordValue = new std::string(remove_brackets(yytext));  
                                 return Word; }

\n             { fprintf(stderr, "Newline\n"); 
               }

[ ]            

.              


%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
  exit(1);
}
