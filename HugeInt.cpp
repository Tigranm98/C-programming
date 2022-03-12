#include "HugeInt.hpp"

HugeInt::HugeInt(std::string value) {
    digits = "";
    int size = value.size();
    for(int i = size-1; i >= 0; --i) {
        if (!isdigit(value[i]))
        {
            std::cout << "wrong symbol " << value[i] << std::endl;
        }
        digits.push_back(value[i] - '0');
    }
}

HugeInt::HugeInt(int value) {
    do {
        digits.push_back(value % 10);
        value /= 10;
    } while (value);
}

HugeInt HugeInt::operator+(const HugeInt& other){
    int t = 0, sum, i;
    int n = this->digits.size(); 
    int m = other.digits.size();
    if(m > n)
        this->digits.append(m - n, 0);
    n = this->digits.size();
    for (i = 0; i < n;i++){
        if(i < m)
            sum = (this->digits[i] + other.digits[i]) + t;
        else
            sum = this->digits[i] + t;
        t = sum / 10;
        this->digits[i] = (sum % 10);
    }
    if(t)
        this->digits.push_back(t);
    return *this;
}

HugeInt HugeInt::operator*(const HugeInt& other) {
    if(this == nullptr || other.digits.size() == 0) { 
        return 0;
    }
    int n = this->digits.size(), m = other.digits.size();
    std::vector<int> vec(n + m, 0);
    for (int i = 0; i < n;i++) {
        for (int j = 0; j < m;j++){
            vec[i + j] += (this->digits[i] ) * (other.digits[j]);
        }
    }
    n += m;
    this->digits.resize(vec.size());
    for (int sum, i = 0, t = 0; i < n; i++)
    {
        sum = t + vec[i];
        vec[i] = sum % 10;
        t = sum / 10;
        this->digits[i] = vec[i] ;
    }
    for (int i = n - 1; i >= 1 && !vec[i];i--)
            this->digits.pop_back();
    return *this;
}

std::ostream& operator<<(std::ostream& out, const HugeInt& obj) {
    for(int i = obj.digits.size() - 1; i >= 0; --i){
        out << int(obj.digits[i]);
    }
    return out;
}