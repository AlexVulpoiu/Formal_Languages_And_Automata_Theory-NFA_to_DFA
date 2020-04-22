#include <fstream>
#include <set>
#include <queue>
#include <vector>
#include <iostream>

using namespace std;

int n, m, k, l, q0, n_dfa, l_dfa;
char alfabet[260];

set<int> finale, finale_dfa, tranzitie[1005][260], tranzitie_dfa[10005][260];
set<int>::iterator itr;

vector<set<int>> codificare;

void citire_automat()
{
    ifstream f("automat.in");
    int i, x, y;
    char c;

    f >> n;
    f >> m;
    for(i = 0; i < m; i++)
        f >> alfabet[i];
    f >> q0;
    f >> k;
    for(i = 0; i < k; i++)
    {
        f >> x;
        finale.insert(x);
    }
    f >> l;
    for(i = 0; i < l; i++)
    {
        f >> x >> c >> y;
        tranzitie[x][int(c)].insert(y);
    }
    f.close();
}

void construire_DFA()
{
    int i, j, s, lg;
    queue<set<int>> q;
    set<set<int>> viz;
    set<int> st, M;
    set<int>::iterator it;

    st.insert(q0);
    q.push(st);         /// coada q contine starile compuse prin care trecem
    l_dfa = 0;
    while(!q.empty())
    {
        M = q.front();      /// pentru fiecare stare compusa din coada
        q.pop();
        if(!M.empty())
        {
            if(viz.find(M) == viz.end())
            {
                s = codificare.size();
                codificare.push_back(M);        /// codificare[i] = M <=> multimea compusa de stari este codificata in DFA cu starea i
                viz.insert(M);                  /// astfel, starea initiala va fi codificata cu 0
            }
            else
            {
                lg = codificare.size();
                s = -1;
                for(i = 0; i < lg; i++)
                    if(codificare[i] == M)
                    {
                        s = i;
                        break;
                    }
            }
            for(it = M.begin(); it != M.end(); it++)    /// verificam daca starea compusa M este stare finala in DFA
                if(finale.find(*it) != finale.end())
                {
                    finale_dfa.insert(s);
                    break;
                }
            for(i = 0; i < m; i++)
            {
                st.clear();
                for(it = M.begin(); it != M.end(); it++)
                    for(itr = tranzitie[*it][int(alfabet[i])].begin(); itr != tranzitie[*it][int(alfabet[i])].end(); itr++)
                        st.insert(*itr);    /// formam o multime cu starile in care ajungem cu caracterul alfabet[i] plecand din starea compusa M
                if(!st.empty())
                {
                    if(viz.find(st) == viz.end())
                    {
                        q.push(st);     /// starea compusa este adaugata in coada daca nu a mai fost vizitata
                        codificare.push_back(st);   /// starea compusa este codificata
                        tranzitie_dfa[s][int(alfabet[i])].insert(codificare.size() - 1);
                        viz.insert(st);
                        l_dfa++;
                        /// adaugam o tranzitie in DFA de la starea curenta codificata cu s la noua starea compusa pe care am gasit-o
                    }
                    else
                    {
                        lg = codificare.size();
                        for(j = 0; j < lg; j++)
                            if(codificare[j] == st)
                            {
                                tranzitie_dfa[s][int(alfabet[i])].insert(j);
                                l_dfa++;
                                break;      /// adaugam o tranzitie in DFA de la s la starea compusa pe care am gasit-o
                            }
                    }
                }
            }
        }
    }
}

void afisare_automat()
{
    ofstream g("automat.out");
    int i, j;

    n_dfa = codificare.size();
    g << "DFA:\n" << n_dfa << '\n';
    g << m << '\n';
    for(i = 0; i < m; i++)
        g << alfabet[i] << ' ';
    g << "\n0\n";
    g << finale_dfa.size() << '\n';
    for(itr = finale_dfa.begin(); itr != finale_dfa.end(); itr++)
        g << *itr << ' ';
    g << '\n' << l_dfa << '\n';
    for(i = 0; i < n_dfa; i++)
        for(j = 0; j < m; j++)
            for(itr = tranzitie_dfa[i][int(alfabet[j])].begin(); itr != tranzitie_dfa[i][int(alfabet[j])].end(); itr++)
                g << i << ' ' << alfabet[j] << ' ' << *itr << '\n';
}

int main()
{
    citire_automat();
    construire_DFA();
    afisare_automat();
    return 0;
}
