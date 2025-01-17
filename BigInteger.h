#ifndef BIGINTEGER_H
#define BIGINTEGER_H

typedef struct node {
    int data;
    struct node *next;
} node;

typedef struct BigInteger {
    struct node *head;
    int length;
    int sign;
} BI;


node* newNode(int val);
void display(BI b);
node* createCopy(node *head);
node* reverse(node *head);
int length(node* head);
int compare(node *l1, node *l2);
struct node* addlist(struct node *l1, struct node *l2);
void freeList(node *head);
node* remZeros(node *head);
BI initialize(char *str);
BI add(BI a, BI b);
BI sub(BI a, BI b);
BI mul(BI a, BI b);
BI div1(BI a, BI b);
BI mod(BI a, BI b);

#endif