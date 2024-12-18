#include "LinkedList.h"


int main() {
    try {

        LinkedList<int> list;
        list.push_tail(1);
        list.push_tail(2);
        list.push_tail(0);
        std::cout << "List: " << list << std::endl;

        LinkedList<int> another_list(5, 10, 50);
        std::cout << "Another list: " << another_list << std::endl;

        list.push_head(another_list);
        std::cout << "Merged list head: " << list << std::endl;

        list.push_tail(another_list);
        std::cout << "Merged list tail: " << list << std::endl;

        list.pop_head();
        list.pop_tail();
        std::cout << "After pops: " << list << std::endl;

        list.delete_node(2);
        std::cout << "After deleting 2s: " << list << std::endl;

        LinkedList<int> copied_list = list;
        std::cout << "Copied list: " << copied_list << std::endl;

        std::cout << "Element at index 1: " << list[1] << std::endl;

        list[1] = 42;
        std::cout << "After modifying index 1: " << list << std::endl;

        std::cout << "List size: " << list.get_size() << std::endl;
        int n = 30;
        LinkedList<int> lucky = lucky_numbers<int>(n);
        LinkedList<int> unlucky = unlucky_numbers(lucky, n);

        std::cout << "Lucky numbers: " << lucky << std::endl;
        std::cout << "Unlucky numbers: " << unlucky << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}

