/*
 * Dennis Shelton, Agustin Rodriguez, Jack Lea
 *
 * Lex.c
 * March 23rd, 2022
 *
 * This code functions as a lexical analyzer.
 *
 * We made no changes to the source files given.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 1000
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

lexeme *list;
int lex_index;

int alphatoken();
int numbertoken();
int symboltoken();
int comment();
int reservedcheck(char *buffer);
void printlexerror(int type);
void printtokens();


lexeme *lexanalyzer(char *input, int printFlag)
{
	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	int input_index = 0;
  lex_index = 0;

  // Loops through input 
  while (input_index < MAX_NUMBER_TOKENS)
  {
    // if and else if statements for white space & alpha & digit & symbol switch
    
    // ignore comments
    if (input[input_index] == '/' && input[input_index + 1] == '/')
    {
      // place index at first char of comment
      // push index through the entire comment and than move onto first non comment char
      input_index += 2;
      while (input[input_index] != '\0')
        {
          input_index++;
        }
      input_index++;
    }
    // ignores white space
    else if (isspace(input[input_index]) || iscntrl(input[input_index])) 
    {
      input_index++;
    }
    // Numbers
    // if we identify a number, we have to check for identifier errors
    // this includes number -> identifier and no whitespace
    // this also includes number -> word and no whitespace
    // so basically check if there's whitespace after the number
    // numbers can't be longer than 5 digits
    else if (isdigit(input[input_index])) 
    {
      int temp;
      char tempw[5];
      int i = 1;
      temp = input[input_index] - 48;
      tempw[0] = input[input_index];
      input_index++;

      while (isdigit(input[input_index])) 
      {
        if (i > 4) 
        {
          printlexerror(2);
          return NULL;
        }
        temp *= 10;
        temp += input[input_index] - 48;
        tempw[i] = input[input_index];
        i++;
        input_index++;
      }
      if (isalpha(input[input_index]))
        printlexerror(1);
      
      tempw[i + 1] = '\0';
      
      strcpy(list[lex_index].name, tempw);
      list[lex_index].value = temp;
      list[lex_index].type = numbersym;
      lex_index++;
    }
    // Identifiers & Reserved Words
    // reserved words are no longer than 12
    // we error with identifier length
    else if (isalpha(input[input_index])) 
    {
      char temp[12];
      int i = 0;
      
      while(isalpha(input[input_index]) || isdigit(input[input_index])) 
      {
        if (i > 11) 
        {
          printlexerror(3);
          return NULL;
        }
        temp[i] = input[input_index];
        i++;
        input_index++;
      }
      temp[i] = '\0';
      // check for reserved words, reserved words are as follows:
      //var, procedure, call, begin, end, if, do, while, read, write
      if(strcmp(temp, "var") == 0) {
        strcpy(list[lex_index].name, "var");
        list[lex_index].value = 2;
        list[lex_index].type = varsym;
        lex_index++;
      }
      else if(strcmp(temp, "procedure") == 0) {
        strcpy(list[lex_index].name, "procedure");
        list[lex_index].value = 4;
        list[lex_index].type = procsym;
        lex_index++;
      }
      else if(strcmp(temp, "call") == 0) {
        strcpy(list[lex_index].name, "call");
        list[lex_index].value = 6;
        list[lex_index].type = callsym;
        lex_index++;
      }
      else if(strcmp(temp, "begin") == 0) {
        strcpy(list[lex_index].name, "begin");
        list[lex_index].value = 8;
        list[lex_index].type = beginsym;
        lex_index++;
      }
      else if(strcmp(temp, "end") == 0) {
        strcpy(list[lex_index].name, "end");
        list[lex_index].value = 10;
        list[lex_index].type = endsym;
        lex_index++;
      }
      else if(strcmp(temp, "if") == 0) {
        strcpy(list[lex_index].name, "if");
        list[lex_index].value = 12;
        list[lex_index].type = ifsym;
        lex_index++;
      }
      else if(strcmp(temp, "do") == 0) {
        strcpy(list[lex_index].name, "do");
        list[lex_index].value = 14;
        list[lex_index].type = dosym;
        lex_index++;
      }
      else if(strcmp(temp, "while") == 0) {
        strcpy(list[lex_index].name, "while");
        list[lex_index].value = 33;
        list[lex_index].type = whilesym;
        lex_index++;
      }
      else if(strcmp(temp, "read") == 0) {
        strcpy(list[lex_index].name, "read");
        list[lex_index].value = 16;
        list[lex_index].type = readsym;
        lex_index++;
      }
      else if(strcmp(temp, "write") == 0) {
        strcpy(list[lex_index].name, "write");
        list[lex_index].value = 18;
        list[lex_index].type = writesym;
        lex_index++;
      }
      else {
        strcpy(list[lex_index].name, temp);
        list[lex_index].value = 20;
        list[lex_index].type = identsym;
        lex_index++;
      }
    }
    // Symbols & comments
    // created a default that errors for an invalid symbol
    else 
    {
      switch(input[input_index])
      {
        case '.':
          strcpy(list[lex_index].name, ".");
          list[lex_index].value = 1;
          list[lex_index].type = periodsym;
          lex_index++;
          input_index = MAX_NUMBER_TOKENS;
        break;
        
        case '[':
          strcpy(list[lex_index].name, "[");
          list[lex_index].value = 3;
          list[lex_index].type = lbracketsym;
          lex_index++;
        break;
        
        case ']':
          strcpy(list[lex_index].name, "]");
          list[lex_index].value = 5;
          list[lex_index].type = rbracketsym;
          lex_index++;
        break;
        
        case ',':
          strcpy(list[lex_index].name, ",");
          list[lex_index].value = 7;
          list[lex_index].type = commasym;
          lex_index++;
        break;
        
        case ';':
          strcpy(list[lex_index].name, ";");
          list[lex_index].value = 9;
          list[lex_index].type = semicolonsym;
          lex_index++;
        break;

        case ':':
          if (input[input_index+1] == '=') 
          {
            strcpy(list[lex_index].name, ":=");
            list[lex_index].value = 11;
            list[lex_index].type = assignsym;
            input_index++;
            lex_index++;
            break;
          }
          else if (isspace(input[input_index+1]) || iscntrl(input[input_index+1])) 
          {
            strcpy(list[lex_index].name, ":");
            list[lex_index].value = 15;
            list[lex_index].type = colonsym;
            input_index++;
            lex_index++;
            break;
          }
          else
          {
            printlexerror(1);
            return NULL;
          }
        break;

        case '?':
          strcpy(list[lex_index].name, "?");
          list[lex_index].value = 13;
          list[lex_index].type = questionsym;
          lex_index++;
        break;  
          
        case '(':
          strcpy(list[lex_index].name, "(");
          list[lex_index].value = 17;
          list[lex_index].type = lparenthesissym;
          lex_index++;
        break;

        case ')':
          strcpy(list[lex_index].name, ")");
          list[lex_index].value = 19;
          list[lex_index].type = rparenthesissym;
          lex_index++;
        break; 

        case '=':
          if (input[input_index+1] == '=') 
          {
            strcpy(list[lex_index].name, "==");
            list[lex_index].value = 21;
            list[lex_index].type = eqlsym;
            input_index++;
            lex_index++;
            break;
          }
          else
          {
            printlexerror(1);
            return NULL;
          }
        break;
        
        case '<':
          if (input[input_index+1] == '>') 
          {
            strcpy(list[lex_index].name, "<>");
            list[lex_index].value = 23;
            list[lex_index].type = neqsym;
            input_index++;
            lex_index++;
            break;
          }
          else if (input[input_index+1] == '=') 
          {
            strcpy(list[lex_index].name, "<=");
            list[lex_index].value = 27;
            list[lex_index].type = leqsym;
            input_index++;
            lex_index++;
            break;
          }
          else if (isspace(input[input_index+1]) || iscntrl(input[input_index+1])) 
          {
            strcpy(list[lex_index].name, ">");
            list[lex_index].value = 25;
            list[lex_index].type = lsssym;
            input_index++;
            lex_index++;
            break;
          }
          else
          {
            printlexerror(1);
            return NULL;
          }
        break;
        
        case '%':
          strcpy(list[lex_index].name, "%");
          list[lex_index].value = 24;
          list[lex_index].type = modsym;
          lex_index++;
        break;  

        case '/':
          strcpy(list[lex_index].name, "/");
          list[lex_index].value = 26;
          list[lex_index].type = divsym;
          lex_index++;
        break;

        case '*':
          strcpy(list[lex_index].name, "*");
          list[lex_index].value = 28;
          list[lex_index].type = multsym;
          lex_index++;
        break;

        case '>':
          if (input[input_index+1] == '=') 
          {
            strcpy(list[lex_index].name, ">=");
            list[lex_index].value = 31;
            list[lex_index].type = geqsym;
            input_index++;
            lex_index++;
            break;
          }
          else if (isspace(input[input_index+1]) || iscntrl(input[input_index+1]))
          {
            strcpy(list[lex_index].name, ">");
            list[lex_index].value = 29;
            list[lex_index].type = gtrsym;
            input_index++;
            lex_index++;
            break;
          }
          else
          {
            printlexerror(1);
            return NULL;
          }
        break;

        case '-':
          strcpy(list[lex_index].name, "-");
          list[lex_index].value = 30;
          list[lex_index].type = subsym;
          lex_index++;
        break;
        
        case '+':
          strcpy(list[lex_index].name, "+");
          list[lex_index].value = 32;
          list[lex_index].type = addsym;
          lex_index++;
        break;
        
        default:
          printlexerror(4);
          return NULL;
        break;
      }
      input_index++;
    }
  }
  
	if (printFlag)
		printtokens();
	list[lex_index++].type = -1;
	return list;
}

void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case lbracketsym:
				printf("%11s\t%d", "[", lbracketsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case rbracketsym:
				printf("%11s\t%d", "]", rbracketsym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case questionsym:
				printf("%11s\t%d", "?", questionsym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case colonsym:
				printf("%11s\t%d", ":", colonsym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case lparenthesissym:
				printf("%11s\t%d", "(", lparenthesissym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case rparenthesissym:
				printf("%11s\t%d", ")", rparenthesissym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
			case neqsym:
				printf("%11s\t%d", "<>", neqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case subsym:
				printf("%11s\t%d", "-", subsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case addsym:
				printf("%11s\t%d", "+", addsym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			default:
				printf("%11s\t%s", "err", "err");
				break;
		}
		printf("\n");
	}
	printf("\n");
}

void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Number Length\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Identifier Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}