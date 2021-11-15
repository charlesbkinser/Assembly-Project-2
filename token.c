#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct token tokens[1000];
int tokencount = 0;

void tokenInit(struct token * self) {
	self->type = TYPE_WHITESPACE;
	self->payload = NULL;
}

void tokenize(char *argv, int size) {
	for (int i = 0; i < size; i++) {
		switch (argv[i]) {

			case ' ':
			{
				break;
			}

			case 'i': 
			{
				if (tryForString(argv,i,size,"int")) {
					pushTokenType(TYPE_KW_INT);
					i = i + 2;
					break;
				} else if (tryForString(argv,i,size,"if")) {
					struct token token;
					pushTokenType(TYPE_KW_IF);
					i = i + 1;
					break;
				}
			}

			case 'm':
			{
				if (tryForString(argv,i,size,"main")) {
					pushTokenType(TYPE_KW_MAIN);
					i = i + 3;
					break;
				}
			}

			case ',':
			{
				pushTokenType(TYPE_COMMA);
				break;
			}

			case ';':
			{
				struct token token;
				pushTokenType(TYPE_SEMI);
				break;
			}

			case '+':
			{
				pushTokenType(TYPE_ADD);
				break;
			}

			case '-':
			{
				pushTokenType(TYPE_SUB);
				break;
			}

			case '&':
			{
				pushTokenType(TYPE_AND);
				break;
			}

			case '|':
			{
				pushTokenType(TYPE_OR);
				break;
			}

			case '=':
			{
				if (tryForString(argv,i,size,"==")) {
					pushTokenType(TYPE_CD_EQUAL);
					i = i + 1;
					break;
				}
				pushTokenType(TYPE_ASSIGN);
				break;
			}

			case '<':
			{
				if (tryForString(argv,i,size,"<=")) {
					pushTokenType(TYPE_CD_LESS_OR_EQUAL);
					i = i + 1;
					break;
				}
				pushTokenType(TYPE_CD_LESS);
				break;
			}

			case '>':
			{
				if (tryForString(argv,i,size,">=")) {
					pushTokenType(TYPE_CD_GREATER_OR_EQUAL);
					i = i + 1;
					break;
				}
				pushTokenType(TYPE_CD_GREATER);
				break;
			}

			case '!':
			{
				if (tryForString(argv,i,size,"!=")) {
					pushTokenType(TYPE_CD_NOT_EQUAL);
					i = i + 1;
					break;
				}
			}

			case '(':
			{
				pushTokenType(TYPE_LEFT_PAREN);
				break;
			}

			case ')':
			{
				pushTokenType(TYPE_RIGHT_PAREN);
				break;
			}

			case '{':
			{
				pushTokenType(TYPE_LEFT_BRACE);
				break;
			}

			case '}':
			{
				pushTokenType(TYPE_RIGHT_BRACE);
				break;
			}

			case 'p':
			{
				if (tryForString(argv,i,size,"printf")) {
					pushTokenType(TYPE_KW_PRINTF);
					i = i + 5;
					break;
				}
			}

			case '"':
			{
				int index = findIndexOfNextChar(argv,i,size,'"');
				if (index != -1) {
                    printf("First %i, second %i ", i, index);
                    char stringparse[index - i];
                    memcpy(stringparse, &argv[i], index - i);
                    stringparse[index - 1] = '\0';
					struct token Token;
                    strcpy(Token.payload, stringparse); // seg fault here
                    Token.type = TYPE_STRING;
                    pushToken(Token);

                    i = index;
                    break;
				}
				//TODO concat from first double quote till second and store as string
				break;
			}

			default:
			{

			}
		}
	}
}
	
//Getter for array of tokens
//https://stackoverflow.com/questions/9914122/getting-an-array-from-another-file-in-c/9914238

void get_token_array(int which, struct token **buffer) {
	if(which == 1) *buffer = tokens;
}


/*
* Dynamically sizing array
* https://stackoverflow.com/questions/3536153/c-dynamically-growing-array
*/

/*
	void initArray(Array *a, size_t initialSize) {
		a->array = malloc(initialSize * sizeof(int));
		a->used = 0;
		a->size = initialSize;
	}

	void insertArray(Array *a, int element) {
		// a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
		// Therefore a->used can go up to a->size
		if (a->used == a->size) {
			a->size *= 2;
			a->array = realloc(a->array, a->size * sizeof(int));
		}
		a->array[a->used++] = element;
	}

	void freeArray(Array *a) {
		free(a->array);
		a->array = NULL;
		a->used = a->size = 0;
	}
	*/
char * tokenTypeString(int typeId) {
	switch (typeId) {
		case TYPE_WHITESPACE: return "WHITESPACE";
		//Valued tokens
		case TYPE_NUMBER: return "NUMBER";
		case TYPE_IDENTIFIER: return "IDENTIFIER";
		case TYPE_STRING: return "STRING";
		//Punctuations
		case TYPE_LEFT_PAREN: return "LEFT_PAREN";
		case TYPE_RIGHT_PAREN: return "RIGHT_PAREN";
		case TYPE_LEFT_BRACE: return "LEFT_BRACE";
		case TYPE_RIGHT_BRACE: return "RIGHT_BRACE";
		case TYPE_COMMA: return "COMMA";
		case TYPE_SEMI: return "SEMI";
		case TYPE_QUOTE: return "QUOTE";
		//Operators
		case TYPE_ADD: return "ADD";
		case TYPE_SUB: return "SUB";
		case TYPE_AND: return "AND";
		case TYPE_OR: return "OR";
		case TYPE_NEG: return "NEG";
		case TYPE_ASSIGN: return "ASSIGN";
		//Conditions
		case TYPE_CD_EQUAL: return "CD_EQUAL";
		case TYPE_CD_LESS: return "CD_LESS";
		case TYPE_CD_GREATER: return "CD_GREATER";
		case TYPE_CD_NOT_EQUAL: return "CD_NOT_EQUAL";
		case TYPE_CD_LESS_OR_EQUAL: return "CD_LESS_OR_EQUAL";
		case TYPE_CD_GREATER_OR_EQUAL: return "CD_GREATER_OR_EQUAL";
		//Keywords and identifiers that we can cheat as keywords
		case TYPE_KW_INT: return "KW_INT";
		case TYPE_KW_SHORT: return "KW_SHORT";
		case TYPE_KW_VOID: return "KW_VOID";
		case TYPE_KW_RETURN: return "KW_RETURN";
		case TYPE_KW_IF: return "KW_IF";
		case TYPE_KW_ELSE: return "KW_ELSE";
		case TYPE_KW_MAIN: return "KW_MAIN";
		case TYPE_KW_SCANF: return "KW_SCANF";
		case TYPE_KW_PRINTF: return "KW_PRINTF";
		//case TYPE_KW_WHILE: return "KW_WHILE"; has duplicated ID. Fix is in another branch
		default: return "UNKNOWN";
	}

}

int tryForString(char * buffer, int position, int size, char * target) {
	int targetLen = strlen(target);
	for (int i = 0; i < targetLen; i++) {
		if (position+1 >= size) return 0;
		if (buffer[position+i] != target[i]) return 0;
	}
	return 1;
}

int findIndexOfNextChar(char * buffer, int position, int size, char target) {
	while (position < size) {
		position = position + 1;
		if (buffer[position] == target) return position;
	}
	return -1;
}

void pushToken(struct token tok) {
	tokens[tokencount] = tok;
	tokencount++;
}

void pushTokenType(int type) {
	struct token token;
	token.type = type;
	tokens[tokencount] = token;
	tokencount++;
}

