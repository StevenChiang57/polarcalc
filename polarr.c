// STEVEN CHIANG

// WARNING: WHEN YOU COMPILE MAKE SURE YOU INCLUDE -lm
// EXAMPLE "clang polarr.c -lm -o test"
//          ./test
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MAXVAL  100

size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values

char buf[BUFSIZ];
size_t bufp = 0;

int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}


int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';

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
  double op2;
  if (strcmp(s, "sin") == 0)
    push(sin(pop()));
  else if(strcmp(s, "exp") == 0)
    push(exp(pop()));
  else if (strcmp(s, "pow") == 0){
    op2 = pop();
    push(pow(pop(), op2));
  }
}

void rpn(void) {
  int type, var;
  double op2;
  char s[BUFSIZ];
  double variable[26];
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
      case 't': //print top 2
        printtoptwo();
        break;
      case 'd': // duplicate top of the topstack
        dup();
        break;
      case 's': //swap top 2
        swap();
        break;
      case 'c': //clear the stack and sp
        clear();
        break;
      case 'n': // MATH COMMANDS sin cos pow
        mathfunc(s);
        break;
      case '=':
        pop();
        if (var >= 'A' && var <= 'Z')
          variable[var - 'A'] = pop();
        else
          printf("error: unknown variable\n");
        break;
      default:      fprintf(stderr, "unknown command %s\n", s);  break;
    }
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}
