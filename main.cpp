#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <queue>
#include <list>
#include <set>

using namespace std;

int main()
{
    // q = count of state
    // s = count of symbol in sigma
    // a = count of accept
    // m = count of transport
    // n = count of string
    int q, s, a, m, n;
    cin >> q >> s >> a >> m >> n;


    // create zigma
    char zigma[s + 1];
    zigma[0] = '$';
    for (int i = 1; i <= s; i++)
        cin >> zigma[i];


    // q0 = start state;
    int q0;
    cin >> q0;


    // create state situation(true : accept)
    bool state[q];
    for (int i = 0; i < q; i++)
        state[i] = false;
    int temp;
    for (int i = 0; i < a; i++)
    {
        cin >> temp;
        state[temp] = true;
    }


    // create adjacency
    vector<pair<int, char>> transport[q];
    int start, end;
    char symbol;
    int symInt;
    for(int i = 0; i < m; i++)
    {
        cin >> start >> symbol >> end;
        pair<int, char> p(end, symbol);
        transport[start].push_back(p);
    }


    // create landa transport
    vector<int> landaTransport[q];
    for(int i = 0; i < q; i++)
    {
        bool visited[q];
        for(int i = 0; i < q; i++)
            visited[i] = false;
        visited[i] = true;

        queue<int> landaQueue;
        landaQueue.push(i);

        landaTransport[i].push_back(i);

        while(!landaQueue.empty())
        {
            int currunt = landaQueue.front();
            landaQueue.pop();

            for(auto it:transport[currunt])
            {
                if(it.second == '$' && visited[it.first] == false)
                {
                    landaQueue.push(it.first);
                    landaTransport[i].push_back(it.first);
                    visited[it.first] = true;
                }
            }
        }
    }


    // create new transports
    for(int i = 0; i < q; i++)
    {
        vector<pair<int, char>> symbolTransport;

        for(auto it:landaTransport[i])
        {
            for(auto it2:transport[it])
            {
                if(it2.second != '$')
                {
                    pair<int, char> p(it2.first, it2.second);
                    symbolTransport.push_back(p);
                }
            }
        }

        for(auto it2:symbolTransport)
        {
            for(auto it3:landaTransport[it2.first])
            {
                pair<int, char> p(it3, it2.second);
                bool tag = true;
                for(auto it4: transport[i])
                {
                    if(it4 == p)
                    {
                        tag = false;
                        break;
                    }
                }

                if(tag)
                    transport[i].push_back(p);
            }
        }
    }


    // create transport 2 without landa transport
    vector<pair<int, char>> transport2[q];
    for(int i = 0; i < q; i++)
    {
        for(auto it:transport[i])
        {
            if(it.second != '$')
                transport2[i].push_back(it);
        }
    }


    // show graph without landa########################################################################
    /*
    cout << endl;
    for(int i = 0; i < q; i++)
    {
        cout << i << " :" << endl;
        for(auto it: transport2[i])
            cout << "( " << it.first << " , " << it.second << " )" << endl;
        cout << endl;
    }
    */
    // show graph without landa########################################################################


    // create all transport of DFA
    vector<tuple<set<int>, char, set<int>>> setOfDfa;
    set<int> tempVector;
    queue<set<int>> tempQueue;
    tempVector.insert(q0);
    tempQueue.push(tempVector);

    while(!tempQueue.empty())
    {
        tempVector = tempQueue.front();
        tempQueue.pop();
        for(int i = 1; i <= s; i++)
        {
            //tuple<vector<int>, char, vector<int>> tupleTemp;
            tuple<set<int>, char, set<int>> tupleTemp;
            //vector<int> endForTempVector;
            set<int> endForTempVector;
            for(auto it: tempVector)
            {
                for(auto it2: transport2[it])
                {
                    if(it2.second == zigma[i])
                    {
                        endForTempVector.insert(it2.first);
                    }
                }
            }
            tupleTemp = make_tuple(tempVector, zigma[i], endForTempVector);
            bool tag = true;
            for(auto it3: setOfDfa)
            {
                if(it3 == tupleTemp)
                {
                    tag = false;
                    break;
                }
            }

            if(tag)
            {
                setOfDfa.push_back(tupleTemp);
                tempQueue.push(endForTempVector);
            }
        }
    }


    // count of state in DFA
    int newq = 0;
    for(auto it: setOfDfa)
    {
        newq++;
    }
    newq /= s;

    // create accept state array
    bool stateNew[newq];
    int i = 0;
    for(auto it:setOfDfa)
    {
        if(i % s == 0)
        {
            bool tag = false;
            for(auto it2:get<0>(it))
            {
                if(state[it2] == true)
                {
                    tag = true;
                    break;
                }
            }
            stateNew[i / s] = tag;
        }
        i++;
    }
    stateNew[newq] = false;


    // show new state situation#################################################################
    /*
    for(int i = 0; i < newq; i++)
    {
        cout << i << ": " << stateNew[i] << endl;
    }
    cout << endl;
    */
    // show new state situation#################################################################


    // create DFA in normal
    vector<pair<int, char>> transportNew[newq + 1];
    i = 0;
    for(auto it:setOfDfa)
    {
        int count = 0;
        if(get<2>(it).empty())
        {
            pair<int, char> p(newq, get<1>(it));
            transportNew[i / s].push_back(p);
        }

        else
        {
            for(auto it2:setOfDfa)
            {
                if(get<2>(it) == get<0>(it2))
                {
                    break;
                }
                count++;
            }
            pair<int, char> p(count / s, get<1>(it));
            transportNew[i / s].push_back(p);
        }
        i++;
    }


    // show DFA transports########################################################################
    /*
    for(auto it: setOfDfa)
    {
        cout << "( ";
        for(auto it2: get<0>(it))
            cout << it2 << ", ";
        cout << " )";
        cout << "   " << get<1>(it) << "    ";
        cout << "( ";
        for(auto it2: get<2>(it))
            cout << it2 << ", ";
        cout << " )" << endl;
    }
    */
    // show DFA transports########################################################################


    // input string for DFA
    /*
    string string;
    for(int i = 0; i < n; i++)
    {
        cin >> string;

        int currentState = q0;
        for(int j = 0; j < string.length(); j++)
        {
            for(auto it:transport[currentState])
            {
                if(it.second == string[j])
                    currentState = it.first;
            }
        }

        if(state[currentState] == true)
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }
    */


    // input string for NFA
    string string;
    for(int i = 0; i < n; i++)
    {
        cin >> string;

        int currentState = q0;
        for(int j = 0; j < string.length(); j++)
        {
            for(auto it:transportNew[currentState])
            {
                if(it.second == string[j])
                    currentState = it.first;
            }
        }

        if(stateNew[currentState] == true)
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }

    return 0;
}
