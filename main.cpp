#include "timer_service.h"

// Example class to demonstrate member function callbacks
class MessageHandler {
public:
    MessageHandler(const std::string& name) : name_(name) {}
    
    // Member function that will be called by timer
    void onTimeout(const std::string& message) {
        std::cout << "   [" << name_ << "] Received timeout message: " << message << "\n";
    }
    
    // Another member function with different argument type
    void onValueTimeout(int value) {
        std::cout << "   [" << name_ << "] Received timeout value: " << value << "\n";
    }
    
private:
    std::string name_;
};

// Example usage and test cases
int main() {
    TimerService timer_service;
    
    std::cout << "Timer Service Demo\n";
    std::cout << "==================\n\n";
    
    // Example 1: Timer with int argument
    std::cout << "1. Scheduling timer with int argument...\n";
    auto id1 = timer_service.schedule(
        std::chrono::milliseconds(1000),
        [](int value) {
            std::cout << "   Timer 1 fired! Value: " << value << "\n";
        },
        42,
        3
    );
    std::cout << "   Timer ID: " << id1 << "\n\n";
    
    // Example 2: Timer with string argument
    std::cout << "2. Scheduling timer with string argument...\n";
    std::string message = "Hello from timer!";
    auto id2 = timer_service.schedule(
        std::chrono::milliseconds(1500),
        [](const std::string& msg) {
            std::cout << "   Timer 2 fired! Message: " << msg << "\n";
        },
        message
    );
    std::cout << "   Timer ID: " << id2 << "\n";
    std::cout << "   (Original string can be destroyed now)\n\n";
    
    // Example 3: Timer with custom struct
    struct Point {
        int x, y;
    };
    
    std::cout << "3. Scheduling timer with custom struct...\n";
    auto id3 = timer_service.schedule(
        std::chrono::milliseconds(2000),
        [](Point p) {
            std::cout << "   Timer 3 fired! Point: (" << p.x << ", " << p.y << ")\n";
        },
        Point{10, 20}
    );
    std::cout << "   Timer ID: " << id3 << "\n\n";
    
    // Example 4: Cancelled timer
    std::cout << "4. Scheduling and then cancelling a timer...\n";
    auto id4 = timer_service.schedule(
        std::chrono::milliseconds(500),
        [](int value) {
            std::cout << "   Timer 4 fired! (This should NOT print)\n";
        },
        999
    );
    std::cout << "   Timer ID: " << id4 << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    bool cancelled = timer_service.cancel(id4);
    std::cout << "   Timer cancelled: " << (cancelled ? "Yes" : "No") << "\n\n";
    
    // Example 5: Multiple timers with same type
    std::cout << "5. Scheduling multiple timers...\n";
    for (int i = 0; i < 3; ++i) {
        timer_service.schedule(
            std::chrono::milliseconds(2500 + i * 200),
            [](int num) {
                std::cout << "   Batch timer " << num << " fired!\n";
            },
            i
        );
    }
    std::cout << "   Scheduled 3 timers\n\n";
    
    // Example 6: Member function callbacks
    std::cout << "6. Scheduling timers with member function callbacks...\n";
    
    MessageHandler handler1("Handler-1");
    MessageHandler handler2("Handler-2");
    
    // Method 1: Using std::bind to bind member function to object
    auto id6a = timer_service.schedule(
        std::chrono::milliseconds(3000),
        std::bind(&MessageHandler::onTimeout, &handler1, std::placeholders::_1),
        std::string("Timeout via std::bind")
    );
    std::cout << "   Timer ID (bind): " << id6a << "\n";
    
    // Method 2: Using lambda to capture object and call member function
    auto id6b = timer_service.schedule(
        std::chrono::milliseconds(3200),
        [&handler2](const std::string& msg) {
            handler2.onTimeout(msg);
        },
        std::string("Timeout via lambda capture")
    );
    std::cout << "   Timer ID (lambda): " << id6b << "\n";
    
    // Method 3: Using lambda with different member function
    auto id6c = timer_service.schedule(
        std::chrono::milliseconds(3400),
        [&handler1](int val) {
            handler1.onValueTimeout(val);
        },
        777
    );
    std::cout << "   Timer ID (lambda with int): " << id6c << "\n\n";
    
    std::cout << "Pending timers 1" << "\n\n";
    std::cout << "Waiting for timers to fire...\n\n";
    
    // Wait for all timers to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    
    std::cout << "\nPending timers 2" << "\n";
    std::cout << "Demo complete!\n";
    
    return 0;
}
