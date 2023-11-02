//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    #include <ds/binary_search_tree.hxx>
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

// start Tree

// start binary_search_tree

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** DEFAULT CTOR **/
    template <std::totally_ordered T>
    binary_search_tree<T>::binary_search_tree() noexcept
        : m_root(nullptr)
        , m_size(0ul)
    {}
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** INITIALIZER_LIST CTOR **/
    template <std::totally_ordered T>
    binary_search_tree<T>::binary_search_tree(std::initializer_list<T> p_list)
        : binary_search_tree(rng::begin(p_list), rng::end(p_list))
    {}
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** COPY CTOR **/
    template <std::totally_ordered T>
    binary_search_tree<T>::binary_search_tree(binary_search_tree const &p_outer)
        : binary_search_tree()
    {
        m_root = clone(p_outer.m_root);
        m_size = p_outer.m_size;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** MOVE CTOR **/
    template <std::totally_ordered T>
    binary_search_tree<T>::binary_search_tree(binary_search_tree &&p_outer) noexcept
        : binary_search_tree()
    {
        swap(*this, p_outer);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** ASSIGNMENT OPERATOR **/
    template <std::totally_ordered T>
    auto binary_search_tree<T>::operator=(binary_search_tree p_rhs)
        -> binary_search_tree &
    {
        swap(*this, p_rhs);

        return *this;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** SEARCH FOR KEY IN TREE **/
    template <std::totally_ordered T>
    auto binary_search_tree<T>::search(T const &p_key) noexcept
        -> std::optional<T>
    {
        if (auto v_current = m_root)
        {
            while (v_current != nullptr && (p_key != v_current->m_data))
            {
                if (p_key == v_current->m_data)
                {
                    return v_current->m_data;
                }
                else if (p_key < v_current->m_data)
                {
                    v_current = v_current->m_left;
                }
                else
                {
                    v_current = v_current->m_right;
                }
            }

            return std::nullopt;
        }

        return std::nullopt;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** REMOVE NODE FROM TREE **/
    template <std::totally_ordered T>
    void binary_search_tree<T>::remove(T const &p_key) noexcept
    {
        if (auto v_current = m_root)
        {
            while (v_current != nullptr)
            {
                if (p_key < v_current->m_data)
                {
                    v_current = v_current->m_left;
                } 
                else if (p_key > v_current->m_data)
                {
                    v_current = v_current->m_right;
                }
                else break;
            }

            if (not v_current)
            {
                return;
            }

            node_type* v_parent = v_current->m_parent;

            if ((not v_current->m_left) && (not v_current->m_right))
            {
                if (v_parent->m_left == v_current)
                {
                    v_parent->m_left = nullptr;
                }
                else
                {
                    v_parent->m_right = nullptr;
                }

                v_current = (delete v_current, nullptr);

                m_size = m_size - 1ul;

                return;
            }

            if ((not v_current->m_left) || (not v_current->m_right))
            {
                if (v_current->m_left != nullptr)
                {
                    v_parent->m_left            = v_current->m_left;
                    v_current->m_left->m_parent = v_parent;
                }
                else
                {
                    v_parent->m_right           = v_current->m_right;
                    v_parent->m_right->m_parent = v_parent;
                }

                v_current = (delete v_current, nullptr);

                m_size = m_size - 1ul;

                return;
            }

            if ((v_current->m_left != nullptr) && (v_current->m_right != nullptr))
            {
                node_type* v_temp = v_current->m_right;

                v_parent = nullptr;

                while (v_temp->m_left != nullptr)
                {
                    v_parent = v_temp;
                    v_temp = v_temp->m_left;
                }

                if (v_parent != nullptr)
                {
                    v_parent->m_left = v_temp->m_right;
                }
                else
                {
                    v_current->m_right = v_temp->m_right;

                    if (v_temp->m_right)
                    {
                        v_temp->m_right->m_parent = v_current;
                    }
                    else
                    {
                        v_temp->m_parent = v_current;
                    }
                }

                v_current->m_data = v_temp->m_data;

                v_temp = (delete v_temp, nullptr);

                m_size = m_size - 1ul;

                return;
            }
        }
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** CLEAR THE TREE **/
    template <std::totally_ordered T>
    void binary_search_tree<T>::clear() noexcept
    {
        node_type *v_current = m_root, *v_temp = nullptr;

        while (v_current != nullptr)
        {
            if (not v_current->m_left)
            {
                v_temp = v_current->m_right;
                
                v_current = (delete v_current, nullptr);
            }
            else
            {
                v_temp            = v_current->m_left;
                v_current->m_left = v_temp->m_right;
                v_temp->m_right   = v_current;
            }

            v_current = v_temp;
        }

        m_root = nullptr;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** PRINT TREE NODES IN INORDER **/
    template <std::totally_ordered T>
    void binary_search_tree<T>::print_inorder() const noexcept
    {
        if (not m_root) return;

        node_type *v_current = m_root, *v_temp = nullptr;

        // iterating tree nodes
        while (v_current != nullptr)
        {
            if (not v_current->m_left)
            {
                // Print node value
                std::cout << v_current->m_data << ' ';
                // When left child are empty then
                // visit to right child
                v_current = v_current->m_right;
            } else
            {
                v_temp = v_current->m_left;
                // Find rightmost node which is
                // equal to current node
                while (v_temp->m_right && (v_temp->m_right != v_current))
                {
                    // Visit to right subtree
                    v_temp = v_temp->m_right;
                }

                if (v_temp->m_right != nullptr)
                {
                    // Print node value
                    std::cout << v_current->m_data << ' ';
                    // Unlink
                    v_temp->m_right = nullptr;
                    // Visit to right child
                    v_current = v_current->m_right;
                }
                else
                {
                    // Change link
                    v_temp->m_right = v_current;
                    // Visit to right child
                    v_current = v_current->m_left;
                }
            }
        }

        std::cout << std::endl;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** PRINT TREE NODES IN PRE_ORDER **/
    template <std::totally_ordered T>
    void binary_search_tree<T>::print_preorder() const noexcept
    {
        if (not m_root) return;

        node_type *v_current = m_root, *v_temp = nullptr;

        // iterating tree nodes
        while (v_current != nullptr)
        {
            if (not v_current->m_left)
            {
                // Print node value
                std::cout << v_current->m_data << ' ';
                // Visit to right childs
                v_current = v_current->m_right;
            }
            else
            {
                v_temp = v_current->m_left;
                // Find rightmost node which is not
                // equal to current node
                while (v_temp->m_right && (v_temp->m_right != v_current))
                {
                    // Visit to right subtree
                    v_temp = v_temp->m_right;
                }

                if (v_temp->m_right != v_current)
                {
                    // Print node value
                    std::cout << v_current->m_data << ' ';
                    // Connect rightmost right node to current node
                    v_temp->m_right = v_current;
                    // Visit to left childs
                    v_current = v_current->m_left;
                }
                else
                {
                    // unlink
                    v_temp->m_right = nullptr;
                    // Visit to right child
                    v_current = v_current->m_right;
                }
            }
        }

        std::cout << std::endl;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** PRINT TREE NODES IN POST_ORDER **/
    template <std::totally_ordered T>
    void binary_search_tree<T>::print_postorder() const
    {
        if (not m_root) return;

        // Create a dummy node
        node_type *v_dummy = nullptr;

        try {
            v_dummy = new node_t{};
        } catch (...) {
            v_dummy = (delete v_dummy, nullptr);

            throw;
        }

        v_dummy->m_left = m_root;
        
        node_type *v_current = v_dummy;

        // Define some useful variables
        node_type *v_parent = nullptr;
        node_type *v_middle = nullptr;
        node_type *v_temp   = nullptr;
        node_type *v_back   = nullptr;

        // iterating tree nodes
        while (v_current != nullptr)
        {
            if (v_current->m_left == nullptr)
            {
                // When left child are empty then
                // Visit to right child
                v_current = v_current->m_right;
            }
            else
            {
                // Get to left child
                v_temp = v_current->m_left;

                while (v_temp->m_right != nullptr && v_temp->m_right != v_current)
                {
                    v_temp = v_temp->m_right;
                }

                if (v_temp->m_right != v_current)
                {
                    v_temp->m_right = v_current;
                    v_current       = v_current->m_left;
                }
                else
                {
                    v_parent = v_current;
                    v_middle = v_current->m_left;

                    // Update new path
                    while (v_middle != v_current)
                    {
                        v_back            = v_middle->m_right;
                        v_middle->m_right = v_parent;
                        v_parent          = v_middle;
                        v_middle          = v_back;
                    }

                    v_parent = v_current;
                    v_middle = v_temp;

                    // Print the resultant nodes.
                    // And correct node link in current path
                    while (v_middle != v_current)
                    {
                        std::cout << v_middle->m_data << ' ';

                        v_back            = v_middle->m_right;
                        v_middle->m_right = v_parent;
                        v_parent          = v_middle;
                        v_middle          = v_back;
                    }

                    // Unlink previous bind element
                    v_temp->m_right = nullptr;
                    // Visit to right child
                    v_current = v_current->m_right;
                }
            }
        }

        std::cout << std::endl;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** TREE SIZE **/
    template <std::totally_ordered T>
    [[nodiscard]] constexpr auto binary_search_tree<T>::size() const noexcept
        -> typename binary_search_tree::size_type
    {
        return m_size;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** EMPTYNESS CHECK **/
    template <std::totally_ordered T>
    [[nodiscard]] constexpr auto binary_search_tree<T>::empty() const noexcept
        -> bool
    {
        return (not m_root);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** MAX DATA VALUE **/
    template <std::totally_ordered T>
    [[nodiscard]] auto binary_search_tree<T>::max() const noexcept
        -> std::optional<value_type>
    {
        if (auto v_current = m_root)
        {
            while (v_current->m_right != nullptr) v_current = v_current->m_right;

            return v_current->m_data;
        }

        return std::nullopt;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** MIN DATA VALUE **/
    template <std::totally_ordered T>
    [[nodiscard]] auto binary_search_tree<T>::min() const noexcept
        -> std::optional<value_type>
    {
        if (auto v_current = m_root)
        {
            while (v_current->m_left != nullptr) v_current = v_current->m_left;

            return v_current->m_data;
        }

        return std::nullopt;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** DTOR **/
    template <std::totally_ordered T>
    binary_search_tree<T>::~binary_search_tree() noexcept
    {
        if (m_root != nullptr) clear();
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template class binary_search_tree<int>;
    template class binary_search_tree<char>;
    template class binary_search_tree<long>;
    template class binary_search_tree<short>;
    template class binary_search_tree<unsigned int>;
    template class binary_search_tree<unsigned char>;
    template class binary_search_tree<unsigned long>;
    template class binary_search_tree<unsigned short>;
    template class binary_search_tree<float>;
    template class binary_search_tree<double>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------