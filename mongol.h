#ifndef MONGOL_H
#define MONGOL_H
//==============================================================
//  mongol.h  — Linux-д тохируулсан монголоор бичих сан
//==============================================================

#include <iostream>
#include <string>

// Linux дээр UTF-8 консол тохиргоо шаардлагагүй (default UTF-8)
struct _MongolSetup {
    _MongolSetup() {
        // Linux: UTF-8 default, тусгай тохиргоо хэрэггүй
    }
};
static _MongolSetup _mongol_setup_instance;

inline void хэвлэ(const std::string& текст) {
    std::cout << текст;
}

inline void мөр(const std::string& текст = "") {
    std::cout << текст << "\n";
}

template <typename T>
inline void тоо(T утга) {
    std::cout << утга;
}

inline std::string мөр_унш() {
    std::string s;
    std::getline(std::cin, s);
    return s;
}

inline std::string асуу(const std::string& асуулт) {
    std::cout << асуулт;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

inline int тоо_унш() {
    int x;
    std::cin >> x;
    std::cin.ignore();
    return x;
}

inline void хүлээ() {
    std::cout << "\nҮргэлжлүүлэхийн тулд Enter дарна уу...";
    std::cin.get();
}

#endif // MONGOL_H
