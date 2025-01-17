#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BigInteger.h"


struct node* newNode(int val) {
    node *temp = (node*)malloc(sizeof(node));
    temp->data = val;
    temp->next = NULL;
    return temp;
}


void display(BI b) {
    if(b.head == NULL)
        return;
    if(b.head->next == NULL) {
        if(b.sign == 1) printf("-");
        printf("%d", b.head->data);
        return ;
    }
    node *temp = b.head;
    b.head = b.head->next;
    display(b);
    printf("%d", temp->data);
}


struct node* createCopy(struct node *head) {
    node *temp = NULL, *itr = NULL;

    while(head) {
        if(temp == NULL) {
            temp = newNode(head->data);
            itr = temp;
        } else {
            itr->next = newNode(head->data);
            itr = itr->next;
        }

        head = head->next;
    }

    return temp;
}


node* reverse(node *head) {
    head = createCopy(head);

    node *cur = head, *prev = NULL, *next;

    while(cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    return prev;
}


int length(node* head) {
    int l = 0;

    while(head) {
        head = head->next;
        ++l;
    }

    return l;
}


int compare(node *l1, node *l2) {
    if(length(l1) > length(l2)) return 1;
    if(length(l1) < length(l2)) return 0;

    l1 = reverse(l1);
    l2 = reverse(l2);

    while(l1) {
        if(l1->data > l2->data) return 1;
        else if(l2->data > l1->data) return 0;
        l1 = l1->next;
        l2 = l2->next;
    }

    return 2;
}


struct node* addlist(struct node *l1, struct node *l2) {
    if(l1 == NULL) return l2;
    if(l2 == NULL) return l1;

    struct node *res = NULL, *itr = NULL;
    int sum, carry = 0;

    if(length(l1) < length(l2)) {
        struct node *temp = l1;
        l1 = l2;
        l2 = temp;
    }

    while(l2) {
        sum = l1->data + l2->data + carry;
        carry = sum / 10;
        if(itr == NULL) {
            res = newNode(sum%10);
            itr = res;
        }
        else { 
            itr->next = newNode(sum%10);
            itr = itr->next;
        }
        l1 = l1->next;
        l2 = l2->next;
    }

    while(l1) {
        sum = l1->data + carry;
        carry = sum / 10;
        itr->next = newNode(sum % 10);
        itr = itr->next;
        l1 = l1->next;
    }

    if(carry) itr->next = newNode(carry);

    return res;
}


void freeList(node *head) {
    node *temp = head;

    while(head) {
        head = head->next;
        free(temp);
        temp = head;
    }

    free(temp);
} 


node* remZeros(node *head) {
    head = reverse(head);
    node *temp;
    while(head && head->next) {
        if(head->data == 0) {
            temp = head;
            head = head->next;
            free(temp);
        }
        else break;
    }
    head = reverse(head);
    return head;
}


BI initialize(char *str) {
    BI b;
    b.length = strlen(str) - 2;
    node *temp = NULL, *itr = NULL;

    for(int i = b.length; i >= 0; --i) {
        if(i == 0) {
            if(*(str + i) == '-') {
                b.sign = 1;
                break;
            } else {
                b.sign = 0;
            }
        }
        if(!temp) {
            temp = newNode(*(str + i) - 48);
            itr = temp;
        } else {
            itr->next = newNode(*(str + i) - 48);
            itr = itr->next;
        }
    }

    b.head = remZeros(temp);
    return b;
}


BI sub(BI a, BI b) {
    if(a.head == NULL) {
        if(b.head == NULL) {
            a.head = newNode(0);
            a.sign = 0;
            a.length = 1;
            return a;
        }
        else {
            a.head = newNode(0);
            return sub(a, b);
        }
    }
    if(b.head == NULL) return a;

    BI d;
    d.sign = 0;

    if(a.sign == 0 && b.sign == 1) {
        d.head = addlist(a.head, b.head);
        d.sign = 0;
        d.length = length(d.head);
        return d;
    }
    if(a.sign == 1 && b.sign == 0) {
        d.head = addlist(a.head, b.head);
        d.sign = 1;
        d.length = length(d.head);
        return d;
    }
    if(a.sign == 1 && b.sign == 1) d.sign = 1;

    int c = compare(a.head, b.head), l = 0;

    if(c == 2) {
        a.head = newNode(0);
        a.length = 1;
        a.sign = 0;
        return a;
    } 

    node *res = NULL, *itr = NULL;

    if(c == 0) {
        d = sub(b, a);
        res = d.head;
        itr = res;
        while(itr->next) itr = itr->next;
        itr->data = -itr->data;
        d.head = res;
        return d;
    }

    int term, borrow = 0;
    node *l1 = a.head, *l2 = b.head;

    while(l2) {
        int x = l1->data;
        int y = l2->data;
        if(x >= (y + borrow)) {
            term = x - y - borrow;
            borrow = 0;
        }
        else {
            term = 10 + x - y - borrow;
            borrow = 1;
        }
        if(res == NULL) {
            itr = newNode(term);
            res = itr;
        } else {
            res->next = newNode(term);
            res = res->next;
        }
        l1 = l1->next;
        l2 = l2->next;
        ++l;
    }

    while(l1) {
        if(borrow) {
            if(l1->data >= borrow) {
                res->next = newNode(l1->data - borrow);
                borrow = 0;
            }
            else {
                res->next = newNode(9);
                borrow = 1;
            }
        }
        else res->next = newNode(l1->data);

        l1 = l1->next;
        res = res->next;
        ++l;
    }

    itr = remZeros(itr);

    d.head = itr;
    d.length = l;
    return d;
}


BI add(BI a, BI b) {
    if(a.head == NULL && b.head == NULL) {
        a.head = newNode(0);
        a.sign = 0;
        a.length = 1;
        return a;
    }
    if(a.head == NULL) return b;
    if(b.head == NULL) return a;

    if(compare(a.head, b.head) == 0) {
        BI c = a;
        a = b;
        b = c;
    }

    BI c;
    c.head = NULL;
    c.length = 0;

    if(a.sign == b.sign) c.sign = b.sign;
    else {
        if(a.sign == 1) {
            a.sign = 0;
            c = sub(a, b);
            c.sign = 1;
            return c;
        }
        else {
            b.sign = 0;
            c = sub(a, b);
            c.sign = 0;
            return c;
        }
    }

    int l = 0;

    node *temp = NULL, *itr = NULL;
    int sum, carry = 0;

    while(b.head) {
        sum = a.head->data + b.head->data + carry;
        carry = sum/10;
        ++c.length;

        if(temp) {
            itr->next = newNode(sum%10);
            itr = itr->next;
        } else {
            temp = newNode(sum%10);
            itr = temp;
        }

        a.head = a.head->next;
        b.head = b.head->next;
        ++l;
    }

    while(a.head) {
        sum = a.head->data + carry;
        carry = sum/10;
        ++c.length;

        if(temp) {
            itr->next = newNode(sum%10);
            itr = itr->next;
        } else {
            temp = newNode(sum%10);
            itr = temp;
        }

        a.head = a.head->next;
        ++l;
    }

    if(carry) {
        itr->next = newNode(carry);
        ++l;
    }

    c.head = temp;
    c.length = l;
    return c;
}


BI mul(BI a, BI b) {
    if(a.head == NULL || b.head == NULL) {
        a.head = newNode(0);
        a.length = 1;
        a.sign = 0;
        return a;
    }

    if(b.length == 1 && b.head->data == 0) return b;

    if(a.length < b.length) {
        BI c = a;
        a = b;
        b = c;
    }

    BI c, d;
    c.head = NULL;
    c.length = 0;

    if(a.sign == b.sign) c.sign = 0;
    else c.sign = 1;

    int count = 0;
    node *l1 = a.head, *l2 = b.head, *res = NULL;

    while(l2) {
        struct node *m = NULL, *mm = NULL, *temp = l1;
        int carry = 0, term;

        for(int i=0; i<count; ++i) {
            if(m) {
                m->next = newNode(0);
                m = m->next;
            } else {
                m = newNode(0);
                mm = m;
            }
        }

        while(temp) {
            term = l2->data * temp->data + carry;
            carry = term/10;
            if(m) {
                m->next = newNode(term%10);
                m = m->next;
            } else {
                m = newNode(term%10);
                mm = m;
            }
            temp = temp->next;
        }

        if(carry) m->next = newNode(carry);
        count++;

        if(res != NULL)
            res = addlist(mm, res);
        else 
            res = addlist(mm, newNode(0));

        freeList(mm);
        l2 = l2->next;
    }

    c.head = res;
    c.length = length(res);
    return c;
}


BI div1(BI a, BI b) {
    if(a.head == NULL) {
        a.head = newNode(0);
        a.length = 1;
        a.sign = 0;
        return a;
    }

    if(b.length == 1 && b.head->data == 1) {
        if(b.sign == 0) return a;
        else {
            a.sign = -a.sign;
            return a;
        }
    }

    int x = compare(a.head, b.head);

    if(x == 2) {
        a.head = newNode(1);
        a.length = 1;
        if(a.sign == b.sign) a.sign = 0;
        else a.sign = 1;
        return a;
    }

    if(x == 0) {
        a.head = newNode(0);
        a.length = 1;
        if(a.sign == b.sign) a.sign = 0;
        else a.sign = 1;
        return a;
    }

    BI c; 

    if(a.sign == b.sign) c.sign = 0;
    else c.sign = 1;

    node *temp = b.head, *tt = NULL, *t = NULL;
    node *count = newNode(0);

    while(compare(a.head, temp)) {
        temp = addlist(temp, b.head);
        count = addlist(count, newNode(1));
        c.head = count;
    }

    c.head = count;
    c.length = length(c.head);
    return c;
}


BI mod(BI a, BI b) {
    if(a.head == NULL && b.head == NULL) {
        a.head = newNode(0);
        a.sign = 0;
        a.length = 1;
        return a;
    }
    if(a.head == NULL) return a;
    if(b.head == NULL) return b;

    int x = compare(a.head, b.head);

    if(x == 2) {
        a.head = newNode(0);
        a.length = 1;
        return a;
    }

    if(x == 0) return a;

    BI c = div1(a, b);
    c = sub(a, mul(c, b));
    c.sign = 0;
    c.length = length(c.head);

    return c;
}