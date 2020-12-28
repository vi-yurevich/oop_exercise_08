#ifndef OOP_EXERSICE_8_PUBSUB_H
#define OOP_EXERSICE_8_PUBSUB_H

#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <mutex>
#include <thread>

template <class M>
class TServer {
    private:
        TServer(){};
        //TServer(const TServer& ob) = delete;
        //TServer& operator=(const TServer&) = delete;
        std::vector<std::function<void(M&)>> sub;
        std::queue<M> messageQue;
        std::mutex mutex;
    public:
        using sub_t = std::function<void(M&)>;
        static TServer& Get() {
            static TServer ob;
            return ob;
        }
        void AddSub(const sub_t &s) {
            std::lock_guard<std::mutex> lock(mutex);
            sub.push_back(s);
        }
        void MakePub(const M &m) {
            std::lock_guard<std::mutex> lock(mutex);
            messageQue.push(m);
        }
        void Launch() {
            while (!false) {
                if (!messageQue.empty()) {
                    std::lock_guard<std::mutex> lock(mutex);
                    M figure = messageQue.front();
                    if (figure.empty()) {
                        break;
                    }
                    messageQue.pop();
                    for (auto s : sub) s(figure);
                } else {
                    std::this_thread::yield();
                }
            }
        }
};

#endif //OOP_EXERSICE_8_PUBSUB_H