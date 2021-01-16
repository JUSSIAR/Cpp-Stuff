#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#define memoryKiller(type, var) (type)var

namespace my_std {
    template <typename Key, typename Type, typename Hash=std::hash<Key>>
    class unordered_map {
        protected:
            using key_type = Key;
            using map_type = Type;
            using value_type = std::pair<Key, Type>;
        private:
            struct Node {
                value_type value;
                Node* next;
                Node* prev;
                Node() = default;
                Node(const Node& other) = default;
                Node(value_type _value) : value(_value) {}
                ~Node() = default;
            };
        public:
            unordered_map(std::size_t backet_count = 100) : table(backet_count) {
                head.next = &tail;
                tail.prev = &head;
                _size = 0;
            }
            class iterator {
                public:
                    iterator() = default;
                    iterator(Node* new_elem) {
                        ptr = new_elem;
                    }
                    iterator& operator++() {
                        ptr = ptr->next; 
                        return *this;
                    }
                    iterator operator++(int) {
                        iterator copy(*this);
                        operator++();
                        return copy;
                    }
                    bool operator==(const iterator& other) const {
                        return ptr == other.ptr;
                    }
                    bool operator!=(const iterator& other) const {
                        return !operator==(other);
                    }
                    value_type operator*() {
                        return ptr->value;
                    }
                    value_type* operator->() {
                        return &(ptr->value);
                    }
                private:
                    Node* ptr;
            };
            std::size_t size() const {
                return _size;
            }
            std::size_t bucket_count() const {
                return table.size();
            }
            bool empty() const {
                return size() == 0;
            }
            float load_factor() const {
                return empty() ? 0 : static_cast<float>(_size) / bucket_count(); 
            }
            float max_load_factor() const noexcept {
                return _max_load_factor;
            }
            void max_load_factor(float new_mlf) {
                _max_load_factor = new_mlf;
            }
            iterator begin() {
                return head.next;
            }
            iterator end() {
                return &tail;
            }
            std::pair<iterator, bool> insert(const value_type& value) {
                const std::size_t bucket_pos = hash(value.first) % bucket_count();
                auto it = std::find_if(table[bucket_pos].begin(), table[bucket_pos].end(), [&](const Node& node) {
                    return node.value.first == value.first;
                });
                if (it == table[bucket_pos].end()) {
                    table[bucket_pos].push_back(Node(value));
                    it = table[bucket_pos].end(); --it;
                    auto nxt = get_next(bucket_pos, it);
                    auto prv = get_prev(bucket_pos, it);
                    it->next = nxt;
                    it->prev = prv;
                    nxt->prev = &(*it);
                    prv->next = &(*it);
                    iterator res(&(*it));
                    return std::make_pair(res, true);
                }
                it->value.second = value.second;
                return std::make_pair(iterator(&(*it)), false);
            }
            // void erase(const value_type& value) {
                
            // } 
        private:
            //Node* get_next(std::size_t pos, typename std::list<Node>::iterator iter) {
            template <typename some_iterator>
            Node* get_next(std::size_t pos, some_iterator iter) {
                if (++iter != table[pos++].end())
                    return &(*iter);
                while (pos < bucket_count() && table[pos].empty())
                    ++pos;
                if (pos == bucket_count())
                    return &tail;
                return &table[pos].front();
            }
            //Node* get_prev(std::size_t pos, typename std::list<Node>::iterator iter) {
            template <typename some_iterator>
            Node* get_prev(std::size_t pos, some_iterator iter) {
                if (iter != table[pos].begin())
                    return &(*(--iter));
                while (pos >= 1 && table[pos - 1].empty())
                    --pos;
                if (pos == 0)
                    return &head;
                return &table[pos - 1].back();
            }
            float _max_load_factor = 290;
            std::size_t _size;
            Node head;
            Node tail;
            Hash hash;
            std::vector<std::list<Node>> table;
    };
}

namespace test {

    inline void test_case1() {

        my_std::unordered_map<int, std::string> my_map;
        my_map.insert(std::make_pair(1, "a"));
        my_map.insert(std::make_pair(2, "b"));
        my_map.insert(std::make_pair(3, "c"));
        my_map.insert(std::make_pair(1, "d"));
        for (auto it = my_map.begin(); it != my_map.end(); it++) {
            std::cout << it->second << '\t';
        }
        std::cout << std::endl << "OK" << std::endl;

    }
    
}

int main() {
    
    test::test_case1();    

    return 0;
}
