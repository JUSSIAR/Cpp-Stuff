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

template <typename TYPE, size_t SIZE>
class array {
    public:    
        typedef TYPE* ptr;
        typedef TYPE& ref;
        typedef TYPE value;

        template <typename ... U>
        array(U... val_list) : arr{val_list...} {}
        ~array() = default;

        ref operator[](size_t index) {
            return arr[index];
        }

        class iterator {
            public:
                iterator(ptr _elem) : element(_elem) {}
                iterator(const iterator& other) = default;
                ~iterator() = default;
                bool operator ==(const iterator& other) const {
                    return element == other.element;
                }
                bool operator !=(const iterator& other) const {
                    return !operator==(other);
                }
                value operator[](int index) const {
                    return element[index];
                }
                iterator& operator+=(int kol_val) {
                    element += kol_val;
                    return *this;
                }
                iterator& operator-=(int kol_val) {
                    element -= kol_val;
                    return *this;
                }
                iterator& operator++() {
                    return operator+=(1);
                }
                iterator& operator--() {
                    return operator-=(1);
                }
                iterator operator++(int) {
                    iterator copy(*this);
                    ++(*this);
                    return copy;
                }
                iterator operator--(int) {
                    iterator copy(*this);
                    --(*this);
                    return copy;
                }
                ref operator*() {
                    return *element;
                }
                ptr operator->() const {
                    return element;
                }
            private:
                ptr element;
        };

        iterator begin() {
            return arr;
        }
        iterator end() {
            return (arr + SIZE);
        }
        
    private:
        value arr[SIZE];
};


namespace test {
    void test_case_iterator() {
	std::cout << std::endl << "test_case_iterator\n" << std::endl;		
        
        array<int, 4> a = {1, 2, 3, 4}; 
        for (auto it = a.begin(); it != a.end(); it++)
            std::cout << *it << ' ';

        std::cout << std::endl << "[---OK---]" << std::endl;
    }
}

int main()
{   	
    test::test_case_iterator();	

    return 0;
}

void long_operation(int kol_sec) {
    assert(kol_sec >= 0 && kol_sec < 100);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * kol_sec));
}
