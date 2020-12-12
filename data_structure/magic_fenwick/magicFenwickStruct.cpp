template <typename UNI_TYPE> struct Fenwick {
private:
    size_t SIZE;
    vector <UNI_TYPE> fen, old, tmp;
    vector <int> id;
    vector <bool> used;
    UNI_TYPE get(int i) {
        UNI_TYPE sum = 0;
        for (; i >= 0; i = (i & (i + 1)) - 1)
            sum += fen[i];
        return sum;
    }
    UNI_TYPE get(int l, int r) {
        return get(r) - (l ? get(l - 1) : 0);
    }
    void upd(int i, UNI_TYPE x) {
        for (; i < SIZE; i |= (i + 1))
            fen[i] += x;
    }
    void update() {
        while (!id.empty()) {
            int p = id.back();
            id.pop_back();
            if (p < SIZE && used[p]) {
                upd(p, tmp[p] - old[p]);
                old[p] = tmp[p];
                used[p] = false;
            }
        }
    }
public:
    UNI_TYPE &operator[](int i) {
        id.push_back(i);
        used[i] = true;
        return tmp[i];
    }
    UNI_TYPE operator() (int l, int r) {
        update();
        return get(l, r);
    }
    bool empty() { return (SIZE > 0); }
    size_t size() { return SIZE; }
    UNI_TYPE back() { return tmp.back(); }
    void push_back(UNI_TYPE X) {
        old.push_back(X);
        tmp.push_back(X);
        fen.push_back(X);
        used.push_back(false);
        upd(SIZE, X);
        SIZE++;
    }
    void pop_back() {
        if (SIZE > 0) {
            fen.pop_back();
            old.pop_back();
            tmp.pop_back();
            used.pop_back();
            SIZE--;
        }
    }
    void clear() {
        id.clear();
        tmp.clear();
        old.clear();
        used.clear();
        fen.clear();
    }
    void rev(int l, int r) {
        reverse(tmp.begin() + l, tmp.begin() + r + 1);
        old = tmp;
        used.assign(SIZE, false);
        fen.assign(SIZE, 0);
        id.clear();
        for (int i = 0; i < SIZE; i++)
            upd(i, tmp[i]);
    }
    void sort(int l, int r) {
        sort(tmp.begin() + l, tmp.begin() + r + 1);
        old = tmp;
        used.assign(SIZE, false);
        fen.assign(SIZE, 0);
        id.clear();
        for (int i = 0; i < SIZE; i++)
            upd(i, tmp[i]);
    }
    Fenwick() {}
    Fenwick(unsigned int SIZE = 0, UNI_TYPE X = 0) {
        this->SIZE = SIZE;
        fen.resize(SIZE, 0);
        old.resize(SIZE, X);
        tmp.resize(SIZE, X);
        used.resize(SIZE, false);
        for (int i = 0; i < SIZE; i++)
            upd(i, X);
    }
    ~Fenwick() = default;
};
