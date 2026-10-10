#include <iostream>

using namespace std;

// Function to print Fibonacci series up to n terms
void generateFibonacci(int n) {
    if (n <= 0) {
        cout << "Please enter a positive integer." << endl;
        return;
    }

    long long first = 0, second = 1;

    cout << "Fibonacci Series (" << n << " terms): ";
    for (int i = 1; i <= n; ++i) {
        cout << first << (i == n ? "" : ", ");
        long long next = first + second;
        first = second;
        second = next;
    }
    cout << endl;
}

int main() {
    int n = 10; // Default term count
    generateFibonacci(n);
    return 0;
}
