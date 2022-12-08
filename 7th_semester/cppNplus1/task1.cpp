#include <map>
#include <iostream>
#include <ctime>

int main(int argc, char **argv) {
    
    
    std::cout << "Пример, когда нам придется записать несколько раз одну и ту же пару в словарь в разных частях программы. \n" <<
                 "В таком случае мы можем запомнить итератор на этот элемент. \n";
    for (int j = 0; j <= 3; j++) {
        if (true) {
        std::map<int, int> map;
        clock_t begin = clock();
        for (int i = 0; i <= 50000; i++) {
            const auto [it_i, success] = map.insert({i, 0});
            /* Какой-то код здесь */
            map.insert(*it_i);
            /* Какой-то код здесь */
            map.insert(*it_i);
            /* Какой-то код здесь */
            map.insert(*it_i);
            /* Какой-то код здесь */
            map.insert(*it_i);
        }
        clock_t end = clock();
        std::cout << "Первый цикл: " << double(end - begin) / CLOCKS_PER_SEC * 1000 << "\n";
        }  

        if (true) {
        std::map<int, int> map;
        clock_t begin2 = clock();
        for (int i = 0; i <= 50000; i++) {
            const auto [it_i, success] = map.insert({i, 0});
            /* Какой-то код здесь */
            map.insert(it_i, *it_i);
            /* Какой-то код здесь */
            map.insert(it_i, *it_i);
            /* Какой-то код здесь */
            map.insert(it_i, *it_i);
            /* Какой-то код здесь */
            map.insert(it_i, *it_i);
            /* Какой-то код здесь */
            map.insert(it_i, *it_i);
        }
        clock_t end2 = clock();
        std::cout << "Второй цикл: " <<  double(end2 - begin2) / CLOCKS_PER_SEC * 1000 << "\n";
        }
    }

    return 0;
}
