#ifndef STACK_N_HXX
#define STACK_N_HXX

#include <algorithm>
#include <cassert>
#include <exception>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>

namespace rng = std::ranges;

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    concept is_class = std::is_class<typename std::decay<T>::type>::value;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class> class stack;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

// START NODE
//
template <class T> class node final
{

public:
    friend class stack<T>;

public:
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;

public:
    using node_ptr_t = typename std::add_pointer<node<T>>::type;

public:
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        node() noexcept
            requires(std::default_initializable<T>)
            : m_data{T{}}
            , m_next{nullptr}
        {}
    //  -------------------------------------------------------------------------
        explicit node(T const &p_value, node_ptr_t p_next) noexcept
            requires(std::copy_constructible<T>)
            : m_data{p_value}
            , m_next{p_next}
        {}
    //  -------------------------------------------------------------------------
        explicit node(T &&p_value, node_ptr_t p_next) noexcept
            requires(std::move_constructible<T>)
            : m_data{std::move(p_value)}
            , m_next{p_next}
        {}
    //  -------------------------------------------------------------------------
        template <class... ARGS>
        node(ARGS &&...p_args) noexcept
            requires(is_class<T> && std::constructible_from<T, ARGS...>)
            : m_data{std::forward<ARGS>(p_args)...}
            , m_next{nullptr}
        {}
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

public:
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto data() noexcept -> decltype(auto) { return m_data; }
    //  --------------------------------------------------------------------------
        auto data() const noexcept -> decltype(auto) { return m_data; }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto next() noexcept -> decltype(auto) { return m_next; }
    //  --------------------------------------------------------------------------
        auto next() const noexcept -> decltype(auto) { return m_next; }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

private:
    value_type m_data;
    node_ptr_t m_next;

};

//  //

//* END NODE *//

template <class T> class stack final
{

public:
    using node_type = node<T>;
    using node_t    = typename node_type::node_ptr_t;

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
public:
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;
    using size_type       = std::size_t;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

public:
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        stack() noexcept;
    //  -------------------------------------------------------------------------
        template <std::input_iterator I, std::sentinel_for<I> S>
        stack(I, S)
            requires(std::convertible_to<std::iter_value_t<I>, T>);
    //  -------------------------------------------------------------------------
        template <rng::input_range R>
        stack(R &&)
            requires(std::convertible_to<rng::range_value_t<R>, T>);
    //  -------------------------------------------------------------------------
        stack(stack const &);
    //  -------------------------------------------------------------------------
        stack(stack &&) noexcept;
    //  -------------------------------------------------------------------------
        stack &operator=(stack);
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        template <class U>
        void push_front(U &&)
            requires(std::convertible_to<U, T>);
    //  -------------------------------------------------------------------------
        template <class U>
        void push(U &&)
            requires(std::convertible_to<U, T>);
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        template <class... ARGS>
        void emplace_front(ARGS &&...)
            requires(is_class<T> && std::constructible_from<T, ARGS...>);
    //  -------------------------------------------------------------------------
        template <class... ARGS>
        void emplace(ARGS &&...)
            requires(is_class<T> && std::constructible_from<T, ARGS...>);
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        void pop_front();
    //  -------------------------------------------------------------------------
        void pop();
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        [[nodiscard]] auto peep() const noexcept -> std::optional<node_type>;
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        [[nodiscard]] auto empty() const noexcept -> bool;
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        [[nodiscard]] auto size() const noexcept -> size_type;
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        ~stack();
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        friend void swap(stack &p_lhs, stack &p_rhs) noexcept
        {
            using std::swap;

            swap(p_lhs.m_head, p_rhs.m_head);
            swap(p_lhs.m_size, p_rhs.m_size);
        }
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

public:
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        void debug() const noexcept
        {
            node_t v_node = m_head;

            while (v_node)
            {
                std::cout << v_node->m_data << ' ';
                v_node = v_node->m_next;
            }

            std::cout << std::endl;
        }
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
private:
    node_t m_head;
    size_type m_size;
};

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <std::input_iterator I, std::sentinel_for<I> S>
    stack<T>::stack(I p_begin, S p_end)
        requires(std::convertible_to<std::iter_value_t<I>, T>)
        : stack()
    {
        using std::placeholders::_1;

        rng::for_each(p_begin, p_end,
                    std::bind(&stack::template push_front<std::iter_reference_t<I>>,
                                this, _1));
    }
//  -------------------------------------------------------------------------
    template <class T>
    template <std::ranges::input_range R>
    stack<T>::stack(R &&p_container)
        requires(std::convertible_to<rng::range_value_t<R>, T>)
        : stack(rng::begin(p_container), rng::end(p_container))
    {}
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <class U>
    void stack<T>::push_front(U &&p_value)
        requires(std::convertible_to<U, T>)
    {
        node_t v_node = nullptr;

        try {
            v_node = new node_type(std::forward<U>(p_value), m_head);
        } catch (...) {
            v_node = (delete v_node, nullptr);

            throw;
        }

        m_head = v_node;

        m_size = -(~m_size);
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <class U>
    void stack<T>::push(U &&p_value)
        requires(std::convertible_to<U, T>)
    {
        push_front(std::forward<U>(p_value));
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <class... ARGS>
    void stack<T>::emplace_front(ARGS &&...p_args)
        requires(is_class<T> && std::constructible_from<T, ARGS...>)
    {
        node_type *v_node = nullptr;

        try {
            v_node = new node_type(std::forward<ARGS>(p_args)...);
        } catch (...) {
            v_node = (delete v_node, nullptr);

            throw;
        }

        v_node->m_next = m_head;
        m_head         = v_node;

        m_size = -(~m_size);
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <class... ARGS>
    void stack<T>::emplace(ARGS &&...p_args)
        requires(is_class<T> && std::constructible_from<T, ARGS...>)
    {
        emplace_front(std::forward<ARGS>(p_args)...);
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    extern template class stack<int>;
    extern template class stack<char>;
    extern template class stack<long>;
    extern template class stack<short>;
    extern template class stack<unsigned int>;
    extern template class stack<unsigned char>;
    extern template class stack<unsigned long>;
    extern template class stack<unsigned short>;
    extern template class stack<float>;
    extern template class stack<double>;
    extern template class stack<std::string>;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
#endif
