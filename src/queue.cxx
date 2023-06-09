//  --------------------------------------------------------------------------
//  -----------------------------------------size_---------------------------------
    #include <ds/queue.hxx>
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------




// start queue
// start queue
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    queue<T>::queue() noexcept requires( std::default_initializable<T> )
        : m_head( new (std::nothrow) node_type( T{} ) )
        , m_size{ 0x0UL }
    {
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    queue<T>::queue( queue const & p_outer ) noexcept
        : queue()
    {
        node_t v_node = p_outer.m_head->m_next;

        while ( v_node != p_outer.m_head )
        {
            push_back(v_node->m_data);
            
            v_node = v_node->m_next;
        }
    } 
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    queue<T>::queue( queue && p_outer ) noexcept
        : queue()
    {
        swap( *this, p_outer );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    auto queue<T>::operator=( queue p_rhs ) noexcept -> decltype(auto)
    {
        swap( *this, p_rhs );
        return *this;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    void queue<T>::push_back( T const& p_value ) noexcept
    {
        m_head->push_back( new (std::nothrow) node_type{ p_value } );

        m_size = -(~m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    void queue<T>::push( T const& p_value ) noexcept
    {
        m_head->push_back( new (std::nothrow) node_type{ p_value } );

        m_size = -(~m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    void queue<T>::pop_front()
    {
        assert( not empty() );

        node_t v_curr = m_head->m_next;
        
        m_head->m_next          = v_curr->m_next;
        v_curr->m_next->m_prev  = m_head;
        v_curr                  = ( free(v_curr), nullptr );

        m_size = ~(-m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    void queue<T>::pop()
    {
        pop_front();
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto queue<T>::empty() const noexcept -> bool 
    {
        return ( ( m_head == m_head->m_prev ) && ( m_head == m_head->m_next ) );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto queue<T>::size() const noexcept -> queue::size_type
    {
        return m_size;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------





//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto queue<T>::peek() const noexcept -> std::optional<node_type>
    {
        return ( not empty()
                ? std::optional{ *m_head->m_next }
                : std::nullopt
            );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------





//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto queue<T>::front() noexcept -> std::optional<node_type>
    {
        return peek();
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------





//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto queue<T>::front() const noexcept -> std::optional<node_type>
    {
        return peek();
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------





//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto queue<T>::back() noexcept -> std::optional<node_type>
    {
        return ( not empty()
                ? std::optional{ *m_head->m_prev }
                : std::nullopt
            );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------





//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto queue<T>::back() const noexcept -> std::optional<node_type>
    {
        return ( not empty()
                ? std::optional{ *m_head->m_prev }
                : std::nullopt
            );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------





//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <typename T>
    queue<T>::~queue()
    {
       while ( not empty() )
       {
            pop_front();
       }

       m_head = ( free(m_head), nullptr );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template class queue<int>;
    template class queue<char>;
    template class queue<long>;
    template class queue<short>;
    template class queue<unsigned int>;
    template class queue<unsigned char>;
    template class queue<unsigned long>;
    template class queue<unsigned short>;
    template class queue<float>;
    template class queue<double>;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------