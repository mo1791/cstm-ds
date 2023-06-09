//  ---------------------------------------------------
//  ---------------------------------------------------
    #include <ds/linked_list.hxx>
//  ---------------------------------------------------
//  ---------------------------------------------------

//  start linked_list

//  start list

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    list<T>::list() noexcept requires( std::default_initializable<T> )
        : m_head( new (std::nothrow) node_type{ T{} } )
        , m_size{ 0x0ul }
    {
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    list<T>::list( std::initializer_list<T> p_list ) noexcept
        : list( std::ranges::begin(p_list), std::ranges::end(p_list) )
    {
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------




//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    list<T>::list( list const & p_outer ) noexcept
        : list( std::ranges::begin(p_outer), std::ranges::end(p_outer) )
    {
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    list<T>::list( list && p_outer ) noexcept
        : list{}
    {
        swap( *this, p_outer );
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    auto list<T>::operator=( list p_list ) noexcept -> decltype(auto)
    {
        swap( *this, p_list );
        return *this;
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------




//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    void list<T>::push_front( T const& p_data ) noexcept
    {
        m_head->push_front( new (std::nothrow) node_type{ p_data } );
        
        m_size = -(~m_size);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    void list<T>::push_back( T const& p_data ) noexcept
    {
        m_head->push_back( new (std::nothrow) node_type{ p_data } );

        m_size = -(~m_size);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    void list<T>::push_before( T const& p_data, const size_type p_pos ) noexcept
    {
        node_t v_target = at(p_pos);

        v_target->push_back( new (std::nothrow) node_type{ p_data } );
        
        m_size = -(~m_size);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    void list<T>::push_after( T const& p_data, const size_type p_pos ) noexcept
    {
        node_t v_target = at(p_pos);

        v_target->push_front( new (std::nothrow) node_type{ p_data } );

        m_size = -(~m_size);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    void list<T>::pop_front()
    {
        assert( not empty() );

        node_t v_curr  = m_head->m_next;
        
        v_curr->m_next->m_prev  = m_head;
        m_head->m_next          = v_curr->m_next;
        v_curr                  = ( free(v_curr), nullptr );
        
        m_size = ~(-m_size);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    void list<T>::pop_back()
    {
        assert( not empty() );

        node_t v_curr = m_head->m_prev;
        
        v_curr->m_prev->m_next = m_head;
        m_head->m_prev         = v_curr->m_prev;
        v_curr                 = ( free(v_curr), nullptr);

        m_size = ~(-m_size);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto list<T>::empty() const noexcept -> bool
    {
        return ( ( m_head == m_head->m_next ) && ( m_head == m_head->m_prev ) );
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto list<T>::size() const noexcept -> typename list::size_type
    {
        return m_size;
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto list<T>::front() const noexcept -> std::optional<node_type>
    {
        return ( not empty() 
                ? std::optional{ *m_head->m_next }
                : std::nullopt );
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    [[nodiscard]] auto list<T>::back() const noexcept -> std::optional<node_type>
    {
        return ( not empty() 
                ? std::optional{ *m_head->m_prev }
                : std::nullopt );
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    auto list<T>::begin() noexcept -> typename list::iterator
    {
        return iterator{ m_head->m_next };
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    auto list<T>::begin() const noexcept -> typename list::iterator
    {
        return iterator{ m_head->m_next };
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    auto list<T>::end() noexcept -> typename list::iterator
    {
        return iterator{ m_head };
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    auto list<T>::end() const noexcept -> typename list::iterator
    {
        return iterator{ m_head };
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    auto list<T>::rbegin() noexcept -> typename list::reverse_iterator
    {
        return reverse_iterator{ m_head->m_prev };
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    auto list<T>::rbegin() const noexcept -> typename list::reverse_iterator
    {
        return reverse_iterator{ m_head->m_prev };
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    auto list<T>::rend() noexcept -> typename list::reverse_iterator
    {
        return reverse_iterator{ m_head };
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    auto list<T>::rend() const noexcept -> typename list::reverse_iterator
    {
        return reverse_iterator{ m_head };
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <typename T>
    list<T>::~list()
    {   
        std::ranges::for_each( *this, std::bind(&list::pop_front, this ) );
        m_head = ( free(m_head), nullptr );
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template class linked_list<int>;
    template class linked_list<char>;
    template class linked_list<long>;
    template class linked_list<short>;
    template class linked_list<unsigned int>;
    template class linked_list<unsigned char>;
    template class linked_list<unsigned long>;
    template class linked_list<unsigned short>;
    template class linked_list<float>;
    template class linked_list<double>;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
