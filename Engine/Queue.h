#ifndef QUEUE_H
#define QUEUE_H

namespace Engine
{
	template <typename T>
	struct Node
	{
		T data;
		Node *next;
	};

	template <typename T>
	class Queue
	{
	public:
		Queue();
		~Queue();

		void Push(T item);
		T Pop();
		unsigned long Length();

	private:
		Node<T> *m_first;
		Node<T> *m_last;
		unsigned long m_length;
	};

	template <typename T>
	Queue<T>::Queue()
	{
		m_first = NULL;
		m_last = NULL;
		m_length = 0;
	}

	template <typename T>
	Queue<T>::~Queue()
	{
		while(Length() > 0)
		{
			Pop();
		}
	}

	template <typename T>
	void Queue<T>::Push(T item)
	{
		Node<T> *node;
		
		node = new Node<T>();
		node->data = item;
		node->next = NULL;

		if(m_last != NULL)
		{
			m_last->next = node;
		}
		else
		{
			m_first = node;
		}
		m_last = node;
		++m_length;
	}

	template <typename T>
	T Queue<T>::Pop()
	{
		T result;
		Node<T> *node;

		if(m_first != NULL)
		{
			node = m_first;
			m_first = m_first->next;
			if(m_last == node)
			{
				m_last = NULL;
			}
			result = node->data;
			delete node;
			--m_length;
		}

		return result;
	}

	template <typename T>
	unsigned long Queue<T>::Length()
	{
		return m_length;
	}
}

#endif // QUEUE_H