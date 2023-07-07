#ifndef LINKEDLIST_SENTINEL_NODE
#define LINKEDLIST_SENTINEL_NODE
// https://godbolt.org/z/1z11Mvzx5

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <type_traits>
#include <utility>

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
template <typename> class list;
template <typename> class Iterator;
template <typename> class Forward;
template <typename> class Backward;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
/* START CONSTRAINTS */

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename _Ty>
    concept is_class = std::is_class<_Ty>::value;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename _Ty, typename U>
    concept non_self = not std::is_same<std::decay_t<_Ty>, U>::value &&
                       not std::is_base_of<U, std::decay_t<_Ty>>::value;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

/* END CONSTRAINTS */

//
template <typename _Ty>
    class node final
{
public:
    friend class list <_Ty>;
    friend class Iterator <_Ty>;
    friend class Forward <_Ty>;
    friend class Backward <_Ty>;

public:
    using value_type       = _Ty;
    using reference        = std::add_lvalue_reference_t<_Ty>;
    using const_reference  = std::add_lvalue_reference_t<std::add_const_t<_Ty>>;
    using pointer          = std::add_pointer_t<_Ty>;
    using const_pointer    = std::add_pointer_t<std::add_const_t<_Ty>>;

public:
    using node_ptr_t = std::add_pointer_t<node<_Ty>>;

public:
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        node() noexcept requires(std::default_initializable<_Ty>)
            : m_data{_Ty{}}
            , m_prev{this}
            , m_next{this}
        {}
    //  --------------------------------------------------------------------------
        explicit node(_Ty const &p_data) noexcept requires(std::copy_constructible<_Ty>)
            : m_data{p_data}
            , m_prev{this}
            , m_next{this}
        {}
    //  --------------------------------------------------------------------------
        explicit node(_Ty &&p_data) noexcept requires(std::move_constructible<_Ty>)
            : m_data{std::move(p_data)}
            , m_prev{this}
            , m_next{this}
        {}
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

public:
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto data()       noexcept -> decltype(auto) { return m_data; }
    //  --------------------------------------------------------------------------
        auto data() const noexcept -> decltype(auto) { return m_data; }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

public:
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        void push_front(node_ptr_t p_node) noexcept
        {
            p_node->m_next = m_next;
            p_node->m_prev = this;
            m_next->m_prev = p_node;
            m_next         = p_node;
        }
    //  --------------------------------------------------------------------------
        void push_back(node_ptr_t p_node) noexcept
        {
            p_node->m_prev = m_prev;
            p_node->m_next = this;
            m_prev->m_next = p_node;
            m_prev         = p_node;
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

private:
    value_type m_data;
    node_ptr_t m_prev;
    node_ptr_t m_next;
};

//  //

//* END NODE *//

// //
template <typename _Ty>
    class Iterator
{
    using node_type = node<_Ty>;

public:
    using value_type         = _Ty;
    using difference_type    = std::ptrdiff_t;

public:
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        Iterator() noexcept = default;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        Iterator(typename node_type::node_ptr_t p_node) noexcept
            : m_node(p_node)
        {}
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto operator*()       noexcept -> value_type & { return m_node->m_data; }
    //  --------------------------------------------------------------------------
        auto operator*() const noexcept -> value_type & { return m_node->m_data; }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        [[nodiscard]] friend auto operator==(Iterator const &p_lhs,
                                             Iterator const &p_rhs) noexcept -> bool
        {
            return (p_lhs.m_node == p_rhs.m_node);
        }
    //  --------------------------------------------------------------------------
        [[nodiscard]] friend auto operator!=(Iterator const &p_lhs,
                                             Iterator const &p_rhs) noexcept -> bool
        {
            return (p_lhs.m_node != p_rhs.m_node);
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

protected:
    typename node_type::node_ptr_t m_node;
};

//  //

//*****//

// //
template <typename _Ty>
    class Forward final : public Iterator<_Ty> 
{
protected:
    using Iterator<_Ty>::m_node;

public:
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        using Iterator<_Ty>::Iterator;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto operator++() noexcept -> Forward &
        {
            m_node = m_node->m_next;
            return *this;
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto operator++(int) noexcept -> Forward
        {
            auto v_tmp = *this;
            return (++(*this), v_tmp);
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto operator--() noexcept -> Forward &
        {
            m_node = m_node->m_prev;
            return *this;
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto operator--(int) noexcept -> Forward
        {
            auto v_tmp = *this;
            return (--(*this), v_tmp);
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
};

//  //

//*****//

// //
template <typename _Ty>
    class Backward final : public Iterator<_Ty>
{
protected:
    using Iterator<_Ty>::m_node;

public:
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        using Iterator<_Ty>::Iterator;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto operator++() noexcept -> Backward &
        {
            m_node = m_node->m_prev;
            return *this;
        }
    //  --------------------------------------------------------------------------
        auto operator++(int) noexcept -> Backward
        {
            auto v_tmp = *this;
            return ((++(*this)), v_tmp);
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto operator--() noexcept -> Backward &
        {
            m_node = m_node->m_next;
            return *this;
        }
    //  --------------------------------------------------------------------------
        auto operator--(int) noexcept -> Backward
        {
            auto v_tmp = *this;
            return (--(*this), v_tmp);
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
};

//  //

//*****//

// //
template <typename _Ty>
    class list final
{
private:
    using node_type  = node<_Ty>;
    using node_t     = typename node_type::node_ptr_t;

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
public:
    using value_type       = _Ty;
    using reference        = std::add_lvalue_reference_t<_Ty>;
    using const_reference  = std::add_lvalue_reference_t<std::add_const_t<_Ty>>;
    using pointer          = std::add_pointer_t<_Ty>;
    using const_pointer    = std::add_pointer_t<std::add_const_t<_Ty>>;
    using size_type        = std::size_t;
    using difference_type  = std::ptrdiff_t;

    using iterator                = Forward<_Ty>;
    using const_iterator          = Forward<const _Ty>;
    using reverse_iterator        = Backward<_Ty>;
    using const_reverse_iterator  = Backward<const _Ty>;
    using iterator_category       = std::bidirectional_iterator_tag;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

public:
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        list() noexcept requires(std::default_initializable<_Ty>);
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        list(std::initializer_list<_Ty>) noexcept;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        list(list &&) noexcept;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        list(list const &) noexcept;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        template <std::ranges::input_range R>
        list(R &&) noexcept requires(std::convertible_to<std::ranges::range_value_t<R>, _Ty>);
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------



    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        template <std::input_iterator I, std::sentinel_for<I> S>
        list(I, S) noexcept requires( std::convertible_to<std::iter_value_t<I>, _Ty>);
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------





    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto operator=(list) noexcept -> decltype(auto);
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        void push_front(_Ty const &) noexcept;
    //  --------------------------------------------------------------------------
        void push_back(_Ty const &)  noexcept;
    //  --------------------------------------------------------------------------
        void push_before(_Ty const &, const size_type) noexcept;
    //  --------------------------------------------------------------------------
        void push_after(_Ty const &, const size_type)  noexcept;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        template <typename... ARGS>
        void emplace_back(ARGS &&...) noexcept
            requires(is_class<_Ty> && std::constructible_from<_Ty, ARGS...>);
    //  --------------------------------------------------------------------------
        template <typename... ARGS>
        void emplace_front(ARGS &&...) noexcept
            requires(is_class<_Ty> && std::constructible_from<_Ty, ARGS...>);
    //  --------------------------------------------------------------------------
        template <typename... ARGS>
        void emplace_before(const size_type, ARGS &&...) noexcept
            requires(is_class<_Ty> && std::constructible_from<_Ty, ARGS...>);
    //  --------------------------------------------------------------------------
        template <typename... ARGS>
        void emplace_after(const size_type, ARGS &&...) noexcept
            requires(is_class<_Ty> && std::constructible_from<_Ty, ARGS...>);
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        void pop_front();
    //  --------------------------------------------------------------------------
        void pop_back();
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        [[nodiscard]] auto empty() const noexcept -> bool;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        [[nodiscard]] auto size() const noexcept -> size_type;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        [[nodiscard]] auto front() const noexcept -> std::optional<node_type>;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        [[nodiscard]] auto back() const noexcept -> std::optional<node_type>;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto begin()        noexcept -> iterator;
    //  --------------------------------------------------------------------------
        auto begin()  const noexcept -> iterator;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto end()        noexcept -> iterator;
    //  --------------------------------------------------------------------------
        auto end()  const noexcept -> iterator;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto rbegin()        noexcept -> reverse_iterator;
    //  --------------------------------------------------------------------------
        auto rbegin()  const noexcept -> reverse_iterator;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto rend()        noexcept -> reverse_iterator;
    //  --------------------------------------------------------------------------
        auto rend()  const noexcept -> reverse_iterator;
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        friend void swap(list &p_lhs, list &p_rhs)
        {
            using std::swap;

            swap(p_lhs.m_head, p_rhs.m_head);
            swap(p_lhs.m_size, p_rhs.m_size);
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        ~list();
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

private:
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        node_t at(const size_type p_pos) noexcept
        {
            assert(p_pos < m_size);

            node_t    v_tmp = nullptr;
            size_type v_mid = (m_size / 2);

            if (v_mid < p_pos)
            {
                auto v_index = ( m_size - 1ul );
                v_tmp        = m_head->m_prev;

                while (v_index > p_pos)
                {
                    v_tmp   = v_tmp->m_prev;
                    v_index = ~(-v_index);
                }
            } else {

                auto v_index{0ul};

                v_tmp = m_head->m_next;

                while (v_index < p_pos)
                {
                    v_tmp   = v_tmp->m_next;
                    v_index = -(~v_index);
                }
            }

            return v_tmp;
        }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

private:
    node_t    m_head;
    size_type m_size;
};




//  //

//*****//

// //
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template<typename _Ty>
    template <std::input_iterator I, std::sentinel_for<I> S>
    list<_Ty>::list(I first, S last) noexcept requires( std::convertible_to<std::iter_value_t<I>, _Ty>)
        : list{}
    {
        using std::placeholders::_1;
        
        std::ranges::for_each( first, last, std::bind( &list::push_back, this, _1) );
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
template <typename _Ty>
template <std::ranges::input_range R>
list<_Ty>::list(R &&p_list) noexcept requires(std::convertible_to<std::ranges::range_value_t<R>, _Ty>)
    : list( std::ranges::begin(p_list), std::ranges::end(p_list) )
{
}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
template <typename _Ty>
template <typename... ARGS>
void list<_Ty>::emplace_front(ARGS &&...p_args) noexcept requires(is_class<_Ty> && std::constructible_from<_Ty, ARGS...>)
{
    m_head->push_front( new (std::nothrow) node_type{_Ty(std::forward<ARGS>(p_args)...)} );

    m_size = -(~m_size);
}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
template <typename _Ty>
template <typename... ARGS>
void list<_Ty>::emplace_back(ARGS &&...p_args) noexcept requires(is_class<_Ty> && std::constructible_from<_Ty, ARGS...>)
{
    m_head->push_back( new (std::nothrow) node_type{_Ty(std::forward<ARGS>(p_args)...)} );

    m_size = -(~m_size);
}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
template <typename _Ty>
template <typename... ARGS>
void list<_Ty>::emplace_before(const size_type p_pos,
                                        ARGS &&...p_args) noexcept
    requires(is_class<_Ty> && std::constructible_from<_Ty, ARGS...>)
{
    node_t v_target = at(p_pos);
    
    v_target->push_back( new (std::nothrow) node_type{_Ty(std::forward<ARGS>(p_args)...)} );

    m_size = -(~m_size);
}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
template <typename _Ty>
template <typename... ARGS>
void list<_Ty>::emplace_after(const size_type p_pos,
                                       ARGS &&...p_args) noexcept requires(is_class<_Ty> && std::constructible_from<_Ty, ARGS...>)
{
    node_t v_target = at(p_pos);

    v_target->push_front( new (std::nothrow) node_type{_Ty(std::forward<ARGS>(p_args)...)} );
    
    m_size = -(~m_size);
}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
extern template class list<int>;
extern template class list<char>;
extern template class list<long>;
extern template class list<short>;
extern template class list<unsigned int>;
extern template class list<unsigned char>;
extern template class list<unsigned long>;
extern template class list<unsigned short>;
extern template class list<float>;
extern template class list<double>;
extern template class list<std::string>;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------





//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <class T>
    inline constexpr bool std::ranges::enable_borrowed_range<list<T>> = true;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


#endif
// // end linked_list
