#include<bits/stdc++.h>
using namespace std;

// The current state of the puzzle along with g(n), h(n), f(n) and parent pointer that points to the previous state.
struct Node {
    vector<vector<int>> state;
    int g;
    int h;
    int f;
    Node* parent;
};

// Min-heap priority queue.
auto cmp = [](Node* a, Node* b) {
    return a->f > b->f;
};
priority_queue<Node*, vector<Node*>, decltype(cmp)> pq(cmp);

// Correct positions of the number in the block.
pair<int,int> mappings(int num) {
    switch(num) {
        case 1 : return {0,0};
        case 2 : return {0,1};
        case 3 : return {0,2};
        case 4 : return {1,0};
        case 5 : return {1,1};
        case 6 : return {1,2};
        case 7 : return {2,0};
        case 8 : return {2,1};
    }
    return {-1,-1};
}

// Goal state checking.
bool check(vector<vector<int>> curr) {
    vector<vector<int>> goal = {
        {1,2,3},
        {4,5,6},
        {7,8,0}
    };
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(curr[i][j] != goal[i][j]) return false;
        }
    }
    return true;
}

// Converting the table into string,
// 4 7 8
// 2 1 0    ->    478210356
// 3 5 6
string encode(const vector<vector<int>>& curr) {
    string s = "";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            s += to_string(curr[i][j]);
    return s;
}

// 'Uordered set' storing the visited states which are converted to strings before storing.
unordered_set<string> visited;

// Heuristic function (Manhattan distance).
int heuristic(vector<vector<int>> curr) {
    int h = 0;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(curr[i][j] == 0) continue;
            pair<int, int> now = mappings(curr[i][j]);
            h += (abs(now.first-i) + abs(now.second-j));
        }
    }
    return h;
}

// Adding all the possible moves into the priority queue.
void add_states(Node* curr) {
    pair<int, int> position = {-1,-1};
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(curr->state[i][j] == 0) {
                position = {i,j};
            }
        }
    }
    vector<int> dr = {1,-1,0,0};
    vector<int> dc = {0,0,1,-1};
    for(int i=0; i<4; i++) {
        if((position.first + dr[i] >= 0 && position.first + dr[i] <= 2) && (position.second + dc[i] >= 0 && position.second + dc[i] <= 2)) {
            Node* next = new Node;
            next->parent = curr;
            next->state = curr->state;
            next->g = curr->g + 1;
            swap(next->state[position.first + dr[i]][position.second + dc[i]], next->state[position.first][position.second]);
            next->h = heuristic(next->state);
            next->f = next->h + next->g;
            if(visited.count(encode(next->state)) == 0) {
                pq.push(next);
                visited.insert(encode(next->state));
            }
        } 
    }
}

// A* algorithm
Node* astar() {
    while(pq.size() != 0) {
        Node* element = pq.top();
        pq.pop();
        if(check(element->state)) return element;
        add_states(element);
    }
    cout << "No solution found" <<endl;
    return NULL;
}

int main() {

    // If the answer is found then goal state will be returned. Hence using stack to print the answer in reverse order.
    stack<vector<vector<int>>> store;
    
    // Initial state
    Node* initial = new Node;
    initial->state = {
        {4,7,8},
        {2,1,0},
        {3,5,6}
    };
    initial->parent = NULL;
    initial->g = 0;
    initial->h = heuristic(initial->state);
    initial->f = initial->g + initial->h;

    // Adding initial state into the queue and marking it visited.
    visited.insert(encode(initial->state));
    pq.push(initial);


    // Function calling
    Node* ans = astar();
    Node* it = ans;

    // Iterating from goal to initial state and adding into the stack simultaneously.
    while(it && it->parent) {
        store.push(it->state);
        it = it->parent;
    }
    store.push(initial->state);

    // Printing the answer.
    while(store.size() != 0) {
        vector<vector<int>> popped_el = store.top();
        store.pop();
        for(int i=0; i<3; i++) {
            for(int j=0; j<3; j++) {
                cout << popped_el[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

}