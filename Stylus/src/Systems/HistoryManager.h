#pragma once

#include <deque>
#include <utility>

namespace Stylus {

	template<typename T>
	class HistoryManager
	{
	public:
		explicit HistoryManager(size_t maxCapacity = 30)
			: m_MaxCapacity(maxCapacity) {}
		~HistoryManager() = default;

		// Deleted to stop accidental copying of 1GB+ RAM of saved history
		HistoryManager(const HistoryManager&) = delete;
		HistoryManager& operator=(const HistoryManager&) = delete;

		HistoryManager(HistoryManager&&) noexcept = default;
		HistoryManager& operator=(HistoryManager&&) noexcept = default;

		template<typename U>
		void ActionPerformed(U&& data)
		{
			if (!m_History.empty() && m_CurrentIndex < m_History.size() - 1)
			{
				m_History.erase(m_History.begin() + m_CurrentIndex + 1, m_History.end());
			}

			if (m_History.size() > m_MaxCapacity)
			{
				m_History.pop_front();
			}

			m_History.push_back(std::forward<U>(data));
			m_CurrentIndex = m_History.size() - 1;
		}

		[[nodiscard]] const T* UndoHistory()
		{
			if (!CanUndo())
			{
				return nullptr;
			}

			m_CurrentIndex--;
			return &m_History[m_CurrentIndex];
		}

		[[nodiscard]] const T* RedoHistory()
		{
			if (!CanRedo())
			{
				return nullptr;
			}

			m_CurrentIndex++;
			return &m_History[m_CurrentIndex];
		}

		[[nodiscard]] const T* GetCurrentState() const
		{
			if (m_History.empty())
			{
				return nullptr;
			}

			return &m_History[m_CurrentIndex];
		}

		[[nodiscard]] bool CanUndo() const
		{
			return !m_History.empty() && m_CurrentIndex > 0;
		}

		[[nodiscard]] bool CanRedo() const
		{
			return !m_History.empty() && m_CurrentIndex < m_History.size() - 1;
		}

		void Clear()
		{
			m_History.clear();
			m_CurrentIndex = 0;
		}
	private:
		size_t m_MaxCapacity;
		size_t m_CurrentIndex = 0;
		std::deque<T> m_History;
	};

}
