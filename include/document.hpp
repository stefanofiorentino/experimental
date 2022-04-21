#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

template <typename T> inline void draw(const T &x, std::ostream &out, size_t position);

class object_t {
public:
  template <typename T>
  explicit object_t(T x) : self_(std::make_unique<model_t<T>>(std::move(x))) {}

  object_t(object_t const &x) = delete;
  object_t &operator=(object_t const &x) = delete;

  object_t(object_t &&x) = default;
  object_t &operator=(object_t &&x) = default;

  friend void draw(const object_t &x, std::ostream &out, size_t position) {
    x.self_->draw(out, position);
  }

private:
  struct concept_t {
    ~concept_t() = default;
    virtual void draw(std::ostream &out, size_t position) const = 0;
  };
  template <typename T> struct model_t final : concept_t {

    model_t(T x) : data_(std::move(x)) {}
    void draw(std::ostream &out, size_t position) const override {
      ::draw(data_, out, position);
    }
    T data_;
  };
  std::unique_ptr<concept_t> self_;
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
