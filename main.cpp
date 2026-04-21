#include <bits/stdc++.h>
using namespace std;

template<typename ValueType>
class MyList {
    struct Node {
        ValueType val;
        Node* prev;
        Node* next;
        Node(const ValueType& v): val(v), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int n;

    Node* node_at(int index) const {
        if (index < 0 || index >= n) return nullptr;
        if (index < n/2) {
            Node* cur = head;
            for (int i = 0; i < index; ++i) cur = cur->next;
            return cur;
        } else {
            Node* cur = tail;
            for (int i = n-1; i > index; --i) cur = cur->prev;
            return cur;
        }
    }
public:
    MyList(): head(nullptr), tail(nullptr), n(0) {}
    MyList(MyList&& obj) noexcept: head(obj.head), tail(obj.tail), n(obj.n) {
        obj.head = obj.tail = nullptr; obj.n = 0;
    }
    MyList(const MyList& obj): head(nullptr), tail(nullptr), n(0) {
        for (Node* cur = obj.head; cur; cur = cur->next) push_back(cur->val);
    }
    MyList& operator=(const MyList& other){
        if (this == &other) return *this;
        clear();
        for (Node* cur = other.head; cur; cur = cur->next) push_back(cur->val);
        return *this;
    }
    MyList& operator=(MyList&& other) noexcept {
        if (this == &other) return *this;
        clear();
        head = other.head; tail = other.tail; n = other.n;
        other.head = other.tail = nullptr; other.n = 0;
        return *this;
    }
    ~MyList(){ clear(); }

    void push_back(const ValueType &value){
        Node* nd = new Node(value);
        if (!tail){ head = tail = nd; }
        else { tail->next = nd; nd->prev = tail; tail = nd; }
        ++n;
    }
    void pop_back(){
        if (!tail) return;
        Node* del = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr; else head = nullptr;
        delete del; --n;
    }
    void push_front(const ValueType &value){
        Node* nd = new Node(value);
        if (!head){ head = tail = nd; }
        else { nd->next = head; head->prev = nd; head = nd; }
        ++n;
    }
    void pop_front(){
        if (!head) return;
        Node* del = head;
        head = head->next;
        if (head) head->prev = nullptr; else tail = nullptr;
        delete del; --n;
    }
    ValueType &front() const { return head->val; }
    ValueType &back() const { return tail->val; }
    void insert(int index,const ValueType &value){
        if (index <= 0) { push_front(value); return; }
        if (index >= n) { push_back(value); return; }
        Node* pos = node_at(index);
        Node* nd = new Node(value);
        Node* pr = pos->prev;
        nd->prev = pr; nd->next = pos;
        pr->next = nd; pos->prev = nd;
        ++n;
    }
    void erase(int index){
        if (index < 0 || index >= n) return;
        if (index == 0){ pop_front(); return; }
        if (index == n-1){ pop_back(); return; }
        Node* pos = node_at(index);
        Node* pr = pos->prev; Node* nx = pos->next;
        pr->next = nx; nx->prev = pr;
        delete pos; --n;
    }
    int size() const { return n; }
    bool empty() const { return n==0; }
    void clear(){ while(!empty()) pop_front(); }

    void link(const MyList &obj){
        for (Node* cur = obj.head; cur; cur = cur->next) push_back(cur->val);
    }
    MyList cut(int index){
        MyList res;
        if (index <= 0) {
            // whole list becomes res
            res.head = head; res.tail = tail; res.n = n;
            head = tail = nullptr; n = 0;
            return res;
        }
        if (index >= n) return res; // nothing to cut
        Node* pos = node_at(index);
        // split before pos: [0..index-1] remains; [index..] goes to res
        res.head = pos;
        res.tail = tail;
        // compute size for res and this
        int leftSize = index;
        int rightSize = n - index;
        res.n = rightSize;
        n = leftSize;
        // detach
        Node* pr = pos->prev;
        if (pr) pr->next = nullptr;
        pos->prev = nullptr;
        tail = pr;
        return res;
    }

    // for debugging/printing
    vector<ValueType> to_vector() const {
        vector<ValueType> v; v.reserve(n);
        for (Node* cur=head; cur; cur=cur->next) v.push_back(cur->val);
        return v;
    }
};

// Since the problem statement lacks I/O spec here, we provide a small
// interactive-like parser for local sanity testing. If no input, do nothing.
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    MyList<int> a, b;
    // Simple command format (optional):
    // commands until EOF: op args
    // push_back x | push_front x | pop_back | pop_front | insert i x | erase i
    // link [0|1 from which to link to A] (we link list b into a)
    // cut i -> cuts a and prints right part
    // print -> prints a
    string cmd;
    while (cin >> cmd){
        if (cmd == "push_back") { int x; cin >> x; a.push_back(x); }
        else if (cmd == "push_front") { int x; cin >> x; a.push_front(x); }
        else if (cmd == "pop_back") { a.pop_back(); }
        else if (cmd == "pop_front") { a.pop_front(); }
        else if (cmd == "insert") { int i,x; cin >> i >> x; a.insert(i,x); }
        else if (cmd == "erase") { int i; cin >> i; a.erase(i); }
        else if (cmd == "link") { 
            // read count then that many nums to list b, then link into a
            int k; if (!(cin >> k)) break; b.clear();
            for (int i=0;i<k;++i){ int x; cin >> x; b.push_back(x);} 
            a.link(b);
        }
        else if (cmd == "cut") { int i; cin >> i; MyList<int> r = a.cut(i); 
            auto v = r.to_vector();
            for (size_t j=0;j<v.size();++j){ if (j) cout << ' '; cout << v[j]; }
            cout << "\n";
        }
        else if (cmd == "print") {
            auto v = a.to_vector();
            for (size_t j=0;j<v.size();++j){ if (j) cout << ' '; cout << v[j]; }
            cout << "\n";
        }
    }
    return 0;
}
