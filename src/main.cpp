#include <iostream>
#include "bankers.hpp"
#include "process.hpp"

static void print_vec(const std::vector<int>& v) {
    std::cout << "[";
    for (size_t i=0;i<v.size();i++) {
        std::cout << v[i] << (i+1==v.size()? "" : ",");
    }
    std::cout << "]";
}

int main() {
    // Example: 3 resource types
    Bankers b({3, 3, 2});

    Process p1(1,0,5,2,{1,1,0});
    Process p2(2,0,3,3,{2,1,1});
    Process p3(3,0,4,1,{1,1,1});

    auto s1 = b.request_resources(p1.pid, p1.max_need);
    std::cout << "P1 request " << (s1 ? "GRANTED" : "BLOCKED") << " avail="; print_vec(b.available()); std::cout << "\n";

    auto s2 = b.request_resources(p2.pid, p2.max_need);
    std::cout << "P2 request " << (s2 ? "GRANTED" : "BLOCKED") << " avail="; print_vec(b.available()); std::cout << "\n";

    auto s3 = b.request_resources(p3.pid, p3.max_need);
    std::cout << "P3 request " << (s3 ? "GRANTED" : "BLOCKED") << " avail="; print_vec(b.available()); std::cout << "\n";

    if (s3) {
        std::cout << "Safe sequence now: ";
        for (int pid : *s3) std::cout << pid << " ";
        std::cout << "\n";
    } else {
        std::cout << "No safe sequence if granting P3 right now.\n";
    }

    // simulate finishing P1 and releasing
    b.release_all(p1.pid);
    std::cout << "After P1 finishes, avail="; print_vec(b.available()); std::cout << "\n";

    return 0;
}

