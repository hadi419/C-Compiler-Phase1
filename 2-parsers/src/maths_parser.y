%code requires{
  #include "ast.hpp"

  #include <cassert>

  extern const Expression *g_root; 

  int yylex(void);
  void yyerror(const char *);
}

%union{
  const Expression *expr;
  double number;
  std::string *string;
}

%token T_TIMES T_DIVIDE T_PLUS T_MINUS T_EXPONENT
%token T_LBRACKET T_RBRACKET
%token T_LOG T_EXP T_SQRT
%token T_NUMBER T_VARIABLE

%type <expr> EXPR TERM UNARY FACTOR
%type <number> T_NUMBER
%type <string> T_VARIABLE T_LOG T_EXP T_SQRT FUNCTION_NAME

%start ROOT 

%%

ROOT : EXPR                                 { g_root = $1; }

EXPR : TERM                                 { $$ = $1; }
     | EXPR T_PLUS TERM                     { $$ = new AddOperator($1, $3); }
     | EXPR T_MINUS TERM                    { $$ = new SubOperator($1, $3); }

TERM : UNARY                                { $$ = $1; }
     | TERM T_TIMES UNARY                   { $$ = new MulOperator($1, $3); }
     | TERM T_DIVIDE UNARY                  { $$ = new DivOperator($1, $3); }

UNARY : FACTOR                              { $$ = $1; }
      | T_MINUS UNARY                       { $$ = new NegOperator($2); }

FACTOR : T_NUMBER                           { $$ = new Number( $1 ); }
       | T_LBRACKET EXPR T_RBRACKET         { $$ = $2; }
       | T_VARIABLE                         { $$ = new Variable(*$1); }
       | T_LOG T_LBRACKET EXPR T_RBRACKET   { $$ = new LogFunction($3); }
       | T_EXP T_LBRACKET EXPR T_RBRACKET   { $$ = new ExpFunction($3); }
       | T_SQRT T_LBRACKET EXPR T_RBRACKET  { $$ = new SqrtFunction($3); }
       | UNARY T_EXPONENT UNARY             { $$ = new ExpOperator($1, $3); }

FUNCTION_NAME : T_LOG                       { $$ = new std::string("log"); }
              | T_SQRT                      { $$ = new std::string("sqrt"); }
              | T_EXP                       { $$ = new std::string("exp"); }

%%

const Expression *g_root; // Definition of variable (to match declaration earlier)

const Expression *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
