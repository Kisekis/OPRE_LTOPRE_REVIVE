#include "Revive.h"
#include <cmath>
#include "my_random.h"
extern "C"{
#include "glpk.h"
}

void computeDelta(const Graph &G) {
    for(int i = 0;i<G.N;i++) {
        for(int j :G.neigh[i]) {
            G.reviveDelta[i][j] = 0.5 * (-G.Hl[i][j] + G.Hr[i][j]);
        }
    }
}

bool revive(Graph &G) {
    {
        computeDelta(G);
        int m = G.N;
        int n = G.channelSize;
        glp_prob *lp;
        lp = glp_create_prob();
        glp_set_obj_dir(lp, GLP_MAX);

        glp_add_rows(lp, m);
        glp_add_cols(lp, n);

        for (int i = 1; i <= m; i++)
        {
            auxiliary_variables_rows:
            glp_set_row_name(lp, i, "y");
            glp_set_row_bnds(lp, i, GLP_FX, 0.0, 0.0);
        }

        int edge_index = 0;
        for(int i = 0;i<G.N;i++) {
            for(int j : G.neigh[i]) {
                if(i<j) {
                    edge_index++;
                    variables_columns:
                    glp_set_col_name(lp, edge_index, "x");
                    double ub = min(abs(G.reviveDelta[i][j]), abs(G.reviveDelta[j][i]));
                    if(ub != 0) {
                        glp_set_col_bnds(lp, edge_index, GLP_DB, 0.0, min(abs(G.reviveDelta[i][j]), abs(G.reviveDelta[j][i])));
                    }else {
                        glp_set_col_bnds(lp, edge_index, GLP_FX, 0.0, 0.0);
                    }


                    to_maximize:
                    glp_set_obj_coef(lp, edge_index, 1);
                }
            }
        }


        int *ia = new int[20000000];
        int *ja = new int[20000000];
        auto *ar = new double[20000000];
        int count = 0;

        for (int i = 1; i <= m; i++)
        {
            int j = 0;
            for(int u = 0;u<G.N;u++) {
                for(int v : G.neigh[u]) {
                    if(u<v) {
                        j++;
                        count = n * (i - 1) + j;
                        ia[count] = i;
                        ja[count] = j;
                        if (u + 1 == i)
                        {
                            if(G.Hl[u][v]>G.Hr[u][v]) {
                                ar[count] = 1;
                            }else if(G.Hl[u][v]<G.Hr[u][v]) {
                                ar[count] = -1;
                            }else {
                                ar[count] = 0;
                            }
                        }
                        else if (v + 1 == i)
                        {
                            if(G.Hl[u][v]>G.Hr[u][v]) {
                                ar[count] = -1;
                            }else if(G.Hl[u][v]<G.Hr[u][v]) {
                                ar[count] = 1;
                            }else {
                                ar[count] = 0;
                            }
                        }
                        else
                        {
                            ar[count] = 0;
                        }
                    }
                }
            }
        }
        glp_load_matrix(lp, m * n, ia, ja, ar);
        calculate:
        glp_simplex(lp, nullptr);
        output:
        int index = 0;
        for(int u = 0;u<G.N;u++) {
            for(int v : G.neigh[u]) {
                if(u<v) {
                    index++;
                    double d = glp_get_col_prim(lp, index);
                    if(G.Hl[u][v]>G.Hr[u][v]) {
                        G.Hl[u][v]-=d;
                        G.Hl[v][u]+=d;
                        G.Hr[u][v]+=d;
                        G.Hr[v][u]-=d;
                    }else {
                        G.Hl[u][v]+=d;
                        G.Hl[v][u]-=d;
                        G.Hr[u][v]-=d;
                        G.Hr[v][u]+=d;
                    }

                }
            }
        }
        delete[] ia;
        delete[] ja;
        delete[] ar;
    }
    return true;
}
