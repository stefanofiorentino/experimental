#pragma once

#include <iostream>
#include <vector>
#include <memory>

class object_t;
using document_t = std::vector<object_t>;

void draw(const document_t& x, std::ostream& out, size_t position);

template <typename T>
void draw(const T& x, std::ostream& out, size_t position)
{
    out << std::string(position, ' ') << x << "\n";
}

class object_t {
public: 
    template <typename T>
    object_t(T x) : self_(std::make_shared<model_t<T>>(std::move(x))) {}
    
    friend void draw(const object_t&x, std::ostream& out, size_t position)
    { x.self_->draw(out, position); }
private: 
    struct concept_t
    {
        virtual ~concept_t() = default;
        virtual void draw(std::ostream& out, size_t position) const = 0;
    };
    template <typename T>
    struct model_t final : concept_t
    {
        model_t(T x) : data_(std::move(x)){}
        void draw(std::ostream& out, size_t position) const override
        {
            // if constexpr (has_bool_reset_v<T>)
            {
                ::draw(data_, out, position);
            }
        }
        T data_;
    };
    std::shared_ptr<const concept_t> self_;
};

void draw(const document_t& x, std::ostream& out, size_t position)
{
    out << std::string(position, ' ') << "<document>\n" ;
    for (const auto& e: x) draw(e, out, position + 2);
    out << std::string(position, ' ') << "</document>\n" ;
}
