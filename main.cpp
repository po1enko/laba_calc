#include <iostream>
#include <stack>
#include <cmath>
#include <cctype>

using namespace std;

bool is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int get_precedence(char op) {
    if (op == '^')
        return 3;
    else if (op == '*' || op == '/')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    else
        return 0;
}

double apply_operator(double a, double b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b != 0)
                return a / b;
            else {
                cout << "oshibka - na 0 delit nelzya" << endl;
                exit(EXIT_FAILURE);
            }
        case '^':
            return pow(a, b);
        default:
            cout << "oshibka" << endl;
            exit(EXIT_FAILURE);
    }
}

double eval_expr(const string& expr) {
    stack<double> values;
    stack<char> operators;

    for (size_t i = 0; i < expr.length(); ++i) {
        if (isspace(expr[i])) {
            continue; // Пропускаем пробелы
        } else if (isdigit(expr[i]) || (expr[i] == '-' && (i == 0 || !isdigit(expr[i - 1])))) {
            size_t j = i;
            while (j < expr.length() && (isdigit(expr[j]) || expr[j] == '.' || expr[j] == '-')) {
                ++j;
            }
            values.push(stod(expr.substr(i, j - i)));
            i = j - 1;
        } else if (expr[i] == '(') {
            operators.push('(');
        } else if (expr[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(apply_operator(a, b, op));
            }
            operators.pop(); // Удаляем '(' из стека операторов
        } else if (is_operator(expr[i])) {
            while (!operators.empty() && get_precedence(operators.top()) >= get_precedence(expr[i])) {
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(apply_operator(a, b, op));
            }
            operators.push(expr[i]);
        } else {
            cout << "oshibka - nedopustimiy simvol v vyrajenii" << endl;
            exit(EXIT_FAILURE);
        }
    }

    while (!operators.empty()) {
        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = operators.top();
        operators.pop();
        values.push(apply_operator(a, b, op));
    }

    return values.top();
}

int main() {
    cout << "vvedite vyrajenie ";
    string expression;
    getline(cin, expression);

    double result = eval_expr(expression);
    cout << "resultat: " << result << endl;

    return 0;
}