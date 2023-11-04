//  ---------------------------------
//  ---------------------------------
        #include <ds/queue.hxx>
//  ---------------------------------
//  ---------------------------------


// start queue
// start queue
//  
//  
template <class T>
queue<T>::queue() noexcept
    : m_head( new(std::nothrow) node_type{} )
    , m_size( 0ul )
{}

 
//  
template <class T>
queue<T>::queue(queue const& p_outer) noexcept : queue()
{
    node_type* v_curr = p_outer.m_head->m_next;
    node_type* v_node = nullptr;

    while (v_curr != p_outer.m_head)
    {
        v_node = new(std::nothrow) node_type(v_curr->m_data);
        
        if ( v_node == nullptr ) break;

        m_head->push_back(v_node);
        v_curr = v_curr->m_next;
    }
 

    m_size = p_outer.m_size;
}


//  
template <class T>
queue<T>::queue(queue&& p_outer)  noexcept : queue()
{
    swap(*this, p_outer);
}


//  
template <class T>
auto queue<T>::operator=(queue p_rhs) noexcept -> queue &
{
    swap(*this, p_rhs);

    return *this;
}


//  
template <class T>
void queue<T>::pop_front() noexcept
{
    assert(not empty());

    node_type* v_curr = m_head->m_next;

    m_head->m_next         = v_curr->m_next;
    v_curr->m_next->m_prev = m_head;

    v_curr = (delete v_curr, nullptr);

    m_size = m_size - 1ul;
}

 
//  
template <class T>
void queue<T>::pop() noexcept
{
    pop_front();
}

 
//  
template <class T>
[[nodiscard]] auto queue<T>::empty() const noexcept -> bool
{
    return ((m_head == m_head->m_prev) && (m_head == m_head->m_next));
}

 
//  
template <class T>
[[nodiscard]] auto queue<T>::size() const noexcept 
    -> typename queue::size_type
{
    return m_size;
}

 
//  
template <class T>
[[nodiscard]] auto queue<T>::peek() const noexcept
    -> std::optional<typename queue::value_type>
{
    return not empty()  ? std::optional{m_head->m_next->data()}
                        : std::nullopt;
}
//  
template <class T>
[[nodiscard]] auto queue<T>::peek() noexcept
    -> std::optional<typename queue::value_type>
{
    return not empty()  ? std::optional{m_head->m_next->data()}
                        : std::nullopt;
}

  
//  
template <class T>
[[nodiscard]] auto queue<T>::front() noexcept
    -> std::optional<typename queue::value_type>
{
    return peek();
}


//  
template <class T>
[[nodiscard]] auto queue<T>::front() const noexcept
    -> std::optional<typename queue::value_type>
{
    return peek();
}

 
//  
template <class T>
[[nodiscard]] auto queue<T>::back() noexcept
    -> std::optional<typename queue::value_type>
{
    return not empty()  ? std::optional{m_head->m_prev->data()}
                        : std::nullopt;
}


//  
template <class T>
[[nodiscard]] auto queue<T>::back() const noexcept
    -> std::optional<typename queue::value_type>
{
    return not empty()  ? std::optional{m_head->m_prev->data()}
                        : std::nullopt;
}

 
//  
template <class T>
queue<T>::~queue() noexcept
{
    while (not empty())
    {
        pop_front();
    }

    m_head = (delete m_head, nullptr);
}
//  
//  

//  
//  
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
//  
//  