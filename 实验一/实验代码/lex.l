%{
#include "parser.tab.h"
#include "string.h"
#include "def.h"
int yycolumn=1;
#define YY_USER_ACTION    	yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;	yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;
typedef union {
	int type_int;
	char type_char[3];
	float type_float;
	char type_id[32];
	char type_string[100];
	struct node *ptr;
} YYLVAL;
#define YYSTYPE YYLVAL

%}
%option yylineno
comment (\/\/.*\n)|(\/\*.*\*\/)
id   [A-Za-z][A-Za-z0-9]*  
int    [0-9]+
char	'[^']'
float  ([0-9]*\.[0-9]+)|([0-9]+\.)
string \"(\\.|[^"\\])*\"
%%
{int}        {printf("columm=%d   int常量 %s \n",yylineno,yytext);yylval.type_int=atoi(yytext); return INT;}
{float}      {printf("columm=%d   float常量 %s \n",yylineno,yytext);yylval.type_float=atof(yytext); return FLOAT;}
{char}	     {printf("columm=%d   char常量 %s \n",yylineno,yytext);strcpy(yylval.type_char, yytext);return CHAR;}
{string}     {printf("columm=%d   string常量 %s \n",yylineno,yytext);strcpy(yylval.type_string,  yytext); return STRING;}
{comment}    {
	printf("this is comment.\n");
	for(int i=0;i<yyleng;i++){
		if(yytext[i]!="\n")
			printf("%c",yytext[i]);
		else
			break;
	}
	printf("\n");
}
"int"        {printf("columm=%d   标示符　%s \n",yylineno,yytext);strcpy(yylval.type_id,  yytext);return TYPE;}
"float"      {printf("columm=%d   标示符　%s \n",yylineno,yytext);strcpy(yylval.type_id,  yytext);return TYPE;}
"char"	     {printf("columm=%d   标示符　%s \n",yylineno,yytext);strcpy(yylval.type_id,  yytext);return TYPE;}
"string"     {printf("columm=%d   标示符　%s \n",yylineno,yytext);strcpy(yylval.type_id,  yytext);return TYPE;}

"return"     {printf("columm=%d   关键字　%s \n",yylineno,yytext);return RETURN;}
"if"         {printf("columm=%d   关键字　%s \n",yylineno,yytext);return IF;}
"else"       {printf("columm=%d   关键字　%s \n",yylineno,yytext);return ELSE;}
"while"      {printf("columm=%d   关键字　%s \n",yylineno,yytext);return WHILE;}
"for"        {printf("columm=%d   关键字　%s \n",yylineno,yytext);return FOR;}
"break"	     {printf("columm=%d   关键字　%s \n",yylineno,yytext);return BREAK;}
"continue"   {printf("columm=%d   关键字　%s \n",yylineno,yytext);return CONTINUE;}


{id}           {printf("columm=%d   变量或函数名称　%s \n",yylineno,yytext);strcpy(yylval.type_id,  yytext); return ID;/*由于关键字的形式也符合标识符的规则，所以把关键字的处理全部放在标识符的前面，优先识别*/}
{int}{id}	{printf("columm=%d   变量命名错误　%s \n",yylineno,yytext);strcpy(yylval.type_id,  yytext);}
";"			{printf("columm=%d   分号%s \n",yylineno,yytext);return SEMI;}
","			{printf("columm=%d   逗号%s \n",yylineno,yytext);return COMMA;}
">"|"<"|">="|"<="|"=="|"!=" {printf("columm=%d   逻辑运算符%s \n",yylineno,yytext);strcpy(yylval.type_id, yytext);;return RELOP;}
"="			{printf("columm=%d   运算符　%s \n",yylineno,yytext);return ASSIGNOP;}
"++"            	{printf("columm=%d   运算符　%s \n",yylineno,yytext);return DPLUS;}
"+"			{printf("columm=%d   运算符　%s \n",yylineno,yytext);return PLUS;}
"--"			{printf("columm=%d   运算符　%s \n",yylineno,yytext);return DMINUS;}
"-"			{printf("columm=%d   运算符　%s \n",yylineno,yytext);return MINUS;}
"*"			{printf("columm=%d   运算符　%s \n",yylineno,yytext);return STAR;}
"/"			{printf("columm=%d   运算符　%s \n",yylineno,yytext);return DIV;}
"&&"        		{printf("columm=%d   运算符　%s \n",yylineno,yytext);return AND;}
"||"            	{printf("columm=%d   运算符　%s \n",yylineno,yytext);return OR;}
"!"			{printf("columm=%d   运算符　%s \n",yylineno,yytext);return NOT;}
"("			{printf("columm=%d   LP %s \n",yylineno,yytext);return LP;}
")"			{printf("columm=%d   RP %s \n",yylineno,yytext);return RP;}
"{"			{printf("columm=%d   LC %s \n",yylineno,yytext);return LC;}
"}"			{printf("columm=%d   RC %s \n",yylineno,yytext);return RC;}
"["	     		{printf("columm=%d   LB %s \n",yylineno,yytext);return LB;}
"]"			{printf("columm=%d   RB %s \n",yylineno,yytext);return RB;}
[\n]			{yycolumn=1;}
[ \r\t]			{}
.			{printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}

%%

/* 和bison联用时，不需要这部分
void main()
{
yylex();
return 0;
}

*/
int yywrap()
{
return 1;
}
