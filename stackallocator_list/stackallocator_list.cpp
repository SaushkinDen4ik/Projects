#include <iostream>
#include <cstddef>

template <size_t N>
struct alignas(std::max_align_t) StackStorage {
  alignas(std::max_align_t) uint8_t pool_[N];
  size_t start_ = 0;

  StackStorage() = default;

  uint8_t* Get(size_t bytes, size_t align) {
    if (start_ % align != 0) {
      start_ += align - (start_ % align);
    }
    uint8_t* return_pointer = pool_ + start_;
    start_ += bytes;
    return return_pointer;
  }
};

template <typename T, size_t N>
class StackAllocator {
 private:
  StackStorage<N>* pointer_on_pool_;

 public:
  using value_type = T;
  using pointer = T*;

  StackAllocator() = default;

  StackAllocator(const StackStorage<N>& storage)
    : pointer_on_pool_(const_cast<StackStorage<N>*>(&storage)) {}

  StackAllocator(const StackAllocator& other)
    : pointer_on_pool_(other.get_pointer_on_storage()) {}

  template <typename U>
  StackAllocator(const StackAllocator<U, N>& other)
    : pointer_on_pool_(other.get_pointer_on_storage()) {}

  StackAllocator operator=(const StackAllocator& other) {
    pointer_on_pool_ = other.get_pointer_on_storage();
    return *this;
  }

  template <typename U>
  StackAllocator operator=(const StackAllocator<U, N>& other) {
    pointer_on_pool_ = other.get_pointer_on_storage();
    return *this;
  }

  ~StackAllocator() {}

  StackStorage<N>* get_pointer_on_storage() const {
    return pointer_on_pool_;
  }

  T* allocate(size_t n) const {
    size_t bytes_to_allocate = n * sizeof(T);
    return reinterpret_cast<T*>(pointer_on_pool_->Get(bytes_to_allocate,
                                                      alignof(T)));
  }

  void deallocate(T* ptr, size_t n) const {

  }

  template <typename U>
  struct rebind {
    using other = StackAllocator<U, N>;
  };

  template <typename U, size_t S>
  friend bool operator==(const StackAllocator&, const StackAllocator<U, S>&);
};

template <typename T, typename U, size_t N, size_t S>
bool operator==(const StackAllocator<T, N>& first_allocator,
                const StackAllocator<U, S>& second_allocator) {
  return first_allocator.get_pointer_on_storage() == second_allocator.get_pointer_on_storage();
}

template <typename T, typename U, size_t N, size_t S>
bool operator!=(const StackAllocator<T, N>& first_allocator,
                const StackAllocator<U, S>& second_allocator) {
  return !(first_allocator == second_allocator);
}

template <typename T, typename Alloc = std::allocator<T>>
class List {
 private:
  struct Node;
  struct BaseNode {
    Node* prev = static_cast<Node*>(this);
    Node* next = static_cast<Node*>(this);

    BaseNode() = default;
    BaseNode(const BaseNode& other)
      : prev(other.prev), next(other.next) {}
  };

  struct Node: BaseNode {
    T value;

    Node() = default;
    Node(const Node& other): value(other.value) {}
    Node(const T& element): value(element) {}
  };

  using InnerAlloc = typename Alloc::template rebind<Node>::other;
  InnerAlloc alloc_;
  BaseNode* fake_node_;
  size_t size_;

  void LinkedToNodes(BaseNode* first, BaseNode* second) {
    first->next = static_cast<Node*>(second);
    second->prev = static_cast<Node*>(first);
  }

  Node* CreateNode(const T& value) {
    Node* add_node = std::allocator_traits<InnerAlloc>::allocate(alloc_, 1);
    try {
      std::allocator_traits<InnerAlloc>::construct(alloc_, add_node, value);
    } catch (...) {
      std::allocator_traits<InnerAlloc>::deallocate(alloc_, add_node, 1);
      throw;
    }
    return add_node;
  }

  Node* CreateNode() {
    Node* add_node = std::allocator_traits<InnerAlloc>::allocate(alloc_, 1);
    try {
      std::allocator_traits<InnerAlloc>::construct(alloc_, add_node);
    } catch (...) {
      std::allocator_traits<InnerAlloc>::deallocate(alloc_, add_node, 1);
      throw;
    }
    return add_node;
  }

  void DeallocateNode(Node* remove_node) {
    std::allocator_traits<InnerAlloc>::destroy(alloc_, remove_node);
    std::allocator_traits<InnerAlloc>::deallocate(alloc_, remove_node, 1);
  }

 public:
  List() : alloc_(InnerAlloc()),
           fake_node_(std::allocator_traits<InnerAlloc>::allocate(alloc_, 1)),
           size_(0) {}

  List(size_t sz) : alloc_(InnerAlloc()),
                    fake_node_(std::allocator_traits<InnerAlloc>::allocate(alloc_, 1)),
                    size_(0) {
    for (size_t i = 0; i < sz; ++i) {
      try {
        push_back();
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          pop_back();
        }
        throw;
      }
    }
  }

  List(size_t sz, const T& val) : alloc_(InnerAlloc()),
                                  fake_node_(std::allocator_traits<InnerAlloc>::allocate(alloc_, 1)),
                                  size_(0) {
    for (size_t i = 0; i < sz; ++i) {
      try {
        push_back(val);
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          pop_back();
        }
        throw;
      }
    }
  }

  List(const Alloc& allocator)
    : alloc_(std::allocator_traits<InnerAlloc>::select_on_container_copy_construction(allocator)),
      fake_node_(std::allocator_traits<InnerAlloc>::allocate(alloc_, 1)),
      size_(0) {}

  List(size_t sz, const Alloc& allocator)
    : alloc_(std::allocator_traits<InnerAlloc>::select_on_container_copy_construction(allocator)),
      fake_node_(std::allocator_traits<InnerAlloc>::allocate(alloc_, 1)),
      size_(0) {
    for (size_t i = 0; i < sz; ++i) {
      try {
        push_back();
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          pop_back();
        }
        throw;
      }
    }
  }

  List(size_t sz, const T& val, const Alloc& allocator)
    : alloc_(std::allocator_traits<InnerAlloc>::select_on_container_copy_construction(allocator)),
      fake_node_(std::allocator_traits<InnerAlloc>::allocate(alloc_, 1)),
      size_(0) {
    for (size_t i = 0; i < sz; ++i) {
      try {
        push_back(val);
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          pop_back();
        }
        throw;
      }
    }
  }

  List(const List& other)
    : alloc_(std::allocator_traits<InnerAlloc>::select_on_container_copy_construction(other.alloc_)),
      fake_node_(std::allocator_traits<InnerAlloc>::allocate(alloc_, 1)),
      size_(0) {
    auto iter = other.begin();
    for (size_t i = 0; i < other.size_; ++i) {
      try {
        push_back(*iter);
        ++iter;
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          pop_back();
        }
        throw;
      }
    }
  }

  List& operator=(const List& other) {
    size_t start_size = size_;
    for (size_t i = 0; i < start_size; ++i) {
      pop_back();
    }
    if (std::allocator_traits<Alloc>::propagate_on_container_copy_assignment::value) {
      alloc_ = other.alloc_;
    } else {
      alloc_ = std::allocator_traits<Alloc>::select_on_container_copy_construction(other.alloc_);
    }
    fake_node_ = std::allocator_traits<InnerAlloc>::allocate(alloc_, 1);
    size_ = 0;
    auto iter = other.begin();
    for (size_t i = 0; i < other.size_; ++i) {
      try {
        push_back(*iter);
        ++iter;
      } catch (...) {

      }
    }
    return *this;
  }

  ~List() {
    if (size_ != 0) {
      while (size_ != 0) {
        pop_back();
      }
    }
  }

  template <bool is_const>
  struct basic_bidirectional_iterator {
   public:
    BaseNode* current_;

    typedef std::conditional_t<is_const, const T, T> value_type;
    typedef std::conditional_t<is_const, const T*, T*> pointer;
    typedef std::conditional_t<is_const, const T&, T&> reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef int difference_type;

    basic_bidirectional_iterator() = default;

    basic_bidirectional_iterator(const basic_bidirectional_iterator<false>& other) {
      current_ = other.current_;
    }

    basic_bidirectional_iterator(BaseNode* cur) : current_(cur) {}

    basic_bidirectional_iterator& operator=(const basic_bidirectional_iterator<false>& other) {
      current_ = other.current_;
      return *this;
    }

    reference operator*() const {
      return (static_cast<Node*>(current_))->value;
    }

    basic_bidirectional_iterator& operator--() {
      current_ = current_->prev;
      return *this;
    }

    basic_bidirectional_iterator& operator++() {
      current_ = current_->next;
      return *this;
    }

    basic_bidirectional_iterator operator--(int) {
      basic_bidirectional_iterator copy(*this);
      current_ = current_->prev;
      return copy;
    }

    basic_bidirectional_iterator operator++(int) {
      basic_bidirectional_iterator copy(*this);
      current_ = current_->next;
      return copy;
    }

    basic_bidirectional_iterator operator+(int n) const {
      basic_bidirectional_iterator copy(*this);
      if (n >= 0) {
        for (int i = 1; i <= n; ++i) {
          ++copy;
        }
      } else {
        n *= -1;
        for (int i = 1; i <= n; ++i) {
          --copy;
        }
      }
      return copy;
    }

    basic_bidirectional_iterator operator-(int n) const {
      basic_bidirectional_iterator copy(*this);
      if (n >= 0) {
        for (int i = 1; i <= n; ++i) {
          --copy;
        }
      } else {
        n *= -1;
        for (int i = 1; i <= n; ++i) {
          ++copy;
        }
      }
      return copy;
    }

    bool operator==(const basic_bidirectional_iterator& other) const {
      return (other.current_ == current_);
    }

    bool operator!=(const basic_bidirectional_iterator& other) const {
      return !(*this == other);
    }

    Node* operator->() const {
      return static_cast<Node*>(current_);
    }
  };

  using const_iterator = basic_bidirectional_iterator<true>;
  using iterator = basic_bidirectional_iterator<false>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = std::reverse_iterator<iterator>;

  InnerAlloc get_allocator() const {
    return alloc_;
  }

  size_t size() const {
    return size_;
  }

  iterator end() const {
    return iterator(fake_node_);
  }

  iterator begin() const {
    if (size_ == 0) { return end(); }
    return ++end();
  }

  const_iterator cbegin() const {
    if (size_ == 0) { return cend(); }
    return const_iterator(++cend());
  }

  const_iterator cend() const {
    return const_iterator(fake_node_);
  }

  reverse_iterator rbegin() const {
    return reverse_iterator(fake_node_);
  }

  reverse_iterator rend() const {
    return reverse_iterator(fake_node_->next);
  }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(fake_node_);
  }

  const_reverse_iterator crend() const {
    return reverse_iterator(fake_node_->next);
  }

  void push_back(const T& value) {
    Node* add_node = CreateNode(value);
    BaseNode* end_node = fake_node_;
    if (size_ != 0) {
      end_node = fake_node_->prev;
    }
    LinkedToNodes(end_node, add_node);
    LinkedToNodes(add_node, fake_node_);
    ++size_;
  }

  void push_back() {
    Node* add_node = CreateNode();
    BaseNode* end_node = fake_node_;
    if (size_ != 0) {
      end_node = fake_node_->prev;
    }
    LinkedToNodes(end_node, add_node);
    LinkedToNodes(add_node, fake_node_);
    ++size_;
  }

  void push_front(const T& value) {
    Node* add_node = CreateNode(value);
    BaseNode* begin_node = fake_node_;
    if (size_ != 0) {
      begin_node = fake_node_->next;
    }
    LinkedToNodes(fake_node_, add_node);
    LinkedToNodes(add_node, begin_node);
    ++size_;
  }

  void pop_back() {
    erase(--cend());
  }

  void pop_front() {
    erase(cbegin());
  }

  void insert(const_iterator add_iter, const T& value) {
    Node* add_node = CreateNode(value);
    if (add_iter == end()) {
      push_back(value);
    } else {
      BaseNode *iter_node = add_iter.operator->();
      BaseNode *next_iter_node = iter_node->prev;
      LinkedToNodes(next_iter_node, add_node);
      LinkedToNodes(add_node, iter_node);
      ++size_;
    }
  }

  void erase(const_iterator erase_iter) {
    Node* remove_node = erase_iter.operator->();
    LinkedToNodes(remove_node->prev, remove_node->next);
    DeallocateNode(remove_node);
    --size_;
  }
};
