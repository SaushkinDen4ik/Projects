#include <iostream>
#include <vector>

template <typename T>
class Deque {
 private:
  T** external_pointer_ = nullptr;
  size_t external_size_;
  size_t deque_size_;
  std::pair<size_t, size_t> beg_;
  const static size_t kN = 250;
  const static size_t kThree = 3;

  void base_reserve(T** ptr) {
    try {
      *ptr = reinterpret_cast<T*>(new uint8_t[kN * sizeof(T)]);
    } catch (...) {
      throw;
    }
  }

  void reserve() {
    for (size_t i = 0; i < external_size_; ++i) {
      try {
        base_reserve(external_pointer_ + i);
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          delete[] reinterpret_cast<uint8_t*>(*(external_pointer_ + j));
          throw;
        }
      }
    }
  }

  void external_resize() {
    T** new_pointer = new T*[kThree * external_size_];
    size_t delta_ptr = (beg_.second + deque_size_ - 1) / kN;
    if (deque_size_ == 0) {
      delta_ptr = 0;
    }

    for (size_t i = 0; i < beg_.first; ++i) {
      delete[] reinterpret_cast<uint8_t*>(*(external_pointer_ + i));
    }
    for (size_t i = beg_.first + delta_ptr + 1; i < external_size_; ++i) {
      delete[] reinterpret_cast<uint8_t*>(*(external_pointer_ + i));
    }

    for (size_t i = beg_.first, j = 0; i <= beg_.first + delta_ptr; ++i, ++j) {
      *(new_pointer + external_size_ + j) = *(external_pointer_ + i);
    }

    for (size_t i = 0; i < external_size_; ++i) {
      base_reserve(new_pointer + i);
    }
    for (size_t i = external_size_ + delta_ptr + 1; i < kThree * external_size_;
         ++i) {
      base_reserve(new_pointer + i);
    }
    delete[] external_pointer_;
    external_pointer_ = new_pointer;
    beg_.first = external_size_;
    external_size_ *= kThree;
  }

  T* get_pointer(size_t position) const {
    if (position < 0 || position > deque_size_) {
      throw;
    }
    size_t delta_ptr = (beg_.second + position) / kN;
    size_t remainder = (beg_.second + position) % kN;
    return (*(external_pointer_ + beg_.first + delta_ptr) + remainder);
  }

  void exception_el_reserve_delete(size_t cnt_of_add_elements) {
    for (size_t j = 0; j < cnt_of_add_elements; ++j) {
      (get_pointer(j))->~T();
    }
    for (size_t i = 0; i < external_size_; ++i) {
      delete[] reinterpret_cast<uint8_t*>(*(external_pointer_ + i));
    }
    delete[] external_pointer_;
    throw;
  }

 public:
  Deque()
      : external_pointer_(new T*[1]),
        external_size_(1),
        deque_size_(0),
        beg_({0, 0}) {
    try {
      *external_pointer_ = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kN]);
    } catch (...) {
      delete[] external_pointer_;
      throw;
    }
  }

  Deque(const Deque<T>& deq)
      : external_size_(deq.external_size_),
        deque_size_(deq.deque_size_),
        beg_(deq.beg_) {
    external_pointer_ = new T*[external_size_];
    reserve();
    for (size_t cnt_of_add_el = 0; cnt_of_add_el < deque_size_;
         ++cnt_of_add_el) {
      try {
        new (get_pointer(cnt_of_add_el)) T(deq[cnt_of_add_el]);
      } catch (...) {
        exception_el_reserve_delete(cnt_of_add_el);
      }
    }
  }

  Deque(int size) : deque_size_(size) {
    size_t str_quantity = size / kN + 1;
    external_pointer_ = new T*[kThree * str_quantity];
    external_size_ = kThree * str_quantity;
    beg_ = {str_quantity, 0};
    reserve();
    for (size_t cnt_of_add_el = 0; cnt_of_add_el < deque_size_;
         ++cnt_of_add_el) {
      try {
        new (get_pointer(cnt_of_add_el)) T();
      } catch (...) {
        exception_el_reserve_delete(cnt_of_add_el);
        deque_size_ = 0;
        external_size_ = 0;
      }
    }
  }

  Deque(int size, const T& element) : deque_size_(size) {
    size_t str_quantity = size / kN + 1;
    external_size_ = kThree * str_quantity;
    external_pointer_ = new T*[external_size_];
    beg_ = {str_quantity, 0};
    reserve();
    for (size_t cnt_of_add_el = 0; cnt_of_add_el < deque_size_;
         ++cnt_of_add_el) {
      try {
        new (get_pointer(cnt_of_add_el)) T(element);
      } catch (...) {
        exception_el_reserve_delete(cnt_of_add_el);
        deque_size_ = 0;
        external_size_ = 0;
      }
    }
  }

  Deque<T>& operator=(const Deque<T>& other) {
    this->~Deque();
    external_size_ = other.external_size_;
    deque_size_ = other.deque_size_;
    beg_ = other.beg_;
    external_pointer_ = new T*[external_size_];
    reserve();
    for (size_t cnt_of_add_el = 0; cnt_of_add_el < deque_size_;
         ++cnt_of_add_el) {
      try {
        new (get_pointer(cnt_of_add_el)) T(other[cnt_of_add_el]);
      } catch (...) {
        exception_el_reserve_delete(cnt_of_add_el);
        external_size_ = 0;
        deque_size_ = 0;
      }
    }
    return *this;
  }

  ~Deque() {
    size_t delta_ptr = (beg_.second + deque_size_ - 1) / kN;
    if (deque_size_ == 0) {
      delta_ptr = 0;
    }
    for (size_t i = beg_.first; i <= beg_.first + delta_ptr; ++i) {
      for (size_t j = 0; j < kN; ++j) {
        (*(external_pointer_ + i) + j)->~T();
      }
    }
    for (size_t i = 0; i < external_size_; ++i) {
      delete[] reinterpret_cast<uint8_t*>(*(external_pointer_ + i));
    }
    delete[] external_pointer_;
  }

  size_t size() const { return deque_size_; }

  T& operator[](size_t index) const { return *(get_pointer(index)); }

  T& at(size_t position) const {
    if (position >= deque_size_ || position < 0) {
      throw std::out_of_range("Out of range");
    }
    return (*this)[position];
  }

  void push_back(const T& element) {  // NOLINT
    if (deque_size_ == 0 ||
        (get_pointer(deque_size_ - 1) ==
         *(external_pointer_ + external_size_ - 1) + kN - 1)) {
      external_resize();
    }
    try {
      new (get_pointer(deque_size_)) T(element);
    } catch (...) {
      throw;
    }
    ++deque_size_;
  }

  void pop_back() {  // NOLINT
    if (deque_size_ == 0) {
      throw;
    }
    (get_pointer(deque_size_ - 1))->~T();
    --deque_size_;
  }

  void push_front(const T& element) {  // NOLINT
    if (beg_.first == 0 and beg_.second == 0) {
      external_resize();
    }
    if (beg_.second == 0) {
      beg_ = {beg_.first - 1, kN - 1};
    } else {
      --beg_.second;
    }
    try {
      new (get_pointer(0)) T(element);
    } catch (...) {
      throw;
    }
    ++deque_size_;
  }

  void pop_front() {  // NOLINT
    if (deque_size_ == 0) {
      throw;
    }
    (get_pointer(0))->~T();
    if (beg_.second == kN - 1) {
      beg_ = {beg_.first + 1, 0};
    } else {
      ++beg_.second;
    }
    --deque_size_;
  }

  template <bool is_const>
  class basic_iterator {  // NOLINT
   private:
    T** ext_pointer_;
    size_t external_;
    size_t internal_;

   public:
    using value_type = std::conditional_t<is_const, const T, T>;  // NOLINT
    using reference = value_type&;                                // NOLINT
    using pointer = value_type*;                                  // NOLINT
    using iterator_category = std::random_access_iterator_tag;    // NOLINT

    basic_iterator(T** ext_p, size_t ext, size_t inter)
        : ext_pointer_(ext_p), external_(ext), internal_(inter) {}

    basic_iterator(std::pair<size_t, size_t> beg, T** ext_p, int position)
        : ext_pointer_(ext_p) {
      size_t delta_ptr = (beg.second + position) / kN;
      size_t remainder = (beg.second + position) % kN;
      external_ = delta_ptr + beg.first;
      internal_ = remainder;
    }

    basic_iterator& operator++() {
      if (internal_ == kN - 1) {
        ++external_;
        internal_ = 0;
      } else {
        ++internal_;
      }
      return *this;
    }

    basic_iterator& operator--() {
      if (internal_ == 0) {
        --external_;
        internal_ = kN - 1;
      } else {
        --internal_;
      }
      return *this;
    }

    basic_iterator operator+(int n) const {
      basic_iterator copy = *this;
      if (n < 0) {
        return copy - (-n);
      }
      int first = (internal_ + n) / kN;
      int second = (internal_ + n) % kN;
      copy.external_ += first;
      copy.internal_ = second;
      return copy;
    }

    basic_iterator operator-(int n) const {
      basic_iterator copy = *this;
      if (n < 0) {
        return copy + (-n);
      }
      for (int i = 0; i < n; ++i) {
        --copy;
      }
      return copy;
    }

    bool operator==(const basic_iterator& other) const {
      return (external_ == other.external_) && (internal_ == other.internal_);
    }

    bool operator!=(const basic_iterator& other) const {
      return !(*this == other);
    }

    bool operator<(const basic_iterator& other) const {
      if (external_ == other.external_) {
        return internal_ < other.internal_;
      }
      return external_ < other.external_;
    }

    bool operator>(const basic_iterator& other) const { return other < *this; }

    bool operator<=(const basic_iterator& other) const {
      return (*this == other) || (*this < other);
    }

    bool operator>=(const basic_iterator& other) const {
      return (*this > other) || (*this == other);
    }

    int operator-(const basic_iterator& other) {
      int my_ext = external_;
      int my_int = internal_;
      int other_ext = other.external_;
      int other_int = other.internal_;
      if (*this < other) {
        std::swap(my_ext, other_ext);
        std::swap(my_int, other_int);
      }
      int delta_ext = my_ext - other_ext;
      if (delta_ext == 0) {
        return std::abs(my_int - other_int);
      }
      int difference =
          std::max(delta_ext - 1, 0) * kN + (kN - 1) - other_int + my_int;
      if (*this < other) {
        difference *= -1;
      }
      return difference;
    }

    reference operator*() const {
      return *(*(ext_pointer_ + external_) + internal_);
    }

    pointer operator->() const {
      return *(ext_pointer_ + external_) + internal_;
    }
  };

  using iterator = basic_iterator<false>;       // NOLINT
  using const_iterator = basic_iterator<true>;  // NOLINT

  iterator begin() {
    iterator iter(external_pointer_, beg_.first, beg_.second);
    return iter;
  }

  const_iterator begin() const {
    const_iterator const_iter(external_pointer_, beg_.first, beg_.second);
    return const_iter;
  }

  iterator end() {
    iterator iter(beg_, external_pointer_, deque_size_);
    return iter;
  }

  const_iterator end() const {
    const_iterator const_iter(beg_, external_pointer_, deque_size_);
    return const_iter;
  }

  const_iterator cbegin() const {
    const_iterator const_iter(external_pointer_, beg_.first, beg_.second);
    return const_iter;
  }

  const_iterator cend() const {
    const_iterator const_iter(beg_, external_pointer_, deque_size_);
    return const_iter;
  }

  using reverse_iterator = std::reverse_iterator<iterator>;  // NOLINT
  using const_reverse_iterator =                             // NOLINT
      std::reverse_iterator<const_iterator>;

  void insert(iterator add, const T& value) {
    try {
      push_back((*this)[deque_size_ - 1]);
    } catch (...) {
      throw;
    }
    for (iterator it = end() - 1; it != add; --it) {
      *(it) = *(it - 1);
    }
    *(add) = value;
  }

  void erase(iterator remove) {
    if (deque_size_ == 0) {
      throw;
    }
    for (iterator it = remove + 1; it != end(); ++it) {
      *(it - 1) = *(it);
    }
    pop_back();
  }

  reverse_iterator rbegin() const { return end(); }

  reverse_iterator rend() const { return begin(); }

  const_reverse_iterator crbegin() const { return cend(); }

  const_reverse_iterator crend() const { return crbegin(); }
};
