#include "def.h"
#include "parser.tab.h"

SYMBOLTABLE new_table={{0},0};
SYMBOL_SCOPE_TX new_scope={{0},0};


struct ASTNode * mknode(int num,int kind,int pos,...){
    struct ASTNode *T=(struct ASTNode *)calloc(sizeof(struct ASTNode),1);
    int i=0;
    T->kind=kind;
    T->pos=pos;
    va_list pArgs;			//用于获取不确定个数的参数
    va_start(pArgs, pos);		//pArgs用来存储额外的参数信息
    for(i=0;i<num;i++)
        T->ptr[i]= va_arg(pArgs, struct ASTNode *);
    while (i<4) T->ptr[i++]=NULL;
    va_end(pArgs);
    return T;
}


void display(struct ASTNode *T,int indent)
{//对抽象语法树的先根遍历
  int i=1;
  struct ASTNode *T0;
  if (T)
	{
	switch (T->kind) {
	case EXT_DEF_LIST:  display(T->ptr[0],indent);    //显示该外部定义（外部变量和函数）列表中的第一个
                        display(T->ptr[1],indent);    //显示该外部定义列表中的其它外部定义
                        break;
	case EXT_VAR_DEF:   printf("%*c外部变量定义：(%d)\n",indent,' ',T->pos);
                        display(T->ptr[0],indent+3);        //显示外部变量类型
                        printf("%*c变量名：\n",indent+3,' ');
                        display(T->ptr[1],indent+6);        //显示变量列表
                        break;
	case TYPE:          printf("%*c类型： %s\n",indent,' ',T->type_id);
                        break;
        case EXT_DEC_LIST:  display(T->ptr[0],indent);     //依次显示外部变量名，
                        display(T->ptr[1],indent);     //后续还有相同的，仅显示语法树此处理代码可以和类似代码合并
                        break;
	case FUNC_DEF:      printf("%*c函数定义：(%d)\n",indent,' ',T->pos);
                        display(T->ptr[0],indent+3);      //显示函数返回类型
                        display(T->ptr[1],indent+3);      //显示函数名和参数
                        display(T->ptr[2],indent+3);      //显示函数体
                        break;
	case FUNC_DEC:      printf("%*c函数名：%s\n",indent,' ',T->type_id);
                        if (T->ptr[0]) {
                                printf("%*c函数形参：\n",indent,' ');
                                display(T->ptr[0],indent+3);  //显示函数参数列表
                                }
                        else printf("%*c无参函数\n",indent+3,' ');
                        break;
	case PARAM_LIST:    display(T->ptr[0],indent);     //依次显示全部参数类型和名称，
                        display(T->ptr[1],indent);
                        break;
	case PARAM_DEC:     printf("%*c类型：%s, 参数名：%s\n",indent,' ',T->ptr[0]->type==INT?"int":"float",T->ptr[1]->type_id);
                        break;
	case EXP_STMT:      printf("%*c表达式语句：(%d)\n",indent,' ',T->pos);
                        display(T->ptr[0],indent+3);
                        break;
	case RETURN:        printf("%*c返回语句：(%d)\n",indent,' ',T->pos);
                        display(T->ptr[0],indent+3);
                        break;
	case COMP_STM:      printf("%*c复合语句：(%d)\n",indent,' ',T->pos);
                        printf("%*c复合语句的变量定义部分：\n",indent+3,' ');
                        display(T->ptr[0],indent+6);      //显示定义部分
                        printf("%*c复合语句的语句部分：\n",indent+3,' ');
                        display(T->ptr[1],indent+6);      //显示语句部分
                        break;
	case STM_LIST:      display(T->ptr[0],indent);      //显示第一条语句
                        display(T->ptr[1],indent);        //显示剩下语句
                        break;
	case WHILE:         printf("%*c循环语句WHILE：(%d)\n",indent,' ',T->pos);
                        printf("%*c循环条件：\n",indent+3,' ');
                        display(T->ptr[0],indent+6);      //显示循环条件
                        printf("%*c循环体：(%d)\n",indent+3,' ',T->pos);
                        display(T->ptr[1],indent+6);      //显示循环体
                        break;
	case FOR:         printf("%*c循环语句FOR：(%d)\n",indent,' ',T->pos);
                        printf("%*c定义部分：\n",indent+3,' ');
                        display(T->ptr[0],indent+6);      //显示循环定义
                        printf("%*c循环条件：(%d)\n",indent+3,' ',T->pos);
                        display(T->ptr[1],indent+6);      //显示循环条件
			printf("%*c运算部分：\n",indent+3,' ');
                        display(T->ptr[2],indent+6);      //显示循环运算部分
                        printf("%*c循环体：(%d)\n",indent+3,' ',T->pos);
                        display(T->ptr[3],indent+6);      //显示循环体
                        break;	
	case IF_THEN:       printf("%*c条件语句(IF_THEN)：(%d)\n",indent,' ',T->pos);
                        printf("%*c条件：\n",indent+3,' ');
                        display(T->ptr[0],indent+6);      //显示条件
                        printf("%*cIF子句：(%d)\n",indent+3,' ',T->pos);
                        display(T->ptr[1],indent+6);      //显示if子句
                        break;
	case IF_THEN_ELSE:  printf("%*c条件语句(IF_THEN_ELSE)：(%d)\n",indent,' ',T->pos);
                        printf("%*c条件：\n",indent+3,' ');
                        display(T->ptr[0],indent+6);      //显示条件
                        printf("%*cIF子句：(%d)\n",indent+3,' ',T->pos);
                        display(T->ptr[1],indent+6);      //显示if子句
                        printf("%*cELSE子句：(%d)\n",indent+3,' ',T->pos);
                        display(T->ptr[2],indent+6);      //显示else子句
                        break;
    case DEF_LIST:      display(T->ptr[0],indent);    //显示该局部变量定义列表中的第一个
                        display(T->ptr[1],indent);    //显示其它局部变量定义
                        break;
    case VAR_DEF:       printf("%*c局部变量定义：(%d)\n",indent,' ',T->pos);
                        display(T->ptr[0],indent+3);   //显示变量类型
                        display(T->ptr[1],indent+3);   //显示该定义的全部变量名
                        break;
    case DEC_LIST:      printf("%*c变量名：\n",indent,' ');
                        T0=T;
                        while (T0) {
                            if (T0->ptr[0]->kind==ID)
                                printf("%*c %s\n",indent+6,' ',T0->ptr[0]->type_id);
			    else if(T0->ptr[0]->kind==ARRAY){
				display(T0->ptr[0],indent);
				}
                            else if (T0->ptr[0]->kind==ASSIGNOP)
                                {
                                printf("%*c %s ASSIGNOP\n ",indent+6,' ',T0->ptr[0]->ptr[0]->type_id);
                                display(T0->ptr[0]->ptr[1],indent+strlen(T0->ptr[0]->ptr[0]->type_id)+7);        //显示初始化表达式
                                }
                            T0=T0->ptr[1];
                            }
                        break;
	case ID:	        printf("%*cID： %s\n",indent,' ',T->type_id);
                        break;
	case INT:	        printf("%*cINT：%d\n",indent,' ',T->type_int);
                        break;
	case FLOAT:	        printf("%*cFLOAT：%f\n",indent,' ',T->type_float);
                        break;
	case CHAR:              printf("%*cCHAR:  %c\n",indent,' ',T->type_char);
			break;
	case STRING:	        printf("%*cSTRING： %s\n",indent,' ',T->type_string);
                        break;
	case BREAK:		printf("%*cBREAK\n",indent,' ');
			break;
	case CONTINUE:		printf("%*cCONTINUE \n",indent,' ');
			break;
	case ARRAY:		printf("%*c数组",indent,' ');
				display(T->ptr[0],0);
				display(T->ptr[1],indent);
			break;
	case ARRAYSIZE:		printf("%*c大小",indent,' ');
				display(T->ptr[0],0);
				display(T->ptr[1],indent);
			break;
	case LASTDPLUS:		printf("%*c后自增",indent,' ',T->pos);
				display(T->ptr[0],0);
			break;
	case FIRSTDPLUS:	printf("%*c先自增",indent,' ',T->pos);
				display(T->ptr[0],0);
			break;
	case LASTDMINUS:	printf("%*c后自减",indent,' ',T->pos);
				display(T->ptr[0],0);
			break;
	case FIRSTDMINUS:	printf("%*c先自减",indent,' ',T->pos);
				display(T->ptr[0],0);
			break;
	case ASSIGNOP:
	case AND:
	case OR:
	case RELOP:
	case PLUS:
	case MINUS:
	case STAR:
	case DIV:
                    printf("%*c%s\n",indent,' ',T->type_id);
                    display(T->ptr[0],indent+3);
                    display(T->ptr[1],indent+3);
                    break;
	case NOT:
	case UMINUS:    printf("%*c%s\n",indent,' ',T->type_id);
                    display(T->ptr[0],indent+3);
                    break;
        case FUNC_CALL: printf("%*c函数调用：(%d)\n",indent,' ',T->pos);
                    printf("%*c函数名：%s\n",indent+3,' ',T->type_id);
                    display(T->ptr[0],indent+3);
                    break;
	case ARGS:      i=1;
                    while (T) {  //ARGS表示实际参数表达式序列结点，其第一棵子树为其一个实际参数表达式，第二棵子树为剩下的
                        struct ASTNode *T0=T->ptr[0];
                        printf("%*c第%d个实际参数表达式：\n",indent,' ',i++);
                        display(T0,indent+3);
                        T=T->ptr[1];
                        }
//                    printf("%*c第%d个实际参数表达式：\n",indent,' ',i);
  //                  display(T,indent+3);
                    printf("\n");
                    break;
         }
      }
}



void DisplaySymbolTable(){
	
	int i;
	
	for(i=0;i<new_table.index;i++){
		printf("%d\t",i);
		printf("%s\t",new_table.symbols[i].name);
		printf("%d\t",new_table.symbols[i].level);
		if(new_table.symbols[i].type==INT)
				printf("int\t");
		else if(new_table.symbols[i].type==FLOAT)
				printf("float\t");
		else
				printf("char\t");
		printf("%c\t",new_table.symbols[i].flag);
		if(new_table.symbols[i].flag=='F')
				printf("%d\n",new_table.symbols[i].paramnum);
		else
				printf("\n");
	
	}
	printf("------------------------------------------------------\n\n");

}

int i,j,counter=0,t;
int isLoop=0;


int Semantic_Analysis(struct ASTNode* T,int type,int level,char flag,int command){
	int type1,type2;
	int indexX=0;
	if(T){
		switch(T->kind){
				case EXT_DEF_LIST:		//外部声明列表
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case EXT_VAR_DEF:		//外部定义声明
						flag='V';
						type=Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case ARRAY:
						Semantic_Analysis(T->ptr[0],type,level,'A',command);
						break;
				case TYPE:
						return T->type;
						break;
				case EXT_DEC_LIST:
						
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case ID:
						i=0;
						while(new_table.symbols[i].level!=level&&i<new_table.index)//转到相同作用域
								i++;
						if(command==0)
						{
							while(i<new_table.index)
							{
							if(strcmp(new_table.symbols[i].name,T->type_id)==0&&new_table.symbols[i].flag==flag)
							{
									if(flag=='V')
											printf("ERROR! 第%d行:相同全局变量名称%s\n",T->pos,T->type_id);
									else if(flag=='T')
											printf("ERROR! 第%d行:相同局部变量名称%s\n",T->pos,T->type_id);
									else if(flag=='F')
											printf("ERROR! 第%d行:相同函数名称%s\n",T->pos,T->type_id);
									else 
											printf("ERROR! Same function parameter %s at line %d\n",T->type_id,T->pos);
									return 0;
							}
						i++;
							}
							strcpy(new_table.symbols[new_table.index].name,T->type_id);
							new_table.symbols[new_table.index].level=level;
							new_table.symbols[new_table.index].type=type;
							new_table.symbols[new_table.index].flag=flag;
							printf("%d\t",new_table.index);
							printf("%s\t",new_table.symbols[new_table.index].name);
							printf("%d\t",new_table.symbols[new_table.index].level);
							if(new_table.symbols[new_table.index].type==INT)
								printf("int\t");
							else if(new_table.symbols[new_table.index].type==FLOAT)
								printf("float\t");
							else
								printf("char\t");
							printf("%c\t",new_table.symbols[new_table.index].flag);
						
							printf("\n");
							new_table.index++;
						}
						else{
							i=new_table.index-1;
							while(i>=0){
								if(strcmp(new_table.symbols[i].name,T->type_id)==0&&(new_table.symbols[i].flag=='V'||new_table.symbols[i].flag=='T')){
								return new_table.symbols[i].type;
								}
							i--;
							}
						if(i<0){
							printf("ERROR! 第%d行:变量%s没有定义 \n",T->pos,T->type_id);
							}
						}
						break;
				case FUNC_DEF:
						type=Semantic_Analysis(T->ptr[0],type,level+1,flag,command);
						Semantic_Analysis(T->ptr[1],type,1,flag,command);
						Semantic_Analysis(T->ptr[2],type,1,flag,command);
						break;
				case FUNC_DEC:
						strcpy(new_table.symbols[new_table.index].name,T->type_id);
						new_table.symbols[new_table.index].level=0;
						new_table.symbols[new_table.index].type=type;
						new_table.symbols[new_table.index].flag='F';
					
						new_table.index++;
						counter=0;
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						new_table.symbols[new_table.index-counter-1].paramnum=counter;
						break;
				case PARAM_LIST:
						counter++;
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case PARAM_DEC:
						flag='P';
						type=Semantic_Analysis(T->ptr[0],type,level+1,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case COMP_STM:
						printf("\t\t***%s***\n",new_table.symbols[new_table.index-1-counter].name);
						//printf("\t\t***Symbol Table***\n");			
    						printf("---------------------------------------------------\n");
    						printf("%s\t%s\t%s\t%s\t%s\t%s\n","Index","Name","Level","Type","Flag","Param_num");
    						printf("---------------------------------------------------\n");
						flag='T';
						command=0;
						new_scope.TX[new_scope.top]=new_table.index;
						new_scope.top++;
						Semantic_Analysis(T->ptr[0],type,level,flag,command);	//分析局部定义部分
						command=1;
						Semantic_Analysis(T->ptr[1],type,level+1,flag,command);	//分析语句列表
						new_table.index=new_scope.TX[new_scope.top-1];
						new_scope.top--;
						printf("\n\n");
						if(new_scope.top==0)
								DisplaySymbolTable();
						break;
				case DEF_LIST:
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case VAR_DEF:
						type=Semantic_Analysis(T->ptr[0],type,level+1,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case DEC_LIST:
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case STM_LIST:
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case EXP_STMT:
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						break;
				case IF_THEN:
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						break;
				case WHILE:
					
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						
						break;
				case FOR:
				case IF_THEN_ELSE:
						Semantic_Analysis(T->ptr[0],type,level,flag,command);
						Semantic_Analysis(T->ptr[1],type,level,flag,command);
						Semantic_Analysis(T->ptr[2],type,level,flag,command);
						break;
				case ASSIGNOP:
				case OR:
				case AND:
				case RELOP:
				case PLUS:
				case MINUS:
				case STAR:
				case DIV:
				case DPLUS:
				case BREAK:
					
				case DMINUS:
					type1=Semantic_Analysis(T->ptr[0],type,level,flag,command);
					type2=Semantic_Analysis(T->ptr[1],type,level,flag,command);
					if(type1==type2)
							return type1;
					break;
				case INT:
					return INT;
				case FLOAT:
					return FLOAT;
				case CHAR:
					return CHAR;
				case FUNC_CALL:
					j=0;
					while(new_table.symbols[j].level==0&&j<new_table.index){
						if(strcmp(new_table.symbols[j].name,T->type_id)==0){
							if(new_table.symbols[j].flag!='F')
								printf("ERRO! 第%d行:函数名%s 在符号表中的定义为变量\n",T->pos,T->type_id);
							break;
						}
						j++;
					}
					if(new_table.symbols[j].level==1||j==new_table.index){
						printf("ERROR! 第%d 行：函数%s未定义\n",T->pos,T->type_id);
						break;
					}
					type=new_table.symbols[j+1].type;
					counter=0;
					Semantic_Analysis(T->ptr[0],type,level,flag,command);
					if(new_table.symbols[j].paramnum!=counter)
							printf("ERROR!第%d行：函数调用%s参数个数不匹配\n",T->pos,T->type_id);
					break;
				case ARGS:
					counter++;
					t=Semantic_Analysis(T->ptr[0],type,level,flag,command);
					if(type!=t)
							printf("ERROR!第%d行：函数调用的第%d 个参数类型不匹配\n",T->pos,counter);
					type=new_table.symbols[j+counter+1].type;
					Semantic_Analysis(T->ptr[1],type,level,flag,command);
					break;


		}

	}
		return 0;
}

