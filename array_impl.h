#include <xmemory>
#include <stdexcept>

    template <typename ElementType, size_t Size>
    class cyclic_queue
    {
    public:
        using index_t           = size_t;
        using alloc_traits      = std::allocator_traits<std::allocator<ElementType>>;


    public:
        cyclic_queue() 
        {
            _head = 0;
            _tail = 0;
            _count = 0;
        }

        cyclic_queue(const cyclic_queue& right)
        {
            assign(right);
        }

        cyclic_queue(cyclic_queue&& right)
        {
            assign(std::move(right));
        }


    public:
        cyclic_queue& operator=(const cyclic_queue& right)
        {
            assign(right);
            return *this;
        }

        cyclic_queue& operator=(cyclic_queue&& right) noexcept
        {
            assign(std::move(right));
            return *this;
        }

        void push(const ElementType& elem)
        {
            emplace(elem);
        }

        void push(ElementType&& elem)
        {
            emplace(std::move(elem));
        }

        template <typename... Valty>
        void emplace(Valty&&... val)
        {
            if (full())
                throw std::logic_error("Queue is full!");

            alloc_traits::construct(_alloc, _elems + _head, std::forward<Valty>(val)...);

            ++_count;
            _head = next_index(_head);
        }

        ElementType pop()
        {
            if (empty())
                throw std::logic_error("Queue is empty!");

            ElementType elem = std::move(_elems[_tail]);

            --_count;
            _tail = next_index(_tail);
            return elem;
        }

        size_t count() const
        {
            return _count;
        }

        size_t size() const
        {
            return Size;
        }

        bool full() const
        {
            return _count == Size;
        }

        bool empty() const
        {
            return _count == 0;
        }

        void clear() noexcept
        {
            if (_elems == nullptr)
                return;

            alloc_traits::destroy(_alloc, _elems);
            _head = 0;
            _tail = 0;
            _count = 0;
        }


    private:
        ElementType _elems[Size];
        index_t _head;
        index_t _tail;

        size_t _count;

        std::allocator<ElementType> _alloc;


    private:
        void assign(const cyclic_queue& right)
        {
            if (this == std::addressof(right))
                return;

            _head = right._head;
            _tail = right._tail;
            _count = right._count;

            for (int i = 0; i < Size; ++i)
            {
                _elems[i] = right._elems[i];
            }
        }

        void assign(cyclic_queue&& right) noexcept
        {
            if (this == std::addressof(right))
                return;

            _head = right._head;
            _tail = right._tail;
            _count = right._count;

            right._head = 0;
            right._tail = 0;
            right._count = 0;

            for (int i = 0; i < Size; ++i)
            {
                _elems[i] = std::move(right._elems[i]);
            }
        }

        index_t next_index(index_t ind) const
        {
            ++ind;
            return ind >= Size ? 0 : ind;
        }

    };
