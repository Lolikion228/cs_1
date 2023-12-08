//
// Created by lolikion on 27.11.23.
//

#include "two_sat.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "../graphs/graphs.h"

CNF2 *CNF2_init() {
    CNF2 *cnf = (CNF2 *) malloc(sizeof(struct CNF2));
    cnf->clauses = NULL;
    return cnf;
}

clause * clause_init(){
    clause *res=(clause *) malloc(sizeof(struct clause));
    return res;
}


CNF2 *CNF2_get(char *str) {
//    enum states {clause_start, var_start,int,neg };!!!!!!!!!!!!!!!!!!!!!!!
//alphabet={...}
    int mx = 0, j, curr, flag = 0, ord;
//    int state


    CNF2 *cnf = CNF2_init();
    clause *curr_clause=clause_init();
    curr_clause->next=NULL;
    cnf->clauses=curr_clause;
    for (int i = strlen(str) - 1; i >= 0; i--) { //make as pointer
       if (str[i] == ')') {
            flag = 1;
        }
        if ((str[i] == ')') || ((str[i] == '|') && (str[i + 1] == '|'))) {
            j = i - 1;
            curr = 0;
            ord = 0;
            while (str[j] != 'x') {
                curr += (str[j] - '0') * (int) pow(10, ord);
                j--;
                ord++;
            }
            mx = mx >= curr ? mx : curr;
            cnf->max = mx;

//            cnf->arr[cnt][flag % 2] = ((str[j - 1] == '!') ? -1 : 1) * curr;

            if(flag%2==1){ curr_clause->first=((str[j - 1] == '!') ? -1 : 1) * curr;}
            if(flag%2==0){
                curr_clause->second=((str[j - 1] == '!') ? -1 : 1) * curr;
//                printf("%d %d\n",curr_clause->first,curr_clause->second);
                if(j>2){
                curr_clause->next=clause_init();
                curr_clause=curr_clause->next;
                curr_clause->next=NULL;
                }
            }
            ++flag;
        }
    }
    return cnf;
}

// CNF DNF /\ \/
void CNF2_print(CNF2 *cnf) {
    clause *curr=cnf->clauses;
//    printf("228\n");
    while (curr!=NULL) {
        printf("(");
        if (curr->first >= 0) { printf("x%d||", curr->first); }
        else { printf("!x%d||", (-1) * (curr->first)); }
        if (curr->second >= 0) { printf("x%d)", curr->second); }
        else { printf("!x%d)", (-1) * (curr->second)); }
        if ( curr->next!=NULL ) { printf("&&"); }
        curr=curr->next;
    }
    printf("\n");
}


void CNF2_free(CNF2 *cnf) {
    clause *curr=cnf->clauses;
    clause *tmp = curr;

    while (curr->next != NULL) {
        tmp = curr;
        curr = curr->next;
        free(tmp);
    }

    if(curr!=NULL) free(curr);

    free(cnf);
}


graph *get_implication_graph(CNF2 *cnf) {
    int N = cnf->max;
    graph *g = graph_init(1 + 2 * N);
    clause *curr=cnf->clauses;

    while (curr!=NULL) {

        int x1 = curr->first, x2 = curr->second;

        if (x1 < 0) { x1 = abs(x1) + N; }
        if (x2 < 0) { x2 = abs(x2) + N; }

        graph_add_arc(g, x1 + (x1 <= N ? N : -(1) * N), x2);
        graph_add_arc(g, x2 + (x2 <= N ? N : -(1) * N), x1);

        curr=curr->next;
    }

    return g;
}

void get_vals(scc_list *sccs, int *res, int cnt) {
    int flag;
    for (int i = 0; i < sccs->len; i++) {
        node *curr=sccs->array[i].head;
        while (curr) {
            if (res[abs(curr->val)] == -1) {
                res[abs(curr->val)] = curr->val >= 0 ? 1 : 0;
            }
            flag = 1;
            for (int k = 1; k < cnt; k++) {
                if (res[k] == -1) {
                    flag = 0;
                    break;
                }
            }
            if (flag == 1) { return; }
            curr=curr->next;
        }
    }

}


int *TWO_SAT(CNF2 *cnf) {
    graph *g = get_implication_graph(cnf);
    graph_print(g);
    scc_list *sccs = scc(g);

    scc_list_print(sccs);
    graph_visualize(g);
    scc_list_transform(sccs, cnf->max);
    scc_list_print(sccs);
    graph_free(g);
    int matches = scc_search_for_matches(sccs);
    printf("matches=%d\n", matches);
    if (matches) { return NULL; }
    int *res = (int *) malloc((1 + cnf->max) * sizeof(int));
    for (int i = 0; i < 1 + cnf->max; i++) { res[i] = -1; }
    get_vals(sccs, res, 1 + cnf->max);
    scc_list_free(sccs);
    return res;
}



int test_two_sat(CNF2 *cnf, int *vals) {
    int curr, x1, x2;
    clause *curr_clause=cnf->clauses;
    while (curr_clause!=NULL) {
        curr = 0;
        x1 = curr_clause->first;
        x2 = curr_clause->second;
        if (x1 > 0) { curr += vals[x1]; }
        if (x1 < 0) { curr += vals[abs(x1)] == 1 ? 0 : 1; }
        if (x2 > 0) { curr += vals[x2]; }
        if (x2 < 0) { curr += vals[abs(x2)] == 1 ? 0 : 1; }
        if (curr == 0) { return 0; }
        curr_clause=curr_clause->next;
    }
    return 1;
}

