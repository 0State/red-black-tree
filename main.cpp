#include <iostream>
#include "tree.h"
#include <iostream>
#include <ctime>
#include <set>

int main(int argc, const char * argv[]) {
    RBTree<int> tree;
    std::set<int> t;
    clock_t start1, start2,start3, stop1, stop2, stop3;
    double time1, time2, time3;
    
    start2 = clock();
    for(int i = 1; i <= 1000000; i++) {
        t.insert(i);
    }
    stop2 = clock();
    time2 = (stop2 - start2)/(double)CLOCKS_PER_SEC;
    std::cout << "Insertion for std::set t took " << 
	time2 << " seconds.\n\n";
    
    start1 = clock();
    for(int i = 1; i <= 1000000; i++) {
        tree.insert(i);
    }
    stop1 = clock();
    time1 = (stop1 - start1)/(double)CLOCKS_PER_SEC;
    std::cout << "Insertion for RBTree<T> tree took " << 
	time1 << " seconds.\n\n";
    

    tree.find(500000);
    tree.find(1000001);
   
    start3 = clock();
    for(int i = 1; i <= 1000000; i++) {
      if(i % 2 == 1)
        tree.remove(i);
    }
    stop3 = clock();
    time3 = (stop3 - start3)/(double)CLOCKS_PER_SEC;
    std::cout << "Deleting odd intergers took " << time3 
              << " seconds.\n\n";
    //tree.print();
    return 0;
}

