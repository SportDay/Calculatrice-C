#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "unbounded_int.h"

static void printUnbounded(unbounded_int *u) {
    chiffre *courant;
    courant = u->premier;
    printf("%c", u->signe);
    while (courant != NULL) {
        printf("%c", courant->c);
        courant = courant->suivant;
    }
}

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

static int CheckStringIsUnbounded(char *c, unbounded_int *u) {
    int res = 1;
    if (checkStingIsNum(c) == 1) {
        chiffre *courant;
        courant = u->premier;
        int start = 0;
        if (c[0] == '-' || c[0] == '+') {
            if (c[0] != u->signe) {
                res = 0;
                start = 1;
            }
        } else {
            if (u->signe != '+') {
                res = 0;
            } else {
                while (courant != NULL) {
                    if (courant->c != *(c + start)) {
                        res = 0;
                    }
                    courant = courant->suivant;
                    start++;
                }
            }
        }
        return res;
    } else {
        return u->signe == '*' ? 1 : 0;
    }
}

int main(void) {

        printf("\033[36m\n**** Test String2unbounded_int ****\033[00m\n");
        char *c11 = "12315484151115400454874554884548";
        char *c12 = "-97487455878451244812308";
        char *c13 = "+47754874110557";
        char *c14 = "5dg";
        char *c15="597";
        char *c16="-74";
        unbounded_int u11 = string2unbounded_int(c11);
        unbounded_int u12 = string2unbounded_int(c12);
        unbounded_int u13 = string2unbounded_int(c13);
        unbounded_int u14 = string2unbounded_int(c14);
        unbounded_int u15 = string2unbounded_int(c15);
        unbounded_int u16 = string2unbounded_int(c16);

        printf("Test 1/5: (Input = %s), Obtained result: ",c16);
        printUnbounded(&u16);
        printf(CheckStringIsUnbounded(c11, &u11) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");

        printf("Test 2/5: (Input = %s), Obtained result: ",c11);
        printUnbounded(&u11);
        printf(CheckStringIsUnbounded(c11, &u11) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");

        printf("Test 3/5: (Input = %s), Obtained result: ",c12);
        printUnbounded(&u12);
        printf(CheckStringIsUnbounded(c12, &u12) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");

        printf("Test 4/5: (Input = %s), Obtained result: ",c13);
        printUnbounded(&u13);
        printf(CheckStringIsUnbounded(c13, &u13) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");

        printf("Test 5/5: (Input = %s), Obtained result: ",c14);
        printUnbounded(&u14);
        printf(CheckStringIsUnbounded(c14, &u14) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");


        printf("\033[36m\n**** Test ll2unbounded_int ****\033[00m\n");
        long long a = 9876543219876541;
        long long b = -645828178;
        long long f = 987;
        char *Resultsforll2unbounded[]={"987","9876543219876541","-645828178"};
        unbounded_int aU = ll2unbounded_int(a);
        unbounded_int bU = ll2unbounded_int(b);
        unbounded_int cU = ll2unbounded_int(f);
        printf("Test 1/3: (Input = %lld), Obtained result: ",f);
        printUnbounded(&cU);
        printf(CheckStringIsUnbounded(Resultsforll2unbounded[0],&cU) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 2/3: (Input = %lld), Obtained result: ",a);
        printUnbounded(&aU);
        printf(CheckStringIsUnbounded(Resultsforll2unbounded[1],&aU) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 3/3: (Input = %lld), Obtained result: ",b);
        printUnbounded(&bU);
        printf(CheckStringIsUnbounded(Resultsforll2unbounded[2],&bU) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");

        printf("\033[36m\n**** Test unbounded_int2String ****\033[00m\n");
        char *a1 = unbounded_int2string(u11);
        char *a2 = unbounded_int2string(u12);
        char *a3 = unbounded_int2string(u13);
        char *a4 = unbounded_int2string(aU);
        char *a5 = unbounded_int2string(bU);
        char *a6= unbounded_int2string(u15);
        printf("Test 1/6: (Input = ");
        printUnbounded(&u11);
        printf("), Obtained result: %s",a1);
        printf(CheckStringIsUnbounded(unbounded_int2string(u11), &u11) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 2/6: (Input = ");
        printUnbounded(&u12);
        printf("), Obtained result: %s",a2);
        printf(CheckStringIsUnbounded(unbounded_int2string(u12), &u12) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 3/6: (Input = ");
        printUnbounded(&u13);
        printf(") Obtained result: %s",a3);
        printf(CheckStringIsUnbounded(unbounded_int2string(u13), &u13) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 4/6: (Input = ");
        printUnbounded(&aU);
        printf(") Obtained result: %s",a4);
        printf(CheckStringIsUnbounded(unbounded_int2string(aU), &aU) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 5/6: (Input = ");
        printUnbounded(&bU);
        printf(") Obtained result: %s",a5);
        printf(CheckStringIsUnbounded(unbounded_int2string(bU), &bU) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 6/6: (Input = ");
        printUnbounded(&u15);
        printf("), Obtained result: %s",a6);
        printf(CheckStringIsUnbounded(unbounded_int2string(u15), &u15) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");


        printf("\033[36m\n**** Test unbounded_int_cmp_unbounded_int ****\033[00m\n");
        int r1 = unbounded_int_cmp_unbounded_int(u11, u12);
        int r2 = unbounded_int_cmp_unbounded_int(u12, u13);
        int r3 = unbounded_int_cmp_unbounded_int(u11, u11);
        int r4=unbounded_int_cmp_unbounded_int(u13,u11);
        int r5=unbounded_int_cmp_unbounded_int(u15,u16);
        int Resultforcmp[]={1,-1,0,-1,1};
        printf("Test 1/5: (Input1 = %s, Input2 = %s), Obtained result: %d",unbounded_int2string(u11),unbounded_int2string(u12),r1);
        printf((r1==Resultforcmp[0])? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 2/5: (Input1 = %s, Input2 = %s), Obtained result: %d",unbounded_int2string(u12),unbounded_int2string(u13),r2);
        printf((r2==Resultforcmp[1])? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 3/5: (Input1 = %s, Input2 = %s), Obtained result: %d",unbounded_int2string(u11),unbounded_int2string(u11),r3);
        printf((r3==Resultforcmp[2])? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 4/5: (Input1 = %s, Input2 = %s), Obtained result: %d",unbounded_int2string(u13),unbounded_int2string(u11),r4);
        printf((r4==Resultforcmp[3])? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 5/5: (Input1 = %s, Input2 = %s), Obtained result: %d",unbounded_int2string(u15),unbounded_int2string(u16),r5);
        printf((r5==Resultforcmp[4])? " : \033[32mTrue\033[00m\n" : " : False\n");


        printf("\033[36m\n**** Test unbounded_int_cmp_ll ****\033[00m\n");
        int r6 = unbounded_int_cmp_ll(u11, a);
        int r7 = unbounded_int_cmp_ll(u12, a);
        long long c =47754874110557;
        int r8 = unbounded_int_cmp_ll(u13, c);
        int r9 = unbounded_int_cmp_ll(u15,f);
        int Resultforcmpll[]={1,-1,0,-1};
        printf("Test 1/4: (Input1 = %s, Input2 = %lld), Obtained result: %d",unbounded_int2string(u11),a,r6);
        printf((r6==Resultforcmpll[0])? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 2/4: (Input1 = %s, Input2 = %lld), Obtained result: %d",unbounded_int2string(u12),a,r7);
        printf((r7==Resultforcmpll[1])? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 3/4: (Input1 = %s, Input2 = %lld), Obtained result: %d",unbounded_int2string(u13),c,r8);
        printf((r8==Resultforcmpll[2])? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 4/4: (Input1 = %s, Input2 = %lld), Obtained result: %d",unbounded_int2string(u15),f,r9);
        printf((r9==Resultforcmpll[3])? " : \033[32mTrue\033[00m\n" : ": False\n");



        printf("\033[36m\n**** Test unbounded_int_somme ****\033[00m\n");
        unbounded_int s11 = unbounded_int_somme(u11, u12);
        unbounded_int s12 = unbounded_int_somme(u11, u13);
        unbounded_int s13 = unbounded_int_somme(bU, u12);
        unbounded_int s14 = unbounded_int_somme(u15,u16);
        char *Resultsomme[]={"12315484053627944576423310072240","12315484151115400502629428995105","-97487455878451244812308","523"};
        printf("Test 1/4: (Input1 =%s, Input2 =%s), Obtained result: ",unbounded_int2string(u11),unbounded_int2string(u12));
        printUnbounded(&s11);
        printf(CheckStringIsUnbounded(Resultsomme[0],&s11) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 2/4: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u11),unbounded_int2string(u13));
        printUnbounded(&s12);
        printf(CheckStringIsUnbounded(Resultsomme[1],&s12) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 3/4: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(bU),unbounded_int2string(u12));
        printUnbounded(&s13);
        printf(CheckStringIsUnbounded(Resultsomme[2],&s13) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
        printf("Test 4/4: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u15),unbounded_int2string(u16));
        printUnbounded(&s14);
        printf(CheckStringIsUnbounded(Resultsomme[3],&s14) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");

    printf("\033[36m\n**** Test unbounded_int_difference ****\033[00m\n");
    unbounded_int d1 = unbounded_int_difference(u11, u12);
    unbounded_int d2 = unbounded_int_difference(u11, u13);
    unbounded_int d3 = unbounded_int_difference(u11, u11);
    unbounded_int d4 = unbounded_int_difference(u13, u12);
    unbounded_int d5=unbounded_int_difference(u12,u13);
    unbounded_int d6=unbounded_int_difference(u15,u16);
    char *Resultdiff[]={"12315484248602856333325799696856","12315484151115400407119680773991","0","97487455926206118922865","-97487455926206118922865","671"};
    printf("Test 1/6: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u11),unbounded_int2string(u12));
    printUnbounded(&d1);
    printf(CheckStringIsUnbounded(Resultdiff[0],&d1) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 2/6: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u11),unbounded_int2string(u13));
    printUnbounded(&d2);
    printf(CheckStringIsUnbounded(Resultdiff[1],&d2) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 3/6: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u11),unbounded_int2string(u11));
    printUnbounded(&d3);
    printf(CheckStringIsUnbounded(Resultdiff[2],&d3) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 4/6: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u13),unbounded_int2string(u12));
    printUnbounded(&d4);
    printf(CheckStringIsUnbounded(Resultdiff[3],&d4) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 5/6: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u12),unbounded_int2string(u13));
    printUnbounded(&d5);
    printf(CheckStringIsUnbounded(Resultdiff[4],&d5) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 6/6: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u15),unbounded_int2string(u16));
    printUnbounded(&d6);
    printf(CheckStringIsUnbounded(Resultdiff[5],&d6) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");


    printf("\033[36m\n**** Test unbounded_int_produit ****\033[00m\n");
    unbounded_int p1 = unbounded_int_produit(u11, u12);
    unbounded_int p2 = unbounded_int_produit(u12, u12);
    unbounded_int p3= unbounded_int_produit(u15,u16);
    char *Resultprod[]={"-1200605217803628184665136516919370660315706906869416784","9503804053652978168344308704573482882148286864","-44178"};
    printf("Test 1/3: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u11),unbounded_int2string(u12));
    printUnbounded(&p1);
    printf(CheckStringIsUnbounded(Resultprod[0],&p1) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 2/3: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u12),unbounded_int2string(u12));
    printUnbounded(&p2);
    printf(CheckStringIsUnbounded(Resultprod[1],&p2) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 3/3: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u15),unbounded_int2string(u16));
    printUnbounded(&p3);
    printf(CheckStringIsUnbounded(Resultprod[2],&p3) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");

    printf("\033[36m\n**** Test unbounded_int_quotient ****\033[00m\n");
    unbounded_int q1=unbounded_int_quotient(u11,u12);
    unbounded_int q2=unbounded_int_quotient(u12,u13);
    unbounded_int q3=unbounded_int_quotient(u13,u11);
    unbounded_int q4=unbounded_int_quotient(u13,u12);
    unbounded_int q5=unbounded_int_quotient(u15,u16);
    char *Resultquotient[]={"-126328911","-2041413734","0","0","-8"};
    printf("Test 1/5: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u11),unbounded_int2string(u12));
    printUnbounded(&q1);
    printf(CheckStringIsUnbounded(Resultquotient[0],&q1) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 2/5: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u12),unbounded_int2string(u13));
    printUnbounded(&q2);
    printf(CheckStringIsUnbounded(Resultquotient[1],&q2) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 3/5: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u13),unbounded_int2string(u11));
    printUnbounded(&q3);
    printf(CheckStringIsUnbounded(Resultquotient[2],&q3) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 4/5: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u13),unbounded_int2string(u12));
    printUnbounded(&q4);
    printf(CheckStringIsUnbounded(Resultquotient[3],&q4) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 5/5: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u15),unbounded_int2string(u16));
    printUnbounded(&q5);
    printf(CheckStringIsUnbounded(Resultquotient[4],&q5) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");

    printf("\033[36m\n**** Test unbounded_int_modulo ****\033[00m\n");
    unbounded_int md1=unbounded_int_modulo(u11,u12);
    unbounded_int md2=unbounded_int_modulo(u12,u13);
    unbounded_int md3=unbounded_int_modulo(u11,u13);
    unbounded_int md4=unbounded_int_modulo(u15,u16);
    char *Resultmod[]={"13830106331141285847960","44035723488087","12440734586383","5"};
    printf("Test 1/4: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u11),unbounded_int2string(u12));
    printUnbounded(&md1);
    printf(CheckStringIsUnbounded(Resultmod[0],&md1) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 2/4: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u12),unbounded_int2string(u13));
    printUnbounded(&md2);
    printf(CheckStringIsUnbounded(Resultmod[1],&md2) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 3/4: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u11),unbounded_int2string(u13));
    printUnbounded(&md3);
    printf(CheckStringIsUnbounded(Resultmod[2],&md3) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");
    printf("Test 4/4: (Input1 = %s, Input2 = %s), Obtained result: ",unbounded_int2string(u15),unbounded_int2string(u16));
    printUnbounded(&md4);
    printf(CheckStringIsUnbounded(Resultmod[3],&md4) == 1 ? " : \033[32mTrue\033[00m\n" : " : False\n");

    destroy_unbounded_int(&u11);
    destroy_unbounded_int(&u12);
    destroy_unbounded_int(&u13);
    destroy_unbounded_int(&u14);
    destroy_unbounded_int(&u15);
    destroy_unbounded_int(&u16);
    destroy_unbounded_int(&aU);
    destroy_unbounded_int(&bU);
    destroy_unbounded_int(&cU);
    destroy_unbounded_int(&s11);
    destroy_unbounded_int(&s12);
    destroy_unbounded_int(&s13);
    destroy_unbounded_int(&s14);
    destroy_unbounded_int(&d1);
    destroy_unbounded_int(&d2);
    destroy_unbounded_int(&d3);
    destroy_unbounded_int(&d4);
    destroy_unbounded_int(&d5);
    destroy_unbounded_int(&d6);
    destroy_unbounded_int(&p1);
    destroy_unbounded_int(&p2);
    destroy_unbounded_int(&p3);
    destroy_unbounded_int(&q1);
    destroy_unbounded_int(&q2);
    destroy_unbounded_int(&q3);
    destroy_unbounded_int(&q4);
    destroy_unbounded_int(&q5);
    destroy_unbounded_int(&md1);
    destroy_unbounded_int(&md2);
    destroy_unbounded_int(&md3);
    destroy_unbounded_int(&md4);

    return 0;
}
