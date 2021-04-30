// Vincent Bell Assignment #1 CS331

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

struct bank {
    unsigned int wolves;
    unsigned int chickens;
    unsigned int boat;

    int sum() {
        return wolves + chickens + boat;
    }
};

struct state {
    bank left;
    bank right;

    bool operator == (state a) {
        if ((left.chickens == a.left.chickens) && (left.wolves == a.left.wolves) && (left.boat == a.left.boat) &&
            (right.chickens == a.right.chickens) && (right.wolves == a.right.wolves) && (right.boat == a.right.boat))
            return true;
        else
            return false;
    }
};

struct node {
    state node_state;
    node* parent;
    int depth;

    bool operator == (node a) {
        if (a.node_state == node_state)
            return true;
        else
            return false;
    }
};

void fileToState(state& start_state, state& goal_state, ifstream& start, ifstream& goal);
bool isValid(state a);
bool required(int totanimals, int totToMove);
bool move(state a, state* node, int chic, int wol);
bool visited(vector<node*> states, state s);
void delete_node(vector<node*> nodes);
void solution(node* sol, int num);
node* BFS(state start, state goal, int &num);
void BFSnode(node*& p, node*& child, state a, vector<node*>& s, queue<node*>& q);
node* DFS(state start, state goal, int &num);
void DFSnode(node*& p, node*& child, state a, vector<node*>& s, stack<node*>& q);
node* IDDFS(state start, state goal, int &num);
void IDDFSnode(node*& p, node*& child, state a, vector<node*>& s, stack<node*>& q);

void fileToState(state &start_state, state &goal_state, ifstream &start, ifstream &goal) {
    char a;
    char b;
    start >> start_state.left.chickens >> a >> start_state.left.wolves >> b >> start_state.left.boat;
    start >> start_state.right.chickens >> a >> start_state.right.wolves >> b >> start_state.right.boat;
    goal >> goal_state.left.chickens >> a >> goal_state.left.wolves >> b >> goal_state.left.boat;
    goal >> goal_state.right.chickens >> a >> goal_state.right.wolves >> b >> goal_state.right.boat;
}

bool isValid(state a) {
     if (a.left.chickens < 0 || a.left.wolves < 0 || a.right.chickens < 0 || a.right.wolves < 0)
        return false;

    if (a.left.chickens > 0) {
        if (a.left.wolves > a.left.chickens && a.left.boat == 0)
            return false;
    }

    if (a.right.chickens > 0) {
        if (a.right.wolves > a.right.chickens && a.right.boat == 0)
            return false;
    }
}

bool required(int totAnimals, int totToMove) {
    if (totAnimals >= totToMove)
        return true;
    else return false;  
}

bool move(state a, state* node, int chic, int wol) {
    if (a.left.boat == 1) {
        if (required(a.left.chickens, chic)) {
            node->left.chickens = a.left.chickens - chic;
            node->right.chickens = a.right.chickens + chic;
        }
        else return false;

        if (required(a.left.wolves, wol)) {
            node->left.wolves = a.left.wolves - wol;
            node->right.wolves = a.right.wolves + wol;
        }
        else return false;

        node->left.boat = 0;
        node->right.boat = 1;
        return true;
    }
    else {
        if (required(a.right.chickens, chic)) {
            node->right.chickens = a.right.chickens - chic;
            node->left.chickens = a.left.chickens + chic;
        }
        else return false;

        if (required(a.right.wolves, wol)) {
            node->right.wolves = a.right.wolves - wol;
            node->left.wolves = a.left.wolves + wol;
        }
        else return false;

        node->right.boat = 0;
        node->left.boat = 1;
        return true;
    }
}

bool visited(vector<node*> states, state s) {
    vector<node*>::iterator it;
    for (it=states.begin(); it != states.end(); it++) {
        if ((*it)->node_state == s)
            return true;
    }
    return false;
}

void delete_node(vector<node*> nodes) {
    vector<node*>::iterator it;
    for (it = nodes.begin(); it != nodes.end(); it++) {
        delete* it;
    }
}

void solution(node* sol, int num) {
    int depth = 0;
    if (sol == 0) {
        cout << "nodes expanded: " << num << endl;
    }
    else {
        while (sol != 0) {
            depth++;
            cout << sol->node_state.left.chickens << " " << sol->node_state.left.wolves << " " << sol->node_state.left.boat 
                << "," << sol->node_state.right.chickens << " " << sol->node_state.right.wolves << " " << sol->node_state.right.boat << endl;
            sol = sol->parent;
        }
        cout << "nodes expanded: " << num << endl;
        cout << "nodes in solution: " << depth << endl;
    }
}

node* BFS(state start, state goal, int& num) {
    num = 0;

    queue<node*> node_q;
    vector <node*> uniq;

    node* n = new node();
    n->node_state = start;
    n->parent = NULL;

    node_q.push(n);
    uniq.push_back(n);

    node* new_n;

    while (!node_q.empty()) {

        n = node_q.front();
        node_q.pop();

        if (n->node_state == goal)
            return n;

        num++;

        state new_s;

        if (move(n->node_state, &new_s, 1, 0) && isValid(new_s) && !visited(uniq, new_s)) {
            BFSnode(n, new_n, new_s, uniq, node_q);
        }
        if (move(n->node_state, &new_s, 2, 0) && isValid(new_s) && !visited(uniq, new_s)) {
            BFSnode(n, new_n, new_s, uniq, node_q);
        }
        if (move(n->node_state, &new_s, 0, 1) && isValid(new_s) && !visited(uniq, new_s)) {
            BFSnode(n, new_n, new_s, uniq, node_q);
        }
        if (move(n->node_state, &new_s, 1, 1) && isValid(new_s) && !visited(uniq, new_s)) {
            BFSnode(n, new_n, new_s, uniq, node_q);
        }
        if (move(n->node_state, &new_s, 0, 2) && isValid(new_s) && !visited(uniq, new_s)) {
            BFSnode(n, new_n, new_s, uniq, node_q);
        }
    }

    return NULL;
}

void BFSnode(node*& p, node*& child, state a, vector<node*>& s, queue<node*>& q) {
    child = new node();
    child->node_state = a;
    child->parent = p;
    s.push_back(child);
    q.push(child);
}

node* DFS(state start, state goal, int& num) {
    num = 0;

    stack<node*> node_stack;
    vector <node*> uniq;

    node* n = new node();
    n->node_state = start;
    n->parent = NULL;

    node_stack.push(n);
    uniq.push_back(n);

    node* new_n;

    while (!node_stack.empty()) {

        n = node_stack.top();
        node_stack.pop();

        if (n->node_state == goal)
            return n;

        num++;

        state new_s;

        if (move(n->node_state, &new_s, 1, 0) && isValid(new_s) && !visited(uniq, new_s)) {
            DFSnode(n, new_n, new_s, uniq, node_stack);
        }
        if (move(n->node_state, &new_s, 2, 0) && isValid(new_s) && !visited(uniq, new_s)) {
            DFSnode(n, new_n, new_s, uniq, node_stack);
        }
        if (move(n->node_state, &new_s, 0, 1) && isValid(new_s) && !visited(uniq, new_s)) {
            DFSnode(n, new_n, new_s, uniq, node_stack);
        }
        if (move(n->node_state, &new_s, 1, 1) && isValid(new_s) && !visited(uniq, new_s)) {
            DFSnode(n, new_n, new_s, uniq, node_stack);
        }
        if (move(n->node_state, &new_s, 0, 2) && isValid(new_s) && !visited(uniq, new_s)) {
            DFSnode(n, new_n, new_s, uniq, node_stack);
        }
    }

    return 0;
}

void DFSnode(node*& p, node*& child, state a, vector<node*>& s, stack<node*>& q) {
    child = new node();
    child->node_state = a;
    child->parent = p;
    s.push_back(child);
    q.push(child);
}

node* IDDFS(state start, state goal, int &num) {
    num = 0;

    int maxDepth = 0;
    int deepestNode = 0;

    stack<node*> node_stack;
    vector <node*> uniq;

    while (true) {

        node* n = new node();
        n->node_state = start;
        n->parent = NULL;

        node_stack.push(n);
        uniq.push_back(n);

        node* new_n;

        while (!node_stack.empty()) {


            n = node_stack.top();
            node_stack.pop();

            if (deepestNode < n->depth)
                deepestNode = n->depth;

            if (n->node_state == goal)
                return n;

            num++;

            state new_s;

            if (n->depth == maxDepth)
                continue;

           

            if (move(n->node_state, &new_s, 1, 0) && isValid(new_s) && !visited(uniq, new_s)) {
                IDDFSnode(n, new_n, new_s, uniq, node_stack);
            }
            if (move(n->node_state, &new_s, 2, 0) && isValid(new_s) && !visited(uniq, new_s)) {
                IDDFSnode(n, new_n, new_s, uniq, node_stack);
            }
            if (move(n->node_state, &new_s, 0, 1) && isValid(new_s) && !visited(uniq, new_s)) {
                IDDFSnode(n, new_n, new_s, uniq, node_stack);
            }
            if (move(n->node_state, &new_s, 1, 1) && isValid(new_s) && !visited(uniq, new_s)) {
                IDDFSnode(n, new_n, new_s, uniq, node_stack);
            }
            if (move(n->node_state, &new_s, 0, 2) && isValid(new_s) && !visited(uniq, new_s)) {
                IDDFSnode(n, new_n, new_s, uniq, node_stack);
            }
        }
        if (deepestNode < maxDepth) {
            return 0;
            cout << "num: " << num << endl;
        }

        deepestNode = 0;
        maxDepth++;
        delete_node(uniq);
        uniq.clear();
    }   
}

void IDDFSnode(node*& p, node*& child, state a, vector<node*>& s, stack<node*>& q){
    child = new node();
    child->node_state = a;
    child->parent = p;
    child->depth = p->depth + 1;
    s.push_back(child);
    q.push(child);
}

int main()
{
    string start_file, goal_file, search_type, output_file;
    state start_state, goal_state;
    ifstream start;
    ifstream goal;

    cout << "Enter a start file (start1.txt, start2.txt, start3.txt): ";
    cin >> start_file;
    if (start_file == "start1.txt" || start_file == "start2.txt" || start_file == "start3.txt") {
        start.open(start_file.c_str());
    }
    else {
        cout << "Invalid goal file" << endl;
        return 0;
    }

    cout << "Enter a goal file (goal1.txt, goal2.txt, goal3.txt): ";
    cin >> goal_file;
    if (goal_file == "goal1.txt" || goal_file == "goal2.txt" || goal_file == "goal3.txt") {
        goal.open(goal_file.c_str());
    }
    else {
        cout << "Invalid goal file" << endl;
        return 0;
    }

    fileToState(start_state, goal_state, start, goal);

    int num;

    node* sol;

    cout << "Enter search type (BFS, DFS, IDDFS, ASTAR): ";
    cin >> search_type;
    if (search_type == "BFS") {
        sol = BFS(start_state, goal_state, num);
        solution(sol, num);
    }
    if (search_type == "DFS") {
        sol = DFS(start_state, goal_state, num);
        solution(sol, num);
    }
    if (search_type == "IDDFS") {
        sol = IDDFS(start_state, goal_state, num);
        solution(sol, num);
    }

    return 0;
}

