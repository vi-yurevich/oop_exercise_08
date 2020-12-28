// Юревич Виталий М80-207Б
// Вариант 13
// Фигуры: ромб, пятиугольник, шестиугольник
//Задание:
//01. Осуществлять ввод из стандартного ввода данных фигур, согласно варианту задания;
//02. Программа должна создавать классы, соответствующие введенным данных фигур;
//03. Программа должна содержать внутренний буфер, в который помещаются фигуры. Для создания 
//    буфера допускается использовать стандартные контейнеры STL. Размер буфера задается 
//    параметром командной строки. Например, для буфера размером 10 фигур: oop_exercise_08 10
//04. При накоплении буфера они должны запускаться на асинхронную обработку, 
//    после чего буфер должен очищаться;
//05. Обработка должна производиться в отдельном потоке;
//06. Реализовать два обработчика, которые должны обрабатывать данные буфера:
//    a. Вывод информации о фигурах в буфере на экран;
//    b. Вывод информации о фигурах в буфере в файл. Для каждого буфера должен создаваться файл
//    с уникальным именем.
//07. Оба обработчика должны обрабатывать каждый введенный буфер. Т.е. после каждого заполнения
//    буфера его содержимое должно выводиться как на экран, так и в файл.
//08. Обработчики должны быть реализованы в виде лямбда-функций и должны хранится в
//    специальном массиве обработчиков. Откуда и должны последовательно вызываться в
//    потоке – обработчике.
//09. В программе должно быть ровно два потока (thread). Один основной (main)
//    и второй для обработчиков;
//10. В программе должен явно прослеживаться шаблон Publish-Subscribe. 
//    Каждый обработчик должен быть реализован как отдельный подписчик.
//11. Реализовать в основном потоке (main) ожидание обработки буфера в потоке-обработчике. 
//    Т.е. после отправки буфера на обработку основной поток должен ждать, пока поток 
//    обработчик выведет данные на экран и запишет в файл.

#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include <fstream> 
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include "Figure.h"
#include "PubSub.h"

class TFactory {
public:
    static TFactory& Object() {
        static TFactory ob;
        return ob;
    }

    std::shared_ptr<TFigure> Rhomb(double x0, double y0, double h, double v) {
    	return std::make_shared<TRhombus>(x0, y0, h, v);
    }

    std::shared_ptr<TFigure> Pent(double x0, double y0, double s) {
    	return std::make_shared<TPentagone>(x0, y0, s);
    }

    std::shared_ptr<TFigure> Hex(double x0, double y0 , double s) {
    	return std::make_shared<THexagone>(x0, y0, s);
    }

private:
    TFactory() {}
    TFactory(const TFactory& ob) = delete;
    TFactory& operator=(const TFactory&) = delete;
};

void AddFigureToVec(char type, std::vector<std::shared_ptr<TFigure>>& vec) {
    bool key = false;
    double x0, y0, side, vert, hor;
    do {
        std::cout << "Укажите координату Х центра фигуры: ";
        std::cin >> x0;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода. Повторите ввод." << std::endl;
        }
        else { 
            key = true;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (key != true);
    do {
        std::cout << "Укажите координату Y центра фигуры: ";
        std::cin >> y0;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода. Повторите ввод." << std::endl;
        }
        else { 
            key = false;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (key != false);
    switch(type) {
        case 'r': {
            do {
                std::cout << "Укажите длину вертикальной диагонали: ";
                std::cin >> vert;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Ошибка ввода. Повторите ввод." << std::endl;
                }
                else if (vert < 0) {
                            std::cout << "Ошибка ввода. Длина не может быть отрицательной. Повторите ввод.\n";
                        }
                else {
                    key = true;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            } while(key != true);
            do {
                std::cout << "Укажите длину горизонтальной диагонали: ";
                std::cin >> hor;
                if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка ввода. Повторите ввод." << std::endl;
                }
                else if (hor < 0) {
                            std::cout << "Ошибка ввода. Длина не может быть отрицательной. Повторите ввод.\n";
                        }
                else {
                    key = false;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            } while(key != false);
            vec.push_back(TFactory::Object().Rhomb(x0, y0, vert, hor));
            break;
        }
        case 'p': {
            do {
                std::cout << "Укажите длину стороны: ";
                std::cin >> side;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Ошибка ввода. Повторите ввод." << std::endl;
                }
                else if (side < 0) {
                            std::cout << "Ошибка ввода. Длина не может быть отрицательной. Повторите ввод.\n";
                        }
                else {
                    key = true;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            } while(key != true);
            vec.push_back(TFactory::Object().Pent(x0, y0, side));
            break;
        }
        case 'h': {
            do {
                std::cout << "Укажите длину стороны: ";
                std::cin >> side;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Ошибка ввода. Повторите ввод." << std::endl;
                }
                else if (side < 0) {
                            std::cout << "Ошибка ввода. Длина не может быть отрицательной. Повторите ввод.\n";
                        }
                else {
                    key = true;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            } while(key != true);
            vec.push_back(TFactory::Object().Hex(x0, y0, side));
            break;
        }
    }
}

std::ostream& operator<<(std::ostream &os, std::vector<std::shared_ptr<TFigure>> fig) {
    for (auto f: fig) os << std::endl << f->Name() << std::endl << f->Coordinate() << std::endl;
    return os;
}

using fig_t = std::vector<std::shared_ptr<TFigure>>;


int main(int argc, char* argv[]) {
    unsigned long buffSize;
    int menu;
    bool key;
    fig_t vec;

    if (argc < 2) {
		std::cout << "Необходимо в качестве параметра указать размер буфера." << std::endl;
        return(1);
	}
    else buffSize = atoi(argv[1]);
    std::cout << buffSize << std::endl;
    if (buffSize < 1) {
        std::cout << "Размер буфера должен быть больше нуля." << std::endl;
        return(1);
    }

    std::srand(time(NULL));

    TServer<fig_t>::Get().AddSub([] (fig_t &figure) {
        std::cout << figure << std::endl;
        std::cout.flush();
    });

    TServer<fig_t>::Get().AddSub([&buffSize] (fig_t &figure) {
        std::ofstream f;
        std::string name = std::to_string(buffSize);
        for (short i = 0; i < 16; ++i) {
		    name.push_back('a' + std::rand() % 26);
	    }
        name.append(".t");
        f.open(name.c_str()); 
        f << figure;
        f.close();
        std::cout << "Буфер очищен и сохранен в файл " << name << std::endl; 
        std::cout.flush();
    });
    std::thread thread([]() {
        TServer<fig_t>::Get().Launch();
    });
	
    menu = 1;
    key = false;
    while(menu != 0) {
        switch(menu) {
            case 1:
                std::cout << "1 - Вывод меню\n2 - Добавить ромб\n3 - Добавить пятиугольник\n";
                std::cout << "4 - Добавить шестиугольник\n5 - Вывод буфера\n0 - Выход из программы\n";
                break;
            case 2:
                AddFigureToVec('r', vec);
                break;
            case 3:
                AddFigureToVec('p', vec);
                break;
            case 4:
                AddFigureToVec('h', vec);
                break;
            case 5: 
                std::cout << vec;
                break;
        }
        if (vec.size() == buffSize) {
                TServer<fig_t>::Get().MakePub(vec);
                vec.clear();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        do {
            std::cout << "Выберете пункт меню: ";
            std::cin >> menu;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка ввода. Повторите ввод." << std::endl;
            }
            else if ((menu < 0) || (menu > 11)) {
                std::cout << "Указанный пункт отсутствует в меню. Попробуйте ещё раз." << std::endl;
            }
            else {
                key = true;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } while(key != true);
        key = false;
    }
    TServer<fig_t>::Get().MakePub(vec);
    vec.clear();
    TServer<fig_t>::Get().MakePub(vec);
    thread.join();
    return(0);
}
