// STEVEN CHIANG

// WARNING: WHEN YOU COMPILE MAKE SURE YOU INCLUDE -lm
// EXAMPLE "clang polarr.c -lm -o test"
//          ./test
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#define MAXOP 100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MAXVAL 100
#define MATH '1'
#define VARIABLE '3'
#define STACK '2'
size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values
double variable[26];
char buf[BUFSIZ];
size_t bufp = 0;

int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}

void buildstring(char *s){
  int c, i = 0;
  while(isalpha(s[++i] = c = getch_())){}
  s[i] = '\0';
}

int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';

  if (c == '0'){
    buildstring(s);
    return STACK;
  }

  if (isalpha(c)){
    buildstring(s);
    return MATH;
  }

  if (c == '=' || c == '?'){
    buildstring(s);
    return VARIABLE;
  }

  if (!isdigit(c) && c != '.') { return c; }  // if not a digit, return

  i = 0;
  if (isdigit(c)) {  // get digits before '.'
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '.') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }
  s[i] = '\0';
  if (c != EOF) { ungetch_(c); }
  return NUMBER;      // return type is NUMBER, number stored in s
}

double pop(void) {
  if (sp == 0) { fprintf(stderr, "stack underflow\n");  return 0.0; }
  return val[--sp];
}

void push(double f) {
  if (sp == MAXVAL) { fprintf(stderr, "stack overflow -- can't push %g\n", f);  return; }
  val[sp++] = f;
}


void dup(void){
  // double topstack = pop();
  // push(topstack);
  // push(topstack);
  // cant pop
  push(val[sp - 1]);
}

void printtoptwo(void){
  if (sp > 0)
    printf("%g\n", val[sp - 1]);
  else
    printf("error: stack empty\n");
}

void swap(void){
  if (sp > 2){
    // double firsttop = pop();
    // double secondtop = pop();
    // push(secondtop);
    // push(firsttop);
    //cant pop
    double temp = val[sp - 1];
    val[sp - 1] = val[sp - 2];
    val[sp - 2] = temp;
  }
  else
    printf("error: stack less than 1 value\n");
}

void clear(void){
  while(sp > 0){
    val[sp] = 0.0;
    sp--;
  }
  sp = 0;
}

void mathfunc(char s[]){
  double op2, result = 0;
  if (strcmp(s, "sin") == 0)
    result = sin(pop());
  else if(strcmp(s, "exp") == 0)
    result = exp(pop());
  else if (strcmp(s, "pow") == 0){
    op2 = pop();
    result = pow(pop(), op2);
  }
  push(result);
  printf("%.2f\n", result);
}

void stacks(char s[]){
  if (strcmp(s, "dup"))
    dup();
  else if (strcmp(s, "swap"))
    swap();
  else if (strcmp(s, "print"))
    printtoptwo();
  else if (strcmp(s, "clear"))
    clear();
}
void variables(char *s){
  if (*s == '=')
    variable[*++s - 'A'] = pop();
  else if (*s == '?')
    push(variable[*++s - 'A']);
  // 7 =a
  // ?a 5 +
  //       12
}

void rpn(void) {
  int type;
  double op2;
  char s[BUFSIZ];
  while ((type = getop(s)) != EOF) {
    switch(type) {
      case '\n':    printf("\t%.8g\n", pop());  break;
      case NUMBER:  push(atof(s));              break;
      case '+':     push(pop() + pop());        break;
      case '*':     push(pop() * pop());        break;
      case '-':     push(-(pop() - pop()));     break;
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
      case '%':
        op2 = pop();
        if (op2 != 0.0)
          push((int) pop() % (int)op2);
        else
          printf("error: zero divisor\n");
        break;
      case STACK:
        stacks();
        break;
      case MATH: // MATH COMMANDS sin cos pow
        mathfunc(s);
        break;
      case VARIABLE:
        variables(s);
        break;
      default:      fprintf(stderr, "unknown command %s\n", s);  break;
    }
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}
