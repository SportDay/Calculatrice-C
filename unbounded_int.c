#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "unbounded_int.h"

#define numLen(X, Y) snprintf(NULL, 0, Y, X) - (X < 0)


static void destroy_chiffre(chiffre *c) {
    if (c != NULL) {
        destroy_chiffre(c->suivant);
        free(c);
    }
}

static void destroy_unbounded_int(unbounded_int *u) {
    destroy_chiffre(u->premier);
    u->premier = NULL;
    u->dernier = NULL;
}


static int checkStingIsNum(const char *e) {
    if (!strlen(e)) return 0;
    int start = 0;
    if (e[0] == '-' || e[0] == '+') {
        start = 1;
    }
    for (int i = start; i < strlen(e); i++) {
        if (!(e[i] >= '0' && e[i] <= '9')) {
            return 0;
        }
    }
    return 1;
}

static unbounded_int *emptyUnbounded() {
    unbounded_int *e = malloc(sizeof(unbounded_int));
    if (e == NULL) {
        perror("Memory ERROR emptyUnbounded\n");
        exit(EXIT_FAILURE);
    }
    e->premier = NULL;
    e->dernier = NULL;
    e->signe = '*';
    e->len = 0;
    return e;
}


static chiffre *makeChiffre(const char e) {
    if (!(e >= '0' && e <= '9')) {
        return NULL;
    }
    chiffre *to_return = malloc(sizeof(chiffre *));
    if (to_return == NULL) {
        perror("Memory ERROR chiffre\n");
        exit(EXIT_FAILURE);
    }
    to_return->suivant = NULL;
    to_return->precedent = NULL;
    to_return->c = e;
    return to_return;
}

static unbounded_int *addFirst(unbounded_int *u, const char c) {
    if (u != NULL) {
        chiffre *ch = makeChiffre(c);
        if (ch != NULL) {
            if (u->dernier == NULL) {
                u->dernier = ch;
            } else {
                u->premier->precedent = ch;
                ch->suivant = u->premier;
            }
            u->premier = ch;
            u->len++;
        }
    }
    return u;
}

static unbounded_int *addLast(unbounded_int *u, const char c) {
    if (u != NULL) {
        chiffre *ch = makeChiffre(c);
        if (ch != NULL) {
            if (u->premier == NULL) {
                u->premier = ch;
            } else {
                u->dernier->suivant = ch;
                ch->precedent = u->dernier;
            }
            u->dernier = ch;
            u->len++;
        }
    }
    return u;
}


static unbounded_int removeFirstZeros(unbounded_int x) {
    int i = 0;
    for (chiffre *e = x.premier; e != NULL; e = e->suivant) {
        if (e->c == '0' && x.len - i == 1) {
            x.premier = e;
            x.len -= i;
            return x;
        } else if (e->c != '0') {
            x.premier = e;
            x.len -= i;
            return x;
        }
        i++;
    }
    return *emptyUnbounded();
}

static unbounded_int somme(unbounded_int ua, unbounded_int ub) {
    unbounded_int *to_return = emptyUnbounded();
    chiffre *longg = ua.dernier;
    chiffre *shortt = ub.dernier;
    if (ua.len < ub.len) {
        longg = ub.dernier;
        shortt = ua.dernier;
    }
    int r = 0;
    int ai;
    int bi;
    to_return->signe = ua.signe;
    for (longg = longg; longg != NULL; longg = longg->precedent) {
        ai = longg->c - '0';
        bi = (shortt == NULL) ? 0 : shortt->c - '0';
        char ci = (char) ((ai + bi + r) % 10 + '0');
        addFirst(to_return, ci);
        r = (ai + bi + r) / 10;
        if (shortt != NULL) {
            shortt = shortt->precedent;
        }
    }
    if (r != 0) {
        addFirst(to_return, (r + '0'));
    }
    return *to_return;
}

static unbounded_int soustraction(unbounded_int ua, unbounded_int ub) {
    unbounded_int *res = emptyUnbounded();
    chiffre *longg = ua.dernier;
    chiffre *shortt = ub.dernier;
    switch (unbounded_int_cmp_unbounded_int(ua, ub)) {
        case -1:
            res->signe = '-';
            longg = ub.dernier;
            shortt = ua.dernier;
            break;
        case 0:
            res->signe = ua.signe;
            return string2unbounded_int("0");
        case 1:
            res->signe = '+';
            longg = ua.dernier;
            shortt = ub.dernier;
            break;
        default:
            res->signe = '*';
            break;
    }

    int r = 0;
    int ai;
    int bi;
    for (longg = longg; longg != NULL; longg = longg->precedent) {
        ai = longg->c - '0';
        if (shortt == NULL) {
            bi = 0;
        } else {
            bi = shortt->c - '0';
        }
        int value = (ai - bi + r);
        r = 0;
        if (value < 0) {
            value += 10;
            r = -1;
        }
        char ci = (char) (value + '0');
        addFirst(res, ci);
        if (shortt != NULL) {
            shortt = shortt->precedent;
        }
    }
    *res = removeFirstZeros(*res);
    return *res;

}

unbounded_int string2unbounded_int(const char *e) {
    if (!checkStingIsNum(e)) {
        return *emptyUnbounded();
    }
    unbounded_int *to_return = emptyUnbounded();
    int start = 0;
    if (e[0] == '-') {
        to_return->signe = '-';
    } else {
        to_return->signe = '+';
    }
    if (e[0] == '-' || e[0] == '+') {
        start = 1;
    }
    for (int i = start; i <= strlen(e) - 1; i++) {
        addLast(to_return, e[i]);
    }
    *to_return = removeFirstZeros(*to_return);
    return *to_return;
}

char *unbounded_int2string(unbounded_int i) {
    if (i.signe == '*') {
        return NULL;
    }
    i = removeFirstZeros(i);
    char *to_return;
    int start = 0;
    if (i.signe == '-') {
        to_return = malloc((sizeof(char) * i.len) + 2);
        to_return[start] = i.signe;
        start = 1;
    } else {
        to_return = malloc((sizeof(char) * i.len) + 1);
    }
    if (to_return == NULL) {
        perror("Memory ERROR unbounded_int2string");
        free(to_return);
        exit(EXIT_FAILURE);
    }
    chiffre *courant = i.premier;
    while (courant != NULL) {
        to_return[start] = courant->c;
        courant = courant->suivant;
        start++;
    }
    to_return[start] = '\0';
    return to_return;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
    if (a.signe == '-' && b.signe == '+') {
        return -1;
    }
    if (a.signe == '+' && b.signe == '-') {
        return 1;
    }
    if((a.len>b.len) && a.signe=='+' && b.signe=='+'){
        return 1;
    }
    if ((a.len > b.len) && a.signe=='-' && b.signe=='-' ) {
        return -1;
    }
    if((a.len<b.len) && a.signe=='+' && b.signe=='+'){
        return -1;
    }
    if((a.len<b.len) && a.signe=='-' && b.signe=='-'){
        return 1;
    }

    chiffre *c1 = a.premier;
    chiffre *c2 = b.premier;
    while (c1 != NULL && c2 != NULL) {
        if (c1->c != c2->c) {
            break;
        }
        c1 = c1->suivant;
        c2 = c2->suivant;
    }

    if (c1 == NULL || c2 == NULL) {
        return 0;
    }
    if (a.signe == '+' && (c1->c - '0') < (c2->c - '0')) {// Si a < b
        return -1;
    }
    if (a.signe == '-' && (c1->c - '0') < (c2->c - '0')) {// Si -a < -b
        return 1;
    }
    if (a.signe == '-') { // Si -a < -b
        return -1;
    }

    return 1;
}

int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    if (a.signe == '-' && b > 0) {
        return -1;
    }
    if (a.signe == '+' && b < 0) {
        return 1;
    }
    return unbounded_int_cmp_unbounded_int(a, ll2unbounded_int(b));
}

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {
    if ((a.signe == '+' && b.signe == '+')) {
        return somme(a, b);
    }
    if ((a.signe == '-' && b.signe == '-')) {
        unbounded_int to_return = somme(a, b);
        to_return.signe = '-';
        return to_return;
    }
    if (a.signe == '+' && b.signe == '-') {
        b.signe = '+';
        return soustraction(a, b);
    }
    if (a.signe == '-' && b.signe == '+') {
        a.signe = '+';
        return soustraction(b, a);
    }
    return *emptyUnbounded();
}


unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b) {
    a = removeFirstZeros(a);
    b = removeFirstZeros(b);
    if (a.signe == '+' && b.signe == '+') {
        return soustraction(a, b);
    } else if (a.signe == '-' && b.signe == '-') {
        a.signe = '+';
        b.signe = '+';
        return soustraction(b, a);
    } else if (a.signe == '+' && b.signe == '-') {
        b.signe = '+';
        return somme(a, b);
    } else if (a.signe == '-' && b.signe == '+') {
        a.signe = '+';
        unbounded_int to_return = somme(b, a);
        to_return.signe = '-';
        return to_return;
    }
    return *emptyUnbounded();
}

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b) {
    unbounded_int to_return;
    char *result = malloc(sizeof(char) * (a.len + b.len) + 1);
    if (result == NULL) {
        perror("Memory ERROR unbounded_int_produit");
        return *emptyUnbounded();
    }
    for (int i = 0; i < (a.len + b.len); i++) {
        *(result + i) = '0';
    }
    int i = 0;

    for (chiffre *ai = a.dernier; ai != NULL && i < a.len; ai = ai->precedent, i++) {
        int r = 0;
        int vali = (ai->c) - 48;
        if (vali == 0) continue;
        int j = 0;
        for (chiffre *bj = b.dernier; bj != NULL && j < b.len; bj = bj->precedent, j++) {
            int valj = (bj->c) - 48;
            int res = (result[(a.len + b.len - 1) - (i + j)] - 48) + (vali) * (valj) + r;
            result[(a.len + b.len - 1) - (i + j)] = (res % 10) + '0';
            r = res / 10;
        }
        result[(a.len + b.len - 1) - (i + b.len)] = r + '0';
    }
    result[a.len + b.len] = '\0';
    to_return = string2unbounded_int(result);
    chiffre *p = to_return.premier;

    if (p != NULL && (p->c) == '0') {
        to_return = removeFirstZeros(to_return);
    }
    if ((a.signe == '-' && b.signe == '+') || (a.signe == '+' && b.signe == '-')) {
        to_return.signe = '-';
    } else {
        to_return.signe = '+';
    }
    free(result);
    return to_return;
}

unbounded_int ll2unbounded_int(long long i) {
    char *tmp;
    int end = sizeof(char) * numLen(llabs(i), "%lli") + 1;
    if (i < 0) {
        tmp = malloc(end + 1);
        end++;
    } else {
        tmp = malloc(end);
    }
    if (tmp == NULL) {
        perror("Memory ERROR ll2unbounded_int");
        return *emptyUnbounded();
    }
    snprintf(tmp, end, "%lli", i);
    unbounded_int to_return = string2unbounded_int(tmp);
    free(tmp);
    return to_return;
}


static unbounded_int Binary_mult(unbounded_int a, unbounded_int b) {
    char *result = malloc(sizeof(char) * (a.len + b.len) + 1);
    if (result == NULL) {
        perror("Memory ERROR Binary_mult");
        free(result);
        return *emptyUnbounded();
    }
    for (int i = 0; i < (a.len + b.len); i++) {
        *(result + i) = '0';
    }
    int i = 0;
    for (chiffre *ai = a.dernier; ai != NULL && i < a.len; ai = ai->precedent, i++) {
        int r = 0;
        int vali = (ai->c) - 48;
        if (vali == 0) continue;
        int j = 0;
        for (chiffre *bj = b.dernier; bj != NULL && j < b.len; bj = bj->precedent, j++) {
            int valj = (bj->c) - 48;
            int res = (result[(a.len + b.len - 1) - (i + j)] - 48) + (vali) * (valj) + r;
            result[(a.len + b.len - 1) - (i + j)] = (res % 2) + '0';
            r = res / 2;
        }
        result[(a.len + b.len - 1) - (i + b.len)] = r + '0';
    }
    result[(a.len + b.len)] = '\0';
    unbounded_int to_return = string2unbounded_int(result);
    chiffre *p = to_return.premier;
    if ((p->c) - '0' == 0) {
        to_return = removeFirstZeros(to_return);
    }
    if ((a.signe == '-' && b.signe == '+') || (a.signe == '+' && b.signe == '-')) {
        to_return.signe = '-';
    }
    free(result);
    return to_return;
}

static unbounded_int dix_puiss(int i) {
    char *s = "1010";
    unbounded_int r = string2unbounded_int(s);
    unbounded_int to_return = string2unbounded_int(s);
    int j = 1;
    while (j < i) {
        to_return = Binary_mult(to_return, r);
        j++;
    }
    return to_return;
}

static unbounded_int deux_puiss(int i) {
    char *s = "2";
    char *un = "1";
    unbounded_int one = string2unbounded_int(un);
    unbounded_int r = string2unbounded_int(s);
    unbounded_int to_return = string2unbounded_int(s);
    if (i == 0) {
        return one;
    } else {
        int j = 1;
        while (j < i) {
            to_return = unbounded_int_produit(to_return, r);
            j++;
        }
        destroy_unbounded_int(&one);
        destroy_unbounded_int(&r);
        return to_return;
    }
}

unbounded_int charToBinary(char a) {
    unbounded_int *to_return = emptyUnbounded();
    int v = a - '0';
    int i = 0;
    for (i = 0; i < 4; i++) {//représenté sur 4 bits maximum
        char s = (v % 2) + '0';
        addFirst(to_return, s);
        v /= 2;
    }
    to_return->signe = '+';
    to_return->len = 4;
    return *to_return;
}

static unbounded_int somme_binaire(unbounded_int ua, unbounded_int ub) {
    unbounded_int *to_return = emptyUnbounded();
    chiffre *longg = ua.dernier;
    chiffre *shortt = ub.dernier;
    if (ua.len < ub.len) {
        longg = ub.dernier;
        shortt = ua.dernier;
    }
    int r = 0;
    int ai;
    int bi;
    to_return->signe = ua.signe;
    for (longg = longg; longg != NULL; longg = longg->precedent) {
        ai = longg->c - '0';
        bi = (shortt == NULL) ? 0 : shortt->c - '0';
        char ci = (char) ((ai + bi + r) % 2 + '0');
        addFirst(to_return, ci);
        r = (ai + bi + r) / 2;
        if (shortt != NULL) {
            shortt = shortt->precedent;
        }
    }
    if (r != 0) {
        addFirst(to_return, (r + '0'));
    }
    return *to_return;
}

static unbounded_int ConvertBinarytoDecimale(unbounded_int a) {
    unbounded_int *to_return = emptyUnbounded();
    int i = 0;
    for (chiffre *x = a.dernier; x != NULL && i < a.len; x = x->precedent, i++) {
        if (((x->c) - '0') == 1) {
            unbounded_int val = deux_puiss(i);
            if (to_return->len == 0) {
                *to_return = val;
            } else {
                *to_return = somme(*to_return, val);
            }
        }
    }
    to_return->signe = a.signe;
    return *to_return;
}

static unbounded_int sub(unbounded_int ua, unbounded_int ub) {
    unbounded_int *res = emptyUnbounded();
    chiffre *longg = ua.dernier;
    chiffre *shortt = ub.dernier;
    int r = 0;
    int ai;
    int bi;
    for (longg = longg; longg != NULL; longg = longg->precedent) {
        ai = longg->c - '0';
        if (shortt == NULL) {
            bi = 0;
        } else {
            bi = shortt->c - '0';
        }
        int value = (ai - bi + r);
        r = 0;
        if (value < 0) {
            value += 2;
            r = -1;
        }
        char ci = (char) (value + '0');
        addFirst(res, ci);
        if (shortt != NULL) {
            shortt = shortt->precedent;
        }
    }
    if (r != 0) {
        res->signe = '-';
    } else {
        res->signe = '+';
    }
    return *res;

}


static unbounded_int unbounded_intToBinary(unbounded_int a) {
    unbounded_int to_return;
    to_return.signe = a.signe;
    int i = 0;
    for (chiffre *ai = a.dernier; ai != NULL && i < a.len; ai = ai->precedent, i++) {
        unbounded_int vb = charToBinary(ai->c);
        if (i == 0) {
            to_return = vb;
        } else {
            unbounded_int p = dix_puiss(i);
            unbounded_int val = Binary_mult(vb, p);
            to_return = somme_binaire(to_return, val);
            destroy_unbounded_int(&vb);
            destroy_unbounded_int(&val);
            destroy_unbounded_int(&p);
        }
    }
    if ((to_return.premier)->c - '0' == 0) {
        to_return = removeFirstZeros(to_return);
    }
    return to_return;
}


unbounded_int unbounded_int_quotient(unbounded_int a, unbounded_int b) {
      if (unbounded_int_cmp_unbounded_int(a, b) == -1 && a.signe == '+') {
          char *s = "0";
          return string2unbounded_int(s);
      }else if(unbounded_int_cmp_unbounded_int(a,b)==-1 && a.signe=='-' && a.len<b.len){
              char *s="-1";
              return string2unbounded_int(s);
      }else if(unbounded_int_cmp_unbounded_int(a,b)==1 && a.signe=='-' && b.signe=='-'){
          char *s="1";
          return string2unbounded_int(s);
      }else if(unbounded_int_cmp_unbounded_int(a,b)==1 && b.signe == '-' && a.len<=b.len){
          char *s = "0";
          return string2unbounded_int(s);
      }else if (unbounded_int_cmp_unbounded_int(a, b) == 0) {
          char *s = "1";
          return string2unbounded_int(s);
      }else{
        unbounded_int abin = unbounded_intToBinary(a);
        unbounded_int bbin = unbounded_intToBinary(b);
        char *s = unbounded_int2string(abin);
        char un = '1';
        char zero = '0';
        unbounded_int *to_return = emptyUnbounded();
        unbounded_int *tmp = emptyUnbounded();
        tmp->signe = '+';
        to_return->signe = '+';
        for (int i = 0; i < bbin.len; i++) {
            addLast(tmp, s[i]);
        }
        for (int j = bbin.len; j < strlen(s); j++) {
            if (sub(*tmp, bbin).signe == '+') {
                addLast(to_return, un);
                *tmp = sub(*tmp, bbin);
            } else {
                addLast(to_return, zero);
            }
            addLast(tmp, s[j]);
        }
        if (sub(*tmp, bbin).signe == '-') {
            addLast(to_return, zero);
        } else {
            addLast(to_return, un);
        }
        destroy_unbounded_int(&abin);
        destroy_unbounded_int(&bbin);
        destroy_unbounded_int(tmp);
        if ((a.signe == '+' && b.signe == '-') || (a.signe == '-' && b.signe == '+')) {
            to_return->signe = '-';
        } else {
            to_return->signe = '+';
        }
        return ConvertBinarytoDecimale(*to_return);
    }
}


unbounded_int unbounded_int_modulo(unbounded_int a, unbounded_int b) {
    unbounded_int q = unbounded_int_quotient(a, b);
    unbounded_int r = unbounded_int_produit(b, q);
    unbounded_int to_return = unbounded_int_difference(a, r);
    char signeB = b.signe;
    if (to_return.signe == '-') {
        b.signe = '+';
        to_return = unbounded_int_somme(to_return, b);
        b.signe = signeB;
    }
    destroy_chiffre(q.premier);
    destroy_chiffre(r.premier);
    return to_return;
}







