#include <string>
#include <vector>
#include <algorithm>

class BigInteger;

bool operator==(const BigInteger& a, const BigInteger& b);

bool operator<(const BigInteger& a, const BigInteger& b);

bool operator!=(const BigInteger& a, const BigInteger& b);

bool operator>(const BigInteger& a, const BigInteger& b);

bool operator<=(const BigInteger& a, const BigInteger& b);

bool operator>=(const BigInteger& a, const BigInteger& b);

BigInteger operator+(const BigInteger& a, const BigInteger& b);

BigInteger operator-(const BigInteger& a, const BigInteger& b);

BigInteger operator*(const BigInteger& a, const BigInteger& b);

BigInteger operator/(const BigInteger& a, const BigInteger& b);

BigInteger operator%(const BigInteger& a, const BigInteger& b);

class BigInteger {
 private:
  static const int calc_system = 1e9;
  const int calc = 9;
  std::vector<long long> digits;
  bool is_positive;
  static int stringIntCast(const std::string& s) {
    int res = 0, ten = 1;
    for (int i = s.size() - 1; i >= 0; --i) {
      res += (s[i] - '0') * ten;
      ten *= 10;
    }
    return res;
  }
  static int how_many_digits(const int number) {
    int power_of_ten = 1, scr_of_digits = 0;
    while (number / power_of_ten != 0) {
      power_of_ten *= 10;
      ++scr_of_digits;
    }
    return  scr_of_digits;
  }
  void delete_nulls() {
    if (digits.size() == 0) {
      return;
    }
    size_t i = digits.size() - 1;
    while (digits[i] == 0) {
      i--;
      digits.pop_back();
      if (i + 1 == 0) {
        break;
      }
    }
  }
 public:
  BigInteger() = default;
  BigInteger(const std::string& s) {
    int check = 0;
    if (s[0] == '-') {
      is_positive = false;
      check = 1;
    }
    else {
      is_positive = true;
    }
    int size = s.size();
    int i = std::max(check, size - calc);
    do {
      std::string temp_s = s.substr(i, std::min(calc, size - check));
      int temp = stringIntCast(temp_s);
      digits.push_back(temp);
      i -= calc;
      if (i + calc > check && i < check) {
        std::string temp2_s = s.substr(check, i + calc - check);
        int temp2 = stringIntCast(temp2_s);
        digits.push_back(temp2);
      }
    } while(i >= check);
  }
  BigInteger(const int a) {
    int copy = a;
    if (copy < 0) {
      is_positive = false;
      copy *= -1;
    } else {
      is_positive = true;
    }
    while (copy != 0) {
      digits.push_back(copy % calc_system);
      copy /= calc_system;
    }
  }

  BigInteger& operator=(const BigInteger& a) {
    digits = a.digits;
    is_positive = a.is_positive;
    return *this;
  }

  explicit operator bool() const {
    return (is_positive && (*this != 0));
  }

  BigInteger& operator+=(const BigInteger& a);
  BigInteger& operator-=(const BigInteger& a);
  BigInteger& operator*=(const BigInteger& a);
  BigInteger& operator/=(const BigInteger& a);
  BigInteger& operator%=(const BigInteger& a);

  std::string toString() const {
    std::string s;
    if (digits.size() == 0) {
      s = "0";
    } else {
      if (!is_positive) {
        s.push_back('-');
      }
      for (size_t i = digits.size() - 1; i + 1 != 0; --i) {
        if (digits[i] != 0) {
          std::string temp_s = std::to_string(digits[i]);
          s += temp_s;
        }
        if (i != 0) {
          for (int k = 1; k <= calc - how_many_digits(digits[i - 1]); ++k) {
            s += "0";
          }
        }
      }
    }
    return s;
  }

  friend std::istream& operator>>(std::istream&, BigInteger&);
  friend bool operator==(const BigInteger&, const BigInteger&);
  friend bool operator<(const BigInteger&, const BigInteger&);
  BigInteger& operator++() {
    *this += 1;
    return *this;
  }
  BigInteger operator++(int) {
    BigInteger copy = *this;
    *this += 1;
    return copy;
  }
  BigInteger operator--() {
    *this -= 1;
    return *this;
  }
  BigInteger operator--(int) {
    BigInteger copy = *this;
    *this -= 1;
    return copy;
  }
  BigInteger operator-() const {
    BigInteger copy = *this;
    copy.is_positive = !is_positive;
    return copy;
  }

  BigInteger sign() const {
    if (is_positive) {
      return 1;
    }
    return -1;
  }
};

std::istream& operator>>(std::istream& in, BigInteger& a) {
  std::string temp_s;
  in >> temp_s;
  a = BigInteger(temp_s);
  return in;
}
std::ostream& operator<<(std::ostream& out, BigInteger a) {
  out << a.toString();
  return out;
}

bool operator==(const BigInteger& a, const BigInteger& b) {
  if (a.digits.size() == 0 && b.digits.size() == 0) {
    return true;
  }
  if (a.is_positive != b.is_positive) {
    return false;
  } else {
    size_t size_a = a.digits.size();
    size_t size_b = b.digits.size();
    if (size_a != size_b) {
      return false;
    } else {
      for (size_t i = 0; i < size_a; ++i) {
        if (a.digits[i] != b.digits[i]) {
          return false;
        }
      }
      return true;
    }
  }
}
bool operator<(const BigInteger& a, const BigInteger& b) {
  if (a.is_positive == b.is_positive) {
    size_t size_a = a.digits.size(), size_b = b.digits.size();
    if (size_a == size_b) {
      for (size_t i = size_a - 1; i + 1 != 0; --i) {
        if (a.digits[i] > b.digits[i]) {
          return false;
        }
        if (a.digits[i] < b.digits[i]) {
          return true;
        }
      }
      return false;
    } else {
      return !(size_a > size_b);
    }
  } else {
    return !a.is_positive;
  }
}
bool operator!=(const BigInteger& a, const BigInteger& b) {
  return !(a==b);
}
bool operator>(const BigInteger& a, const BigInteger& b) {
  return b < a;
}
bool operator<=(const BigInteger& a, const BigInteger& b) {
  return a < b || a == b;
}
bool operator>=(const BigInteger& a, const BigInteger& b) {
  return b <= a;
}

BigInteger& BigInteger::operator-=(const BigInteger &a) {
  if (is_positive && a.is_positive) {
    if (*this >= a) {
      for (size_t i = 0; i < a.digits.size(); ++i) {
        digits[i] = digits[i] - a.digits[i];
        if (digits[i] < 0) {
          --digits[i + 1];
          digits[i] += calc_system;
        }
      }
      size_t last_num = digits.size() - 1;
      if (digits.size() == 0) {
        last_num = 0;
      }
      for (size_t i = 0; i < last_num; ++i) {
        if (digits[i] < 0) {
          --digits[i + 1];
          digits[i] += calc_system;
        }
      }
      delete_nulls();
    } else {
      BigInteger copy = a;
      copy -= *this;
      *this = copy;
      is_positive = false;
    }
  } else if (is_positive && !a.is_positive) {
    *this += -a;
  } else if (!is_positive && !a.is_positive) {
    BigInteger copy = -a;
    is_positive = true;
    copy -= *this;
    *this = copy;
  } else if (!is_positive && a.is_positive) {
    is_positive = true;
    *this += a;
    is_positive = false;
  }
  return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger &a) {
  if (is_positive && a.is_positive) {
    size_t size = digits.size(), size_a = a.digits.size();
    size_t delta_size = size_a - size;
    if (size > size_a) {
      delta_size = 0;
    }
    for (size_t i = 1; i <= delta_size + 1; ++i) {
      digits.push_back(0);
    }
    for (size_t i = 0; i < size_a; ++i) {
      digits[i+1] += (digits[i] + a.digits[i]) / calc_system;
      digits[i] = (digits[i] + a.digits[i]) % calc_system;
    }
    for (size_t i = size_a; i < digits.size() - 1; ++i) {
      digits[i+1] += digits[i] / calc_system;
      digits[i] = digits[i] % calc_system;
    }
    delete_nulls();
  }
  if (!is_positive && !a.is_positive) {
    is_positive = true;
    *this += -a;
    is_positive = false;
  }
  if (!is_positive && a.is_positive) {
    BigInteger copy = a;
    is_positive = true;
    copy -= *this;
    *this = copy;
  }
  if (is_positive && !a.is_positive) {
    *this -= -a;
  }
  return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& a) {
  if (a == 0) {
    is_positive = true;
    return (*this = 0);
  }
  size_t size_a = a.digits.size(), my_start_size = digits.size();
  for (size_t i = my_start_size; i < my_start_size + size_a - 1; ++i) {
    digits.push_back(0);
  }
  for (size_t i = digits.size() - 1; i + 1 != 0; --i) {
    long long start_val = digits[i];
    for (size_t j_my = std::min(i, my_start_size); j_my + 1 != 0; --j_my) {
      size_t j_a = i - j_my;
      if (j_a == size_a) {
        break;
      }
      digits[i] += digits[j_my] * a.digits[j_a];
      size_t k = i;
      while (digits[k] >= calc_system) {
        if (k == digits.size() - 1) {
          digits.push_back(0);
        }
        int carry = digits[k] / calc_system;
        digits[k] %= calc_system;
        ++k;
        digits[k] += carry;
      }
    }
    digits[i] -= start_val;
  }
  size_t last_num = digits.size() - 1;
  if (digits.size() == 0) {
    last_num = 0;
  }
  for (size_t i = 0; i < last_num; ++i) {
    if (digits[i] >= 0) {
      int carry = digits[i] / calc_system;
      digits[i] %= calc_system;
      digits[i + 1] += carry;
    } else {
      digits[i] += calc_system;
      --digits[i + 1];
    }
  }
  delete_nulls();
  is_positive = (is_positive == a.is_positive);
  return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &div_number) {
  BigInteger copy_div = div_number;
  bool start_sign = is_positive;
  is_positive = true, copy_div.is_positive = true;
  BigInteger current_div(0);
  BigInteger quotient(0);
  for (size_t i = digits.size(); i + 1 != 0; --i) {
    int left_pos = 0;
    int right_pos = calc_system;
    while (left_pos + 1 != right_pos) {
      int middle = (left_pos + right_pos) >> 1;
      if (middle * copy_div <= current_div) {
        left_pos= middle;
      } else {
        right_pos = middle;
      }
    }
    quotient.digits.push_back(left_pos);
    current_div -= (left_pos * copy_div);
    if (i > 0) {
      current_div = digits[i - 1] + calc_system * current_div;
    }
  }
  size_t last_num = quotient.digits.size() - 1;
  for (size_t j = 0; 2 * j <= last_num; ++j) {
    std::swap(quotient.digits[j], quotient.digits[last_num - j]);
  }
  quotient.delete_nulls();
  *this = quotient;
  is_positive = (start_sign == div_number.is_positive);
  return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &div_number) {
  BigInteger copy = *this;
  copy /= div_number;
  *this = (*this - copy * div_number);
  return *this;
}

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
  BigInteger copy = a;
  copy += b;
  return copy;
}
BigInteger operator-(const BigInteger& a, const BigInteger& b) {
  BigInteger copy = a;
  copy -= b;
  return copy;
}
BigInteger operator*(const BigInteger& a, const BigInteger& b) {
  BigInteger copy = a;
  copy *= b;
  return copy;
}
BigInteger operator/(const BigInteger& a, const BigInteger& b) {
  BigInteger copy = a;
  copy /= b;
  return copy;
}
BigInteger operator%(const BigInteger& a, const BigInteger& b) {
  BigInteger copy = a;
  copy %= b;
  return copy;
}

class Rational;

bool operator< (const Rational& a, const Rational& b);

class Rational {
 private:
  BigInteger numerator = 0;
  BigInteger denominator = 1;

  static BigInteger gcd(const BigInteger a, const BigInteger b) {
    if (a == b || b == 0) {
      return  a;
    }
    if (a == 0) {
      return b;
    }
    if (a % 2 == 0 && b % 2 == 0) {
      return 2 * gcd(a/2, b/2);
    }
    if (a % 2 == 0) {
      return gcd(a/2, b);
    }
    if (b % 2 == 0) {
      return gcd(a, b/2);
    }
    if (a < b) {
      return gcd((b - a)/2, a);
    }
    return gcd((a - b)/2, b);
  }
  void reduce() {
    if (numerator.sign() != denominator.sign()) {
      numerator *= denominator.sign();
      denominator *= denominator.sign();
    } else {
      denominator *= denominator.sign();
      numerator *= numerator.sign();
    }
    if (numerator == 0) {
      numerator *= numerator.sign();
    }
    BigInteger div = gcd(numerator * numerator.sign(), denominator);
    numerator /= div;
    denominator /= div;
  }
 public:
  BigInteger getNumerator() const {
    return numerator;
  }
  BigInteger getDenominator() const {
    return denominator;
  }
  Rational() = default;
  Rational(const int a): numerator(a) {}
  Rational(const BigInteger& a): numerator(a) {}
  Rational(const Rational& a) = default;

  Rational& operator+=(const Rational& a) {
    numerator *= a.denominator;
    numerator += a.numerator * denominator;
    denominator *= a.denominator;
    return *this;
  }
  Rational& operator-=(const Rational& a) {
    numerator *= a.denominator;
    numerator -= a.numerator * denominator;
    denominator *= a.denominator;
    return *this;
  }
  Rational& operator*=(const Rational& a) {
    numerator *= a.numerator;
    denominator *= a.denominator;
    return *this;
  }
  Rational& operator/=(const Rational& a) {
    numerator *= a.denominator;
    denominator *= a.numerator;
    return *this;
  }
  Rational operator-() const {
    Rational copy(*this);
    copy *= -1;
    return copy;
  }
  std::string toString() const {
    Rational copy = *this;
    copy.reduce();
    std::string ans_str = copy.numerator.toString();
    if (copy.denominator != 1) {
      ans_str += "/";
      ans_str += copy.denominator.toString();
    }
    return ans_str;
  }
  std::string asDecimal(size_t precision) const {
    std::string ans_str;
    if (*this < 0) {
      ans_str += "-";
    }
    BigInteger first(numerator * numerator.sign());
    first /= denominator;
    ans_str += first.toString() + ".";
    BigInteger div = numerator * numerator.sign() % denominator;
    for (size_t i = 1; i <= precision; ++i) {
      ans_str += (div * 10 / denominator).toString();
      div = div * 10 % denominator;
    }
    return ans_str;
  }

  explicit operator double() const {
    return atof(asDecimal(50).c_str());
  }
  friend bool operator==(const Rational&, const Rational&);
  friend bool operator<(const Rational& a, const Rational& b);
};

bool operator==(const Rational& a, const Rational& b) {
  Rational a_copy = a, b_copy = b;
  a_copy.reduce();
  b_copy.reduce();
  if (a_copy.getNumerator() == 0 && b_copy.getNumerator() == 0) {
    return true;
  }
  return (a_copy.getNumerator() == b_copy.getNumerator() && a_copy.getDenominator() == b_copy.getDenominator());
}
bool operator!=(const Rational& a, const Rational& b) {
  return !(a == b);
}
bool operator<(const Rational& a, const Rational& b) {
  Rational a_copy = a, b_copy = b;
  a_copy.reduce();
  b_copy.reduce();
  if (a_copy.getNumerator().sign() != b_copy.getNumerator().sign()) {
    return (b_copy.getNumerator().sign() == 1);
  }
  return (a_copy.getNumerator() * b_copy.getDenominator() < b_copy.getNumerator() * a_copy.getDenominator());
}
bool operator>(const Rational& a, const Rational& b) {
  return b < a;
}
bool operator<=(const Rational& a, const Rational& b) {
  return (a < b || a == b);
}
bool operator>=(const Rational& a, const Rational& b) {
  return (a > b || a == b);
}

Rational operator+(const Rational& a, const Rational& b) {
  Rational copy(a);
  copy += b;
  return copy;
}
Rational operator-(const Rational& a, const Rational& b) {
  Rational copy(a);
  copy -= b;
  return copy;
}
Rational operator*(const Rational& a, const Rational& b) {
  Rational copy(a);
  copy *= b;
  return copy;
}
Rational operator/(const Rational& a, const Rational& b) {
  Rational copy(a);
  copy /= b;
  return copy;
}

std::ostream& operator<<(std::ostream& out, const Rational& a) {
  std::string temp_s = a.toString();
  out << temp_s;
  return out;
}
