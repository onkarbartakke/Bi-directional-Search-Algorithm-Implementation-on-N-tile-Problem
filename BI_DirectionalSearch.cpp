#include <bits/stdc++.h>

#define n 3
using namespace std;
// BT19CSE012 ONKAR BARTAKKE
class Node
{
public:
    Node *parent;
    Node *child;
    vector<vector<char>> mat;
    int rowB;
    int colB;

    Node(vector<vector<char>> mat)
    {
        this->mat = mat;
        this->parent = NULL;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (mat[i][j] == 'B')
                {
                    rowB = i;
                    colB = j;
                    break;
                }
            }
        }
    }
};

vector<vector<char>> FinalState(n, vector<char>(n));
bool ReachedFinal = false;

queue<Node *> qu1; // FOR Forward Search
queue<Node *> qu2; // For Backward Search

unordered_map<string, Node *> mp1; // Forward Buffer
unordered_map<string, Node *> mp2; // Backward Buffer

int dx[] = {1, 0, -1, 0};
int dy[] = {0, 1, 0, -1};

void PrintTile(Node *node) //Print Tile helper function
{
    if (node == NULL)
    {
        cout << "\nNULL TILE NODE\n";
        return;
    }

    cout << "\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << node->mat[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n";
}

bool isParent(Node *node, vector<vector<char>> mat) // Is parent
{
    bool isPar = true;

    if (node->parent == NULL)
        return false;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (node->parent->mat[i][j] != mat[i][j])
            {
                isPar = false;
                return false;
            }
        }
    }

    return true;
}

string HashToString(vector<vector<char>> mat) // Converts Mat to string which can be hashed
{
    string s = "";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            s += mat[i][j];
        }
    }

    return s;
}

void PrintPath1(Node *n1, string s1) // Helper Function
{
    stack<Node *> st;

    Node *temp = n1;

    while (temp != NULL)
    {
        st.push(temp);
        temp = temp->parent;
    }

    while (!st.empty())
    {
        temp = st.top();
        st.pop();

        PrintTile(temp);
    }

    temp = mp2[s1];

    if (temp->parent)
        temp = temp->parent;
    else
    {
        PrintTile(temp);
        return;
    }

    while (temp)
    {
        PrintTile(temp);
        temp = temp->parent;
    }

    return;
}

void PrintPath2(Node *n2, string s2) // Helper Function
{
    Node *temp = mp1[s2];

    stack<Node *> st;

    while (temp != NULL)
    {
        st.push(temp);
        temp = temp->parent;
    }

    while (!st.empty())
    {
        temp = st.top();
        st.pop();

        PrintTile(temp);
    }

    if (n2->parent)
        n2 = n2->parent;
    else
    {
        PrintTile(n2);
        return;
    }

    temp = n2;
    while (temp != NULL)
    {
        PrintTile(temp);
        temp = temp->parent;
    }

    return;
}

void BiDirectionalSearch(vector<vector<char>> &mat)
{
    Node *root = new Node(mat);
    Node *dest = new Node(FinalState);

    qu1.push(root);
    qu2.push(dest);

    Node *t1, *t2;

    vector<vector<char>> tile(n, vector<char>(n));
    int x1, y1, x2, y2, nx, ny;

    while (!qu1.empty() && !qu2.empty())
    {
        t1 = qu1.front();
        qu1.pop();

        t2 = qu2.front();
        qu2.pop();

        x1 = t1->rowB;
        y1 = t1->colB;

        x2 = t2->rowB;
        y2 = t2->colB;

        for (int i = 0; i < 4; i++)
        {
            nx = x1 + dx[i];
            ny = y1 + dy[i];

            if (nx >= 0 && ny >= 0 && nx < n && ny < n)
            {
                swap(t1->mat[x1][y1], t1->mat[nx][ny]);

                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < n; j++)
                        tile[i][j] = t1->mat[i][j];
                }

                swap(t1->mat[x1][y1], t1->mat[nx][ny]);

                string s1 = HashToString(tile);
                if (!isParent(t1, tile) && mp1.find(s1) == mp1.end())
                {
                    Node *ch1 = new Node(tile);

                    ch1->parent = t1;

                    mp1[s1] = ch1;
                    qu1.push(ch1);

                    if (mp2.find(s1) != mp2.end())
                    {
                        // cout << "\nPrinting Path 1\n";
                        // cout << "From State : \n";
                        // PrintTile(ch1);
                        cout << "\nPath from the Given State to Final State is as Follows : \n\n";
                        PrintPath1(ch1, s1);
                        ReachedFinal = true;
                        return;
                    }
                }
            }

            nx = x2 + dx[i];
            ny = y2 + dy[i];

            if (nx >= 0 && ny >= 0 && nx < n && ny < n)
            {
                swap(t2->mat[x2][y2], t2->mat[nx][ny]);

                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < n; j++)
                        tile[i][j] = t2->mat[i][j];
                }

                swap(t2->mat[x2][y2], t2->mat[nx][ny]);

                string s2 = HashToString(tile);

                if (!isParent(t2, tile) && mp2.find(s2) == mp2.end())
                {
                    Node *ch2 = new Node(tile);

                    ch2->parent = t2;
                    mp2[s2] = ch2;
                    qu2.push(ch2);

                    if (mp1.find(s2) != mp1.end())
                    {
                        // cout << "\nPrinting Path 2\n";
                        // cout << "From State : \n";
                        // PrintTile(ch2);
                        cout << "\nPath from the Given State to Final State is as Follows : \n\n";
                        PrintPath2(ch2, s2);
                        ReachedFinal = true;
                        return;
                    }
                }
            }
        }
    }
}

int main()
{
    cout << "This algorithm solves the 3X3 tile Puzzle using Bi-Directional Search \n";
    cout << "Use 'B' to represent blank state \n";

    cout << "\n\nEnter the state of the 3X3 tile : \n";

    vector<vector<char>> mat(n, vector<char>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> mat[i][j];
        }
    }

    cout << "\n\n";
    int cnt = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            FinalState[i][j] = ('1' + cnt);
            cnt++;
        }
    }

    FinalState[n - 1][n - 1] = 'B';

    BiDirectionalSearch(mat);
}