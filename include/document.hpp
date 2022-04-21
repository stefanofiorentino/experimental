#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

template <typename T> inline void draw(const T &x, std::ostream &out, size_t position);

class object_t {
public:
  explicit object_t(int x) : self_(std::make_unique<model_t>(std::move(x))) {}

  object_t(object_t const &x) = delete;
  object_t &operator=(object_t const &x) = delete;

  object_t(object_t &&x) = default;
  object_t &operator=(object_t &&x) = default;

  friend void draw(const object_t &x, std::ostream &out, size_t position) {
    x.self_->draw(out, position);
  }

private:
  struct model_t {
    model_t(int x) : data_(std::move(x)) {}
    void draw(std::ostream &out, size_t position) const {
      ::draw(data_, out, position);
    }
    int data_;
  };
  std::unique_ptr<model_t> self_;
};

using collection_t = std::vector<object_t>;

template <typename T>
inline void draw(const T &x, std::ostream &out, size_t position) {
  out << std::string(position, ' ') << x << "\n";
}

template <>
inline void draw(const collection_t &x, std::ostream &out, size_t position) {
  for (const auto &e : x)
    draw(e, out, position);
}
