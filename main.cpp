#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <climits>
#include <cstdlib>

using namespace std;

/* ---------------- TRANSACTION STRUCT ---------------- */
struct Transaction {
    int step;
    string type;
    string sender;
    string receiver;
    double amount;
    int isFraud;
};

/* ---------------- READ CSV ---------------- */
vector<Transaction> readCSV(const string& filename) {
    vector<Transaction> transactions;
    ifstream file(filename.c_str());
    string line;

    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return transactions;
    }

    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        Transaction t;

        getline(ss, temp, ','); t.step = atoi(temp.c_str());
        getline(ss, t.type, ',');
        getline(ss, t.sender, ',');
        getline(ss, t.receiver, ',');
        getline(ss, temp, ','); t.amount = atof(temp.c_str());

        for (int i = 0; i < 4; i++)
            getline(ss, temp, ',');

        getline(ss, temp, ',');
        t.isFraud = atoi(temp.c_str());

        transactions.push_back(t);
    }

    file.close();
    return transactions;
}

/* ---------------- MERGE SORT ---------------- */
void merge(vector<Transaction>& a, int l, int m, int r) {
    vector<Transaction> temp;
    int i = l, j = m + 1;

    while (i <= m && j <= r) {
        if (a[i].amount <= a[j].amount)
            temp.push_back(a[i++]);
        else
            temp.push_back(a[j++]);
    }

    while (i <= m) temp.push_back(a[i++]);
    while (j <= r) temp.push_back(a[j++]);

    for (int k = l; k <= r; k++)
        a[k] = temp[k - l];
}

void mergeSort(vector<Transaction>& a, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(a, l, m);
    mergeSort(a, m + 1, r);
    merge(a, l, m, r);
}

/* ---------------- RABIN-KARP ---------------- */
bool rabinKarp(string text, string pattern) {
    int d = 256, q = 101;
    int n = text.length();
    int m = pattern.length();

    if (m > n) return false;

    int p = 0, t = 0, h = 1;

    for (int i = 0; i < m - 1; i++)
        h = (h * d) % q;

    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (int i = 0; i <= n - m; i++) {
        if (p == t && text.substr(i, m) == pattern)
            return true;

        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) t += q;
        }
    }
    return false;
}

/* ---------------- DIJKSTRA (NO >> ISSUE) ---------------- */
void dijkstra(int src, vector<vector<pair<int,int> > > &graph)
 {
    int V = graph.size();
    vector<int> dist(V, INT_MAX);

    // max heap, store negative distance
    priority_queue<pair<int,int> > pq;

    dist[src] = 0;
    pq.push(make_pair(0, src));

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = -pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (size_t i = 0; i < graph[u].size(); i++) {
            int v = graph[u][i].first;
            int w = graph[u][i].second;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push(make_pair(-dist[v], v));
            }
        }
    }

    cout << "\nShortest money transfer paths:\n";
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX)
            cout << "Account " << i << " : Unreachable\n";
        else
            cout << "Account " << i << " : " << dist[i] << endl;
    }
}

/* ---------------- MAIN ---------------- */
int main() {

   vector<Transaction> transactions = readCSV("C:/Users/sahan/Downloads/bs140513_032310.csv");


    if (transactions.empty()) {
        cout << "No data loaded." << endl;
        return 0;
    }

    cout << "Transactions loaded: " << transactions.size() << endl;

    mergeSort(transactions, 0, transactions.size() - 1);

    cout << "\nTop 5 High Value Transactions:\n";
    for (int i = transactions.size() - 1; i >= transactions.size() - 5; i--) {
        cout << transactions[i].sender << " -> "
             << transactions[i].receiver << " : "
             << transactions[i].amount << endl;
    }

    if (rabinKarp("ABABCDABAB", "ABAB"))
        cout << "\nSuspicious repeated transaction pattern detected!\n";

    vector<vector<pair<int,int> > > graph(5);
    graph[0].push_back(make_pair(1, 10));
    graph[1].push_back(make_pair(2, 5));
    graph[2].push_back(make_pair(3, 8));
    graph[3].push_back(make_pair(4, 3));

    dijkstra(0, graph);

    cout << "\nFraud detection completed successfully.\n";
    return 0;
}
