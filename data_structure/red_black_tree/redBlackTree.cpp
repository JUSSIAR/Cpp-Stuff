#include <iostream>
#include <fstream>
#include <typeinfo>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <functional>
#include <assert.h>

void long_operation(int kol_sec);

template <typename key, typename value> 
class Map {

public:
    typedef key key_type;
    typedef value map_type;
    typedef std::pair<key, value> value_type;

    enum Color {
        RED,
        BLACK
    };

    struct Node {
        Color color;
        Node* prev;
        Node* next;
        Node* left;
        Node* right;
        Node* parent;
        value_type val;
        Node() :
            left(nullptr),
            right(nullptr),
            prev(nullptr),
            next(nullptr),
            parent(nullptr),
            color(Color::RED)
        {}
        Node(value_type _val) :
            left(nullptr),
            right(nullptr),
            prev(nullptr),
            next(nullptr),
            parent(nullptr),
            color(Color::RED),
            val(_val)
        {}
        ~Node() = default;
    };

    struct Iterator {
        Node* ptr;
        Iterator() = delete;
        Iterator(Node* _ptr) : ptr(_ptr) {}
        Iterator(const Iterator& other) :
            ptr(new Node(*(other.ptr)))
        {}
        ~Iterator() = default;
        Iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        Iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }
        Iterator operator++(int) {
            Iterator copy(*this);
            operator++();
            return copy;
        }
        Iterator operator--(int) {
            Iterator copy(*this);
            operator--();
            return copy;
        }
        value_type& operator*() const {
            return ptr->val;
        }
        Node* operator->() const {
            return ptr;
        }
        bool operator==(const Iterator& other) const {
            return (ptr == other.ptr);
        }
        bool operator!=(const Iterator&  other) const {
            return !operator==(other);
        }
    };

    Iterator begin() {
        return head->next;
    }
    Iterator end() {
        return tail;
    }

    Map() : root(nullptr) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }

    Iterator Find(const key_type& k) {
        return __find__(root, k);
    }

    map_type& operator[](const key_type& k) {
        std::pair<Iterator, bool> p = Insert(std::make_pair(k, map_type()));
        return p.first->val.second;
    }

    std::pair<Iterator, bool> Insert(const value_type& v) {
        Node* NewNode = new Node(v);
        std::pair<Iterator, bool> p = __Recursive_insert__(root, NewNode);
        if (p.second) {
            __Repair_tree__(NewNode);
            root = NewNode;
            while (Get_parent(root) != nullptr)
                root = Get_parent(root);
        } else {
            delete NewNode;
        }
        return p;
    }

    size_t Erase(const key_type& k) {
        Iterator it = __find__(root, k);
        if (it == end())
            return 0;
        Node* v_min = Get_smallval_node(it->right);
        std::swap(v_min->val, it->val);
        __replace_node__(v_min, v_min->right);
        Remove(v_min);
    }

private:

    Node* root;
    Node* head;
    Node* tail;

private:

    void Remove(Node* node) {
        Node* child = node->right;
        __replace_node__(node, node->right);
        if (node->color == Color::BLACK && child->color == Color::RED) {
            child->color = Color::Red;
        } else if (node->color == Color::RED && child->color == Color::BLACK) {
            // no any actions
        } else {
            
        }
        delete node;
    }

    void __remove_repair_tree__(Node* node) {
        if (node->parent == nullptr) return;
        
        //__remove_case_1__(node);

    }

    void __remove_case_1__(Node* node) {
        Node* s = Get_Sibling(node);
        Node* sl = s->left;
        Node* sr = s->right;

        if (sl->color == Color::BLACK && sr->color == Color::BLACK &&
            node->parent->color == Color::BLACK && s->color == Color::RED) 
        {
            node->parent->color = Color::RED;
            s->color = Color::BLACK;
            if (node->parent->left != nullptr) {
                __left_rotate__(node->parent);
            } else {
                __right_rotate__(node->parent);
            }
        }
        __remove_case_2__(node);
    }

    void __remove_case_2__(Node* node) {
        Node* s = Get_Sibling(node);
        Node* sl = s->left;
        Node* sr = s->right;

        if (sl->color == Color::BLACK && sr->color == Color::BLACK &&
            node->parent->color == Color::BLACK && s->color == Color::BLACK) 
        {
            s->color = Color::RED;
            __remove_repair_tree__(node);
        }
        __remove_case_3__(node);
    }

    void __remove_case_3__(Node* node) {
        Node* s = Get_Sibling(node);
        Node* sl = s->left;
        Node* sr = s->right;

        if (sl->color == Color::BLACK && sr->color == Color::BLACK &&
            node->parent->color == Color::RED && s->color == Color::BLACK) 
        {
            std::swap(node->parent->color, s->color);
        }
        __remove_case_4__(node);
    }

    void __remove_case_4__(Node* node) {
        //__remove_case_4__
    }

    void __remove_case_5__(Node* node) {
        //__remove_case_5__
    }

    void __remove_case_6__(Node* node) {
        //__remove_case_6__
    }

    void __replace_node__(Node* node, Node* child) {
        child->parent = node->parent;
        if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
    }

    Iterator __find__(Node* cur, const key_type& k) {        
        if (cur == nullptr)
            return end();        
        if (cur->val.first == k)
            return Iterator(cur);
        else if (k < cur->val.first)
            return __find__(cur->left, k);
        else
            return __find__(cur->right, k);
    }

    Node* Get_parent(Node* temp_vert) {
        return (temp_vert == nullptr) ? nullptr : temp_vert->parent;
    }

    Node* Get_GrandParent(Node* node) {
        return Get_parent(Get_parent(node));
    }

    Node* Get_Sibling(Node* node) {
        if (Get_parent(node) == nullptr)
            return nullptr;
        return (node->parent->left == node) ? node->parent->right : node->parent->left;
    }

    Node* Get_uncle(Node* node) {
        return Get_Sibling(Get_parent(node));    
    }

    Node* Get_bigval_node(Node* temp) {
        return (temp->right == nullptr) 
            ? temp 
            : Get_bigval_node(temp->right);
    }

    Node* Get_smallval_node(Node* temp) {
        return (temp->left == nullptr) 
            ? temp 
            : Get_smallval_node(temp->left);
    }


    // 1. node -> root
    // 2. parent: Black
    // 3. parent: Red, uncle: Red
    // 4. parent: Red, uncle: Black  

    std::pair<Iterator, bool> __Recursive_insert__(Node* place, Node* who) {
        if (place != nullptr) {
            if (place->val.first == who->val.first) {
                return std::make_pair(Iterator(place), false);
            }
            if (place->val.first > who->val.first) {
                if (place->left == nullptr) {
                    place->left = who;
                    who->parent = place;
                } else {
                    return __Recursive_insert__(place->left, who);
                }
            } else {
                if (place->right == nullptr) {
                    place->right = who;
                    who->parent = place;
                } else {
                    return __Recursive_insert__(place->right, who);
                }
            }
        }
        who->color = Color::RED;
        who->parent = place;
        who->left = nullptr;
        who->right = nullptr;
        if (who->parent == nullptr) {
            who->next = tail;
            who->prev = head;
            head->next = who;
            tail->prev = who;
        } else {
            //parent < node
            if (who->parent->val.first < who->val.first) {
                Node* successor = who->parent;
                while (successor != tail && successor->val.first < who->val.first) {
                    successor = successor->next;
                }                
                who->next = successor;
                who->prev = successor->prev;
                successor->prev = who;
                who->prev->next = who;
            } 
            // node < parent
            else {
                Node* predecessor = who->parent;
                while (predecessor != head && predecessor->val.first > who->val.first) {
                    predecessor = predecessor->prev;
                }
                who->next = predecessor->next;
                who->prev = predecessor;
                predecessor->next = who;
                who->next->prev = who;
            }
        }
        return std::make_pair(Iterator(who), true);
    }

    void __left_rotate__(Node* node) {
        Node* left = node->left;
        Node* right = node->right;
        Node* ch1 = right->left;
        Node* ch2 = right->right;
        Node* par = node->parent;
        /* for_rigth */ {
            right->left = node;
            right->parent = par;
        }
        /* for_node */ {
            node->parent = right;
            node->right = ch1;
        }
        /* for_par */ {
            if (par != nullptr) {
                if (par->left == node)
                    par->left = right;
                else
                    par->right = right;
            }
        }
        /* for_ch1 */ {
            if (ch1 != nullptr) {
                ch1->parent = node;
            }
        }
    }

    void __right_rotate__(Node* node) {
        Node* left = node->left;
        Node* right = node->right;
        Node* ch1 = left->left;
        Node* ch2 = left->right;
        Node* par = node->parent;
        /* for_left */ {
            left->right = node;
            left->parent = par;
        }
        /* for_node */ {
            node->parent = left;
            node->left = ch2;
        }
        /* for_par */ {
            if (par != nullptr) {
                if (par->left == node)
                    par->left = left;
                else
                    par->right = left;
            }
        }
        /* for_ch2 */ {
            if (ch2 != nullptr) {
                ch2->parent = node;
            }
        }
    }

    Node* __inside__(Node* node) {
        auto GP = Get_GrandParent(node);
        auto P = Get_parent(node);
        if (GP == nullptr)
            return node;
        if (GP->left == P && P->right == node) {
            __left_rotate__(P);
            node = node->left;
        }
        if (GP->right == P && P->left == node) {
            __right_rotate__(P);
            node = node->right;
        }
        return node;
    }

    void __outside__(Node* node) {
        auto GP = Get_GrandParent(node);
        auto P = Get_parent(node);
        if (P->left == node) {
            __right_rotate__(GP);
        } else {
            __left_rotate__(GP);
        }
        std::swap(P->color, GP->color);
    }

    void __Repair_tree__(Node* node) {
        if (Get_parent(node) == nullptr) {
            node->color = Color::BLACK;
            return;
        }
        if (node->parent->color == Color::BLACK) {
            return;
        }
        if (Get_uncle(node) != nullptr && Get_uncle(node)->color == Color::RED) {
            Get_GrandParent(node)->color = Color::RED;
            Get_parent(node)->color = Color::BLACK;
            Get_uncle(node)->color = Color::BLACK;
            return;
        }
        node = __inside__(node);
        __outside__(node);
    }
};



namespace test {
    inline void test_case_map() {
        std::cout << std::endl << "test_case_map\n" << std::endl;	


        /*
            this version has memory leaks
        */	
        
        Map<int, std::string> myMap;

        auto r1 = myMap.Insert(std::make_pair(1, "a"));
        long_operation(1);
        std::cout << r1.second << "added 1" << std::endl;

        auto r2 = myMap.Insert(std::make_pair(3, "b"));
        long_operation(1);
        std::cout << r2.second << "added 2" << std::endl;

        auto r3 = myMap.Insert(std::make_pair(2, "c"));
        long_operation(1);
        std::cout << r3.second << "added 3" << std::endl;

        auto r4 = myMap.Insert(std::make_pair(5, "d"));
        long_operation(1);
        std::cout << r4.second << "added 4" << std::endl;

        auto r5 = myMap.Insert(std::make_pair(-7, "e"));
        long_operation(1);
        std::cout << r5.second << "added 5" << std::endl;


        std::cout << "\n\n\n" << std::endl;
        long_operation(2);
        for (Map<int, std::string>::Iterator it = myMap.begin();
            it != myMap.end(); it++) {
                std::cout << it->val.second << '\t';
            }
        std::cout << '\n';
        std::cout << "out tested\n\n" << std::endl; 

        std::cout << "value on key 3 is ";
        long_operation(1);
        std::cout << myMap[3] << std::endl;

        // long_operation(1);
        // myMap[25] = "abacaba";
        // std::cout << "added new key 25 with value ";
        // std::cout << myMap[25] << "\n\n\n" << std::endl;
        
        
        long_operation(2);
        std::cout << std::endl << "[---OK---]" << std::endl;
    }
}

int main()
{
    test::test_case_map();
    return 0;
}

void long_operation(int kol_sec) {
    assert(kol_sec >= 0 && kol_sec < 100);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * kol_sec));
}
