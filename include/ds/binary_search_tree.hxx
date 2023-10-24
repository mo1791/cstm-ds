#ifndef BINARY_SEARCH_TREE_NODE
#define BINARY_SEARCH_TREE_NODE



#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>


namespace rng = std::ranges;

/** START CONCEPT **/

/** IS_CLASS CONCEPT (satisfied if and only if T is a CLASS type) **/
template <class T> concept is_class = std::is_class_v<T>;

/** END CONCEPTS **/

/** FROWARD DECL **/
template <std::totally_ordered> struct binary_search_tree;

/** START NODE **/

template <class T> struct node final
{

    friend struct binary_search_tree<T>;

public:
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;

   public:
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** DEFAULT CTOR **/
        node() noexcept
            requires(std::default_initializable<T>)
            : m_data(T{})
            , m_left(nullptr)
            , m_right(nullptr)
            , m_parent(nullptr)
        {}
    //  -----------------------------------------------------------------------
        /** PARAM CTOR **/
        explicit node(T const& data) noexcept
            requires(std::copy_constructible<T>)
            : m_data(data)
            , m_left(nullptr)
            , m_right(nullptr)
            , m_parent(nullptr)
        {}
    //  -----------------------------------------------------------------------
        /** PARAM CTOR ( rvalue ref ) **/
        explicit node(T&& data) noexcept
            requires(std::move_constructible<T>)
            : m_data(std::move(data)),
            m_left(nullptr),
            m_right(nullptr),
            m_parent(nullptr)
        {}
    //  -----------------------------------------------------------------------
        /** **/
        template <class... ARGS>
        node(ARGS&&... p_args) noexcept
            requires(is_class<T> && std::constructible_from<T, ARGS...>)
            : m_data(std::forward<ARGS>(p_args)...)
            , m_left(nullptr)
            , m_right(nullptr)
            , m_parent(nullptr) {}
    //  -----------------------------------------------------------------------
    //	-----------------------------------------------------------------------

private:
    T     m_data;
    node* m_left;
    node* m_right;
    node* m_parent;
};

/** **/

/** **/

template <std::totally_ordered T>
struct binary_search_tree
{

public:
    using node_t = node<T>;

public:
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;
    using size_type       = std::size_t;

   public:
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** DEFAULT CTOR **/
        binary_search_tree() noexcept;
    //  -----------------------------------------------------------------------
        /** INITIALIZER_LIST CTOR **/
        binary_search_tree(std::initializer_list<T> /* list */);
    //  -----------------------------------------------------------------------
        /** RANGE TYPE CTOR  **/
        template <rng::range R>
        binary_search_tree(R&& /* range */)
            requires(std::convertible_to<rng::range_value_t<R>, T>);
    //  -----------------------------------------------------------------------
    /**  ITERATOR TYPE CTOR **/
        template <std::input_iterator I, std::sentinel_for<I> S>
        binary_search_tree(I /* begin */, S /* end */)
            requires(std::convertible_to<std::iter_value_t<I>, T>);
    //  -----------------------------------------------------------------------
        /** COPY CTOR  **/
        binary_search_tree(binary_search_tree const& /* outer */);
    //  -----------------------------------------------------------------------
        /** MOVE CTOR **/
        binary_search_tree(binary_search_tree&& /* outer */) noexcept;
    //  -----------------------------------------------------------------------
        /** ASSIGNMENT **/
        binary_search_tree& operator=(binary_search_tree /* rhs */);
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** INSERTING A NODE IN TREE **/
        template <class U>
        void insert(U&& /* data */)
            requires(std::convertible_to<std::decay_t<U>, T>);
    //  -----------------------------------------------------------------------
        /** INSERTING A NODE IN TREE ( construct in place ) **/
        template <class... Args>
        void emplace(Args&&... /* args */)
            requires(is_class<T> && std::constructible_from<T, Args...>);
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** SEARCH FOR A NODE IN TREE **/
        auto search(T const& /* key */) noexcept -> std::optional<T>;
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** DELETE A NODE FROM TREE **/
        void remove(T const& /* key */) noexcept;
    //  -----------------------------------------------------------------------
        /** MAKE TREE EMPTY **/
        void clear() noexcept;
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** PRINT TREE NODES IN INORDER **/
        void print_inorder() const noexcept;
    //  -----------------------------------------------------------------------
        /** PRINT TREE NODES IN PRE_ORDER **/
        void print_preorder() const noexcept;
    //  -----------------------------------------------------------------------
        /** PRINT TREE NODES IN POST_ORDER **/
        void print_postorder() const;
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** EMPTYNESS CHECK **/
        [[nodiscard]] constexpr auto empty() const noexcept -> bool;
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** TREE SIZE **/
        [[nodiscard]] constexpr auto size() const noexcept -> size_type;
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** MAX DATA VALUE **/
        [[nodiscard]] auto max() const noexcept -> std::optional<T>;
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** MIN DATA VALUE **/
        [[nodiscard]] auto min() const noexcept -> std::optional<T>;
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** CLONE BINARY TREE **/
        [[nodiscard]] auto clone() const -> node_t*;
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** DTOR **/
        ~binary_search_tree() noexcept;
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

private:
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** CLONE BINARY TREE ( HELPER ) **/
        [[nodiscard]] auto clone(node_t* p_root) const -> node_t*
        {
            node_t* v_node = nullptr;

            if (p_root != nullptr)
            {
                try {
                    auto v_current = new node_t(p_root->m_data);
                    auto v_copy = v_current;

                    while (p_root != nullptr)
                    {
                        if ((p_root->m_left != nullptr) && (not v_copy->m_left))
                        {
                            v_node = new node_t(p_root->m_left->m_data);

                            v_copy->m_left           = v_node;
                            v_copy->m_left->m_parent = v_copy;

                            p_root = p_root->m_left;
                            v_copy = v_copy->m_left;
                        } 
                        else if ((p_root->m_right != nullptr) &&
                                (not v_copy->m_right))
                        {
                            v_node = new node_t(p_root->m_right->m_data);

                            v_copy->m_right           = v_node;
                            v_copy->m_right->m_parent = v_copy;

                            p_root = p_root->m_right;
                            v_copy = v_copy->m_right;
                        }
                        else
                        {
                            p_root = p_root->m_parent;
                            v_copy = v_copy->m_parent;
                        }
                    }

                    return v_current;

                } catch (...) {
                    v_node = (delete v_node, nullptr);

                    throw;
                }
            }

            return p_root;
        }
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------
        /** SWAP **/
        void swap(binary_search_tree& p_lhs, binary_search_tree& p_rhs) noexcept
        {
            using std::swap;

            swap(p_lhs.m_root, p_rhs.m_root);
            swap(p_lhs.m_size, p_rhs.m_size);
        }
    //  -----------------------------------------------------------------------
    //  -----------------------------------------------------------------------

private:
    node_t* m_root;
    size_type m_size;
};

/** END TREE **/

// //

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** **/
    template <std::totally_ordered T>
    template <rng::range R>
    binary_search_tree<T>::binary_search_tree(R&& p_range)
        requires(std::convertible_to<rng::range_value_t<R>, T>)
        : binary_search_tree(rng::begin(p_range), rng::end(p_range))
    {}
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** **/
    template <std::totally_ordered T>
    template <std::input_iterator I, std::sentinel_for<I> S>
    binary_search_tree<T>::binary_search_tree(I p_begin, S p_end)
        requires(std::convertible_to<std::iter_value_t<I>, T>)
        : binary_search_tree()
    {
        using std::placeholders::_1;

        rng::for_each(p_begin, p_end,
                    std::bind(&binary_search_tree::template insert<std::iter_reference_t<I>>,
                                this, _1));
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** **/
    template <std::totally_ordered T>
    template <class U>
    void binary_search_tree<T>::insert(U&& p_data)
        requires(std::convertible_to<std::decay_t<U>, T>)
    {
        node_t* v_node = nullptr;

        try {
            v_node = new node_t(std::forward<U>(p_data));
        } catch (...) {
            v_node = (delete v_node, nullptr);

            throw;
        }

        if (not m_root)
        {
            m_root = v_node;
            m_size = -(~m_size);

            return;
        }

        node_t *v_current = m_root, *v_parent = nullptr;

        while (v_current != nullptr)
        {
            v_parent = v_current;

            if (p_data <= v_current->m_data)
            {
                v_current = v_current->m_left;
            } 
            else
            {
                v_current = v_current->m_right;
            }
        }

        v_node->m_parent = v_parent;

        if (not v_parent)
        {
            v_parent = v_node;
            m_size   = -(~m_size);

            return;
        }

        if (p_data <= v_parent->m_data)
        {
            v_parent->m_left = v_node;
        }
        else
        {
            v_parent->m_right = v_node;
        }

        m_size = -(~m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** **/
    template <std::totally_ordered T>
    template <class... Args>
    void binary_search_tree<T>::emplace(Args&&... p_args)
        requires(is_class<T> && std::constructible_from<T, Args...>)
    {
        node_t* v_node = nullptr;

        try {
            v_node = new node_t(std::forward<Args>(p_args)...);
        } catch (...) {
            v_node = (delete v_node, nullptr);

            throw;
        }

        if (not m_root)
        {
            m_root = v_node;
            m_size = -(~m_size);

            return;
        }

        node_t *v_current = m_root, *v_parent = nullptr;

        while (v_current != nullptr)
        {
            v_parent = v_current;

            if (v_node->m_data <= v_current->m_data)
            {
                v_current = v_current->m_left;
            }
            else
            {
                v_current = v_current->m_right;
            }
        }

        v_node->m_parent = v_parent;

        if (not v_parent)
        {
            v_parent = v_node;
            m_size   = -(~m_size);

            return;
        }

        if (v_node->m_data <= v_parent->m_data)
        {
            v_parent->m_left = v_node;
        }
        else
        {
            v_parent->m_right = v_node;
        }

        m_size = -(~m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** USER DEFINED TYPE DEDUCTION **/
//  -----------------------------------------------------------------------
    template <rng::range R>
    binary_search_tree( R&& ) -> binary_search_tree<rng::range_value_t<R>>;
//  -----------------------------------------------------------------------
    template <std::input_iterator I, std::sentinel_for<I> S>
    binary_search_tree( I, S ) -> binary_search_tree<std::iter_value_t<I>>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
	extern template class binary_search_tree<int>;
	extern template class binary_search_tree<char>;
	extern template class binary_search_tree<long>;
	extern template class binary_search_tree<short>;
	extern template class binary_search_tree<unsigned int>;
	extern template class binary_search_tree<unsigned char>;
	extern template class binary_search_tree<unsigned long>;
	extern template class binary_search_tree<unsigned short>;
	extern template class binary_search_tree<float>;
	extern template class binary_search_tree<double>;
	extern template class binary_search_tree<std::string>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


#endif