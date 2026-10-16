#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <algorithm>


bool equal_digits(std::string current) {
    for (int i = 0; i < (int)current.size(); i++) {
        for (int j = i + 1; j < (int)current.size(); j++) {
            if (current[i] == current[j]) {
                return true;
            }
        }
    }
    return false;
};

const std::vector<std::string>& possibleNums() {
    static std::vector<std::string> possible;
    if (!possible.empty()) return possible;
    for (int i = 0; i < 10000; i++) {
        std::string num = std::to_string(i);
        num.insert(0, 4-num.size(), '0');

        if (!equal_digits(num)) {
            possible.push_back(num);
        }
    }
    return possible;
}

class Game {
 public:
    Game () : rnd(std::random_device{}()) {
        possible = possibleNums();
        secret = possible[rnd()%possible.size()];
        gameRun = false;
    }

    void run() {
        gameRun = true;
        std::cout << "Это игра Быки и Коровы. Сейчас загадайте четырехзначное число,\n"
             << "у которого все цифры отличаются. Компьютер тоже загадал. Можете пытаться угадать.\n"
             << "Удачи!\n\n";
        int move = 0;
        while (gameRun) {
            if (move % 2 == 0) {
                playerMove();
            } else {
                computerMove();
            }
            move++;
        }
    }

 private:
    std::mt19937 rnd;
    std::string secret;
    std::vector<std::string> possible;
    bool gameRun;

    void playerMove() {
        std::string input;
        std::cout << "Попытайтесь угадать число компьютера. Вам выведут сколько быков и коров.\n";
        while (std::cin >> input) {
            if (!std::all_of(input.begin(), input.end(),
                       [](char c) { return std::isdigit(c); })) {
                std::cout << "Неправильный ввод: вводи цифры\n";
                continue;
            }
            if (equal_digits(input)) {
                std::cout << "Неправильный ввод: надо разные цифры\n";
                continue;
            }
            if (input.size() != 4) {
                std::cout << "Неправильный ввод: длина должна быть 4\n";
                continue;
            }
            break;
        }
        auto [bulls, cows] = count(input, secret);
        std::cout << "Быков: " << bulls << " Коров: " << cows << '\n';
        if (bulls == 4) {
            std::cout << "You win!\n";
            gameRun = false;
        }
    };

    std::array<int, 2> count(std::string given, std::string real) {
        int bulls = 0, cows = 0;
        int n = given.size();
        for (int i = 0; i < n; i++) {
            if (given[i] == real[i]) {
                bulls += 1;
            }
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                if (given[i] == real[j]) {
                    cows += 1;
                }
            }
        }

        return {bulls, cows};
    }

    void computerMove() {
        std::string query = possible[rnd()%possible.size()];
        std::cout << "Мой ход: " << query << '\n';
        std::cout << "Напишите сколько быков(совпали позиция и значение) и коров(совпали только значения)" << '\n';
        int bulls, cows;
        while (std::cin >> bulls >> cows) {
            if (bulls < 0 or cows < 0 or bulls + cows > 4) {
                std::cout << "Такого не могло получиться, попробуй еще раз.\n";
                continue;
            }
            break;
        }
        possible.erase(std::find(possible.begin(), possible.end(), query));
        for (int i = 0; i < (int)possible.size();) {
            if (count(query, possible[i]) != std::array<int,2>{bulls, cows}) {
                possible.erase(possible.begin() + i);
            } else {
                i++;
            }
        }
        if (bulls == 4) {
            std::cout << "Computer win\n";
            gameRun = false;
            return;
        }
        if (possible.size() == 0) {
            std::cout << "Но...такого не может быть....Я и представить не могу что ты загадал\n";
            gameRun = false;
            return;
        }
        std::cout << '\n';
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}